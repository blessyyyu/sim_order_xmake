/**
 * @file singleton.h
 * @brief 单例模板类
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

namespace library
{
    namespace utils
    {
        template <typename T>
        class Singleton
        {
        public:
            inline static T *Instance()
            {
                static T t;
                return &t;
            }

        protected:
            Singleton() {}
            ~Singleton() {}

        private:
            Singleton(const Singleton &rhs);
            Singleton &operator=(const Singleton &rhs);
        };
    } // namespace utils
} // namespace library
