/**
 * @file redispp.cpp
 * @brief redis哨兵模式
 * @author
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022
 *
 * @par 修改日志:
 * <table>
 * <tr> <th>日期</th>       <th>作者</th> <th>修改说明</th> </tr>
 * <tr> <td>2022-11-02</td> <td></td> <td>初始创建</td> </tr>
 * </table>
 */
#include "redispp/redispp.h"

#include <vector>

namespace library
{
    namespace redis
    {
        // redis连接池配置
        struct RedisPool
        {
            int totalSize;                   // 当前总的连接数
            int usedSize;                    // 已经使用的连接数
            int idleSize;                    // 闲置的连接数
            std::mutex mtx;                  // 连接池锁
            std::queue<RedisPtr> redisQueue; // redis连接队列
        };

        void Redispp::Init(SentinelConfigArray sentinelConfigs, RedisConfigPtr redisConfig)
        {
            _sentinelConfigs = sentinelConfigs;
            _redisConfig = redisConfig;

            // 创建哨兵配置，支持多哨兵
            sw::redis::SentinelOptions ops;
            for (auto config : _sentinelConfigs)
            {
                std::pair<std::string, int> node;
                node.first = config->host;
                node.second = config->port;
                ops.nodes.push_back(node);
            }
            ops.password = _redisConfig->sentinelPasswd;
            ops.connect_timeout = std::chrono::milliseconds(_redisConfig->connectTimeout);
            ops.socket_timeout = std::chrono::milliseconds(_redisConfig->socketTimeout);

            // 创建哨兵对象
            _sentinel = std::make_shared<sw::redis::Sentinel>(ops);

            // 创建Redis连接池（所有数据库实例）
            for (int i = 0; i < MAX_DB_SIZE; i++)
            {
                std::shared_ptr<RedisPool> redisPool = std::make_shared<RedisPool>();
                redisPool->totalSize = 0;
                redisPool->usedSize = 0;
                redisPool->idleSize = 0;
                _redisConnPool.push_back(redisPool);
            }
        }

        RedisPtr Redispp::GetRedis(int db)
        {
            if (!_sentinel)
            {
                return nullptr;
            }

            // db=-1时，取默认配置文件中的数据库实例
            int idx = (-1 == db) ? _redisConfig->db : db;
            if (idx > MAX_DB_SIZE || idx < 0)
            {
                return nullptr;
            }

            RedisPtr redis = nullptr;
            auto redisPool = _redisConnPool[idx];

            // 若连接池不为空，则从连接池中取出一个连接
            std::unique_lock<std::mutex> lock(redisPool->mtx);
            std::queue<RedisPtr> &redisQueue = redisPool->redisQueue;
            if (!redisQueue.empty())
            {
                redis = redisQueue.front();
                redisQueue.pop();
                if (redis != nullptr)
                {
                    redisPool->usedSize++;
                    redisPool->idleSize--;
                    return redis;
                }

                // 将失效的连接去除
                redisPool->totalSize--;
            }

            // 若总连接数已经超过5倍的配置，则不再新建连接
            if (redisPool->totalSize >= _redisConfig->poolSize * 5)
            {
                return nullptr;
            }

            // 创建新的连接
            sw::redis::ConnectionOptions connectionOpts;
            connectionOpts.password = _redisConfig->redisPasswd;                                      // Optional. No password by default.
            connectionOpts.connect_timeout = std::chrono::milliseconds(_redisConfig->connectTimeout); // Required.
            connectionOpts.socket_timeout = std::chrono::milliseconds(_redisConfig->socketTimeout);   // Required.
            connectionOpts.db = idx;

            sw::redis::ConnectionPoolOptions poolOpts;
            poolOpts.size = _redisConfig->poolSize; // Optional. The default size is 1.
            redis = std::make_shared<sw::redis::Redis>(_sentinel, _redisConfig->masterName, (_redisConfig->master ? sw::redis::Role::MASTER : sw::redis::Role::SLAVE), connectionOpts, poolOpts);
            redisPool->totalSize++;
            redisPool->usedSize++;
            redisPool->idleSize = 0;
            return redis;
        }

        bool Redispp::GiveBack(RedisPtr redis, bool isvalid, int db)
        {
            // db=-1时，取默认配置文件中的数据库实例
            int idx = (-1 == db) ? _redisConfig->db : db;
            if (idx > MAX_DB_SIZE || idx < 0)
            {
                return false;
            }

            // 若连接已经失败，则重新创建一个连接再放回连接池
            if (!isvalid)
            {
                sw::redis::ConnectionOptions connectionOpts;
                connectionOpts.password = _redisConfig->redisPasswd;                                      // Optional. No password by default.
                connectionOpts.connect_timeout = std::chrono::milliseconds(_redisConfig->connectTimeout); // Required.
                connectionOpts.socket_timeout = std::chrono::milliseconds(_redisConfig->socketTimeout);   // Required.
                connectionOpts.db = idx;
                sw::redis::ConnectionPoolOptions poolOpts;
                poolOpts.size = _redisConfig->poolSize; // Optional. The default size is 1.
                redis = std::make_shared<sw::redis::Redis>(_sentinel, _redisConfig->masterName, (_redisConfig->master ? sw::redis::Role::MASTER : sw::redis::Role::SLAVE), connectionOpts, poolOpts);
            }

            // 将连接放回连接池
            auto redisPool = _redisConnPool[idx];
            std::unique_lock<std::mutex> lock(redisPool->mtx);
            redisPool->redisQueue.push(redis);
            redisPool->idleSize++;
            redisPool->usedSize--;
            return true;
        }

        RedisProxy::RedisProxy(int db)
        {
            _db = db;
            _ptr = Redispp::Instance()->GetRedis(db);
            _isValid = _ptr != nullptr;
        }

        RedisProxy::~RedisProxy()
        {
            Redispp::Instance()->GiveBack(_ptr, _isValid, _db);
        }

        void RedisProxy::SetInvalid()
        {
            _isValid = false;
        }

        RedisPtr RedisProxy::operator->() const
        {
            return _ptr;
        }

        RedisProxy::operator void *() const
        {
            return _ptr.get();
        }

        bool RedisProxy::operator!=(const RedisProxy &v) const
        {
            return _ptr != v._ptr;
        }

        bool RedisProxy::operator==(const RedisProxy &v) const
        {
            return _ptr == v._ptr;
        }

        bool RedisProxy::operator==(nullptr_t) const
        {
            return !_ptr;
        }

        bool RedisProxy::operator!=(nullptr_t) const
        {
            return (bool)_ptr;
        }
    } // namespace redis
} // namespace library
