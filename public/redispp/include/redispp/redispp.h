/**
 * @file redispp.h
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
#pragma once

#include <memory>
#include <queue>

#include "redispp/redispp_export.h"
#include "sw/redis++/redis++.h"
#include "utils/singleton.h"

namespace library
{
    namespace redis
    {
        constexpr int MAX_DB_SIZE = 16; // Redis最大数据库实例数

        // redis哨兵配置
        struct SentinelConfig
        {
            std::string host; // 主机地址
            int port;         // 主机端口
        };

        // redis配置
        struct RedisConfig
        {
            std::string sentinelPasswd; // 哨兵密码
            std::string redisPasswd;    // Redis密码
            std::string masterName;     // 主库名称
            bool master;                // 是否连接主库（只有主库才有写权限）
            int db;                     // 默认的数据库实例id
            int connectTimeout;         // 连接超时时间（毫秒）
            int socketTimeout;          // 请求超时时间（毫秒）
            int poolSize;               // 连接池大小
        };

        using SentinelConfigPtr = std::shared_ptr<SentinelConfig>;
        using SentinelConfigArray = std::vector<SentinelConfigPtr>;
        using RedisConfigPtr = std::shared_ptr<RedisConfig>;

        using RedisPtr = std::shared_ptr<sw::redis::Redis>;
        using SentinelPtr = std::shared_ptr<sw::redis::Sentinel>;

        struct RedisPool;
        using RedisPoolPtr = std::shared_ptr<RedisPool>;
        using RedisPoolArray = std::vector<RedisPoolPtr>;
        class REDISPP_EXPORT Redispp : public library::utils::Singleton<Redispp>
        {
        public:
            /**
             * @brief 初始化连接池
             * @param sentinelConfigs 哨兵配置
             * @param redisConfig redis连接配置
             */
            void Init(SentinelConfigArray sentinelConfigs, RedisConfigPtr redisConfig);

            /**
             * @brief 获取一个有效的redis连接
             * @param db 数据库实例id，如果为-1则读取配置文件中的db
             * @return RedisPtr redis连接实例
             */
            RedisPtr GetRedis(int db = -1);

            /**
             * @brief 归还一个redis连接
             * @param redis 通过GetRedis获取的redis连接
             * @param isvalid 是否有效，如果调用的过程中出现异常则返还的时候设置为false，如果正常则为true
             * @param db 数据库实例id，isvalid为false时，会以此重新创建新的连接
             * @return true 成功
             * @return false 失败
             */
            bool GiveBack(RedisPtr redis, bool isvalid = true, int db = -1);

        private:
            SentinelConfigArray _sentinelConfigs; // 哨兵配置
            RedisConfigPtr _redisConfig;          // redis连接配置
            SentinelPtr _sentinel;                // 哨兵对象
            RedisPoolArray _redisConnPool;        // redis连接池列表
        };

        class RedisProxy
        {
        public:
            /**
             * @brief 构造函数
             * @param db redis数据库实例序号
             */
            RedisProxy(int db = -1);

            /**
             * @brief 析构函数
             */
            ~RedisProxy();

            /**
             * @brief 将状态置为无效，redis对象判断为空或者redis操作过程中抛出异常时使用
             */
            void SetInvalid();

            /**
             * @brief 重载->操作符，实现对RedisPtr的调用
             * @return RedisPtr redis连接对象
             */
            RedisPtr operator->() const;

            /**
             * @brief 重载void*返回原始指针，用于判空
             * @return void* 返回ptr_的原始指针
             */
            operator void *() const;

            /**
             * @brief 重载!=
             * @param v 比较对象
             * @return true 不相等
             * @return false 相等
             */
            bool operator!=(const RedisProxy &v) const;

            /**
             * @brief 重载==
             * @param v 比较对象
             * @return true 相等
             * @return false 不相等
             */
            bool operator==(const RedisProxy &v) const;

            /**
             * @brief 重载==，用于判空
             * @return true 为空
             * @return false 不为空
             */
            bool operator==(nullptr_t) const;

            /**
             * @brief 重载!=，判空
             * @return true 不为空
             * @return false 为空
             */
            bool operator!=(nullptr_t) const;

        private:
            bool _isValid; // 是否有效，用于归还连接时，连接池决定是否重连
            int _db;       // 数据库实例号
            RedisPtr _ptr; // 当前的Reids连接对象
        };
    } // namespace redis
} // namespace library
