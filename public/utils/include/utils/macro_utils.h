/**
 * @file assert_utils.h
 * @brief 宏定义工具
 * @author
 * @date 2022-08-06
 *
 * @copyright Copyright (c) 2022
 *
 * @par 修改日志:
 * <table>
 * <tr> <th>日期</th>       <th>作者</th> <th>修改说明</th> </tr>
 * <tr> <td>2022-08-06</td> <td></td> <td>初始创建</td> </tr>
 * </table>
 */
#pragma once

#define EXIT_ON_ERROR(format_, ...)                     \
    do                                                  \
    {                                                   \
        fprintf(stderr, format_, ##__VA_ARGS__);        \
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__); \
        abort();                                        \
    } while (0)
