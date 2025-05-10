/**
 * @file exception_utils.h
 * @brief 异常类
 * @author
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 * @par 修改日志:
 * <table>
 * <tr> <th>日期</th>       <th>作者</th> <th>修改说明</th> </tr>
 * <tr> <td>2022-06-03</td> <td></td> <td>初始创建</td> </tr>
 * </table>
 */
#pragma once

#include <stdexcept>

#include "utils/utils_export.h"

namespace library
{
    namespace utils
    {
        class UTILS_EXPORT Exception : public std::runtime_error
        {
        public:
            Exception(const std::string &msg)
                : std::runtime_error(msg)
            {
            }
        };
    } // namespace utils
} // namespace library
