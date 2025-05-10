#pragma once

#include <inttypes.h>

struct Order {
    int32_t branch_no;       // 操作分支机构
    char client_id[32];      // 客户编号
    char fund_account[32];   // 资产账户
    char password[32];       // 密码
    char order_id[16];       // 客户订单编号
    int32_t batch_no;        // 委托批号
    char stock_account[32];  // 证券账号
    char exchange_type;      // 交易所类别
    char stock_code[32];     // 证券代码
    char op_entrust_way;     // 委托方式
    char entrust_prop;       // 委托属性
    char entrust_bs;         // 买卖方向
    int entrust_amount;      // 委托数量
    double entrust_price;    // 委托价格
    double entrust_money;    // 委托金额
    char registe_sure_flag;  // 是否已签署确认书

    // 委托生成数据
    int32_t init_date = 0;     // 交易日期
    int32_t entrust_time = 0;  // 委托时间
    int32_t entrust_no = 0;    // 委托编号
    int32_t report_no = 0;     // 申请编号
    char seat_no[6] = {0};     // 席位编号
    double deal_price = 0;     // 成交价格
    double deal_amount = 0;    // 成交数量
    double cancel_amount = 0;  // 撤销数量
    char entrust_status = 0;   // 委托状态
    double fees = 0;           // 总费用
    double freeze_money = 0;   // 冻结资金
    int32_t update_time = 0;   // 委托更新时间
    char remark[256] = {};     // 提示说明
};

struct AccountFund {
    double fund_avl = 0;      // 可用资金
    double fund_trd_frz = 0;  // 交易冻结
};

const int ACCOUNT_MAX = 5000000;
