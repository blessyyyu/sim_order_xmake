/**
 * @file spin_mutex.h
 * @brief 自旋锁，保持与std::mutex一致
 * @author
 * @date 2022-06-26
 *
 * @copyright Copyright (c) 2022
 *
 * @par 修改日志:
 * <table>
 * <tr> <th>日期</th>       <th>作者</th> <th>修改说明</th> </tr>
 * <tr> <td>2022-06-26</td> <td></td> <td>初始创建</td> </tr>
 * </table>
 */
#pragma once

#include <atomic>

#include "utils/utils_export.h"

namespace library
{
    namespace utils
    {
        class UTILS_EXPORT SpinMutex
        {
        public:
            SpinMutex() = default;
            SpinMutex(const SpinMutex &) = delete;
            SpinMutex &operator=(const SpinMutex &) = delete;
            void lock();
            bool try_lock();
            void unlock();

        private:
            std::atomic_flag flag = ATOMIC_FLAG_INIT;
        };
    } // namespace utils
} // namespace library
