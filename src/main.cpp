/**
 * @file main.cpp
 * @brief redis队列测试，一个线程向list中rpush数据，一个线程从list中lpop数据
 */
#include <iostream>
#include <thread>

#include "common_def.h"
#include "fmt/format.h"
#include "redispp/redispp.h"
#include "utils/cmdline.h"
#include "utils/time_utils.h"
#include "xmf/xmf_json.h"

int main(int argc, char** argv) {
    library::xmf::XmfJson xmfJson;
    auto dataPtr = xmfJson.Read("config.json");
    if (!dataPtr) {
        std ::cout << "读取config.json失败，请检查文件是否存在:" << xmfJson.GetErrorMsg() << std::endl;
        return false;
    }

    try {
        // Redis配置
        auto redisCfgPtr = dataPtr->Item("redis");
        if (redisCfgPtr == nullptr) {
            std::cout << "缺少redis配置" << std::endl;
            return false;
        }

        auto sentinelsPtr = redisCfgPtr->Item("sentinels");
        if (sentinelsPtr == nullptr) {
            std::cout << "缺少redis->sentinels配置" << std::endl;
            return -1;
        }

        library::redis::SentinelConfigArray sentConfArr;  // redis哨兵配置

        for (auto itor = sentinelsPtr->GetChildIterator(); !itor->IsEof(); itor->MoveNext()) {
            auto sentinel = std::make_shared<library::redis::SentinelConfig>();
            sentinel->host = itor->GetValue()->Item("host")->ToString();
            sentinel->port = itor->GetValue()->Item("port")->ToInt();
            sentConfArr.push_back(sentinel);
        }

        // redis配置
        auto redisConfPtr = std::make_shared<library::redis::RedisConfig>();
        redisConfPtr->sentinelPasswd = redisCfgPtr->Item("sentinel_passwd")->ToString();
        redisConfPtr->redisPasswd = redisCfgPtr->Item("redis_passwd")->ToString();
        redisConfPtr->masterName = redisCfgPtr->Item("master_name")->ToString();
        redisConfPtr->master = redisCfgPtr->Item("master")->ToBool();
        redisConfPtr->db = redisCfgPtr->Item("db")->ToInt();
        redisConfPtr->connectTimeout = redisCfgPtr->Item("connect_timeout")->ToInt();
        redisConfPtr->socketTimeout = redisCfgPtr->Item("socket_timeout")->ToInt();
        redisConfPtr->poolSize = redisCfgPtr->Item("pool_size")->ToInt();

        // 初始化Reids配置
        library::redis::Redispp::Instance()->Init(sentConfArr, redisConfPtr);
    } catch (library::utils::Exception& e) {
        std::cout << "读取config.json失败: " << e.what() << std::endl;
        return false;
    }

    // 命令行处理
    cmdline::parser parser;
    parser.add<std::string>("type", 't', "订单角色: push-新增count个订单 pop-取出全部订单", false, "push", cmdline::oneof<std::string>("push", "pop"));
    parser.add<int64_t>("account_count", 'a', "账户数量，push时有效", false, 1);         // 默认1个账户
    parser.add<int64_t>("orde_count", 'n', "订单数量，push时有效", false, 10000);        // 默认1万笔订单
    parser.add<int64_t>("queue_count", 'c', "队列数量，push时有效", false, 1);           // 默认1个队列
    parser.add<std::string>("queue_name", 'q', "订单队列的名称", false, "order_queue");  // 订单队列名称 queue_name_i
    parser.parse_check(argc, argv);

    auto type = parser.get<std::string>("type");
    auto accountCount = parser.get<int64_t>("account_count");
    auto orderCount = parser.get<int64_t>("orde_count");
    auto queueCount = parser.get<int64_t>("queue_count");
    auto queueName = parser.get<std::string>("queue_name");

    int64_t clOrderId = library::utils::Time::NowNano();

    // 向队列中添加订单
    if (type == "push") {
        library::redis::RedisProxy redis;
        if (redis == nullptr) {
            std::cout << "Redis连接数不够" << std::endl;
            return -1;
        }

        int64_t clientId = 600000000001;
        int64_t fundAccount = 700000000001;

        Order order;
        order.branch_no = 30;
        strcpy(order.password, "abc123");
        order.batch_no = 0;
        strcpy(order.stock_account, "B880820006");
        order.exchange_type = '1';  // 1-上海
        strcpy(order.stock_code, "600028");
        order.op_entrust_way = '1';  // 限价委托
        order.entrust_prop = '0';
        order.entrust_bs = '1';
        order.entrust_amount = 300;
        order.entrust_price = 4.26;
        order.registe_sure_flag = '1';

        try {
            for (size_t i = 0; i < orderCount; i++) {
                strcpy(order.client_id, std::to_string(clientId + i % accountCount).c_str());
                strcpy(order.fund_account, std::to_string(fundAccount + i % accountCount).c_str());
                strcpy(order.order_id, std::to_string(redis->incr("order_no")).c_str());
                redis->rpush(fmt::format("{}_{}", queueName, (fundAccount + i % accountCount) % queueCount), sw::redis::StringView((const char*)&order, sizeof(order)));
            }

            std::cout << "批量创建" << orderCount << "笔订单成功" << std::endl;
        } catch (const sw::redis::Error& e) {
            // 出现异常，丢弃该连接
            redis.SetInvalid();
            std::cout << "Redis操作异常:" << e.what() << std::endl;
        }
    } else {
        library::redis::RedisProxy redis;
        if (redis == nullptr) {
            std::cout << "Redis连接数不够" << std::endl;
            return -1;
        }

        try {
            int sum = 0;
            for (size_t i = 0; i < queueCount; i++) {
                auto element = redis->lpop(fmt::format("{}_{}", queueName, i));
                while (element) {
                    ++sum;
                    auto order = (Order*)element->data();
                    std::cout << "Order[编号:" << order->order_id << ",账户:" << order->fund_account
                              << ",代码:" << order->exchange_type << "|" << order->stock_code
                              << ",买卖:" << order->entrust_bs << ",价格:" << order->entrust_price << ",数量:" << order->entrust_amount << std::endl;
                    element = redis->lpop(fmt::format("{}_{}", queueName, i));
                }
            }

            std::cout << "批量消费" << sum << "笔订单成功" << std::endl;
        } catch (const sw::redis::Error& e) {
            // 出现异常，丢弃该连接
            redis.SetInvalid();
            std::cout << "Redis操作异常:" << e.what() << std::endl;
        }
    }

    return 0;
}
