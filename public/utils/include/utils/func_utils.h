/**
 * @file func_utils.h
 * @brief 函数工具类
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

#include <functional>

namespace library
{
    namespace utils
    {
        class FuncCallOnInit
        {
        public:
            FuncCallOnInit(std::function<void(void)> func)
            {
                func();
            }
        };

        class FuncCallOnExit
        {
            std::function<void(void)> _func;

        public:
            FuncCallOnExit(std::function<void(void)> func)
                : _func(func)
            {
            }

            ~FuncCallOnExit()
            {
                _func();
            }
        };
    } // namespace utils
} // namespace library

#define FUNC_CALL_ON_INIT(id, func) library::utils::FuncCallOnInit _on_init_##id(func)
#define FUNC_CALL_ON_EXIT(id, func) library::utils::FuncCallOnExit _on_exit_##id(func)
