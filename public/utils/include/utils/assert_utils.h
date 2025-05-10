/**
 * @file assert_utils.h
 * @brief 断言宏定义
 * @author
 * @date 2022-11-01
 *
 * @copyright Copyright (c) 2022
 *
 * @par 修改日志:
 * <table>
 * <tr> <th>日期</th>       <th>作者</th> <th>修改说明</th> </tr>
 * <tr> <td>2022-11-01</td> <td></td> <td>初始创建</td> </tr>
 * </table>
 */
#pragma once

#define ASSERT_BREAK(cond, msg) \
    if (!(cond))                \
    {                           \
        LOGOUT(ERROR, msg);     \
        break;                  \
    }
#define ASSERT_CONTINUE(cond, msg) \
    if (!(cond))                   \
    {                              \
        LOGOUT(ERROR, msg);        \
        continue;                  \
    }
#define ASSERT_EXIT(cond, msg) \
    if (!(cond))               \
    {                          \
        LOGOUT(ERROR, msg);    \
        exit(-1);              \
    }
