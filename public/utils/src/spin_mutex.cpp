/**
 * @file spin_mutex.cpp
 * @brief 自旋锁实现
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
#include "utils/spin_mutex.h"

namespace library
{
    namespace utils
    {
        void SpinMutex::lock()
        {
            while (flag.test_and_set(std::memory_order_acquire))
                ;
        }

        bool SpinMutex::try_lock()
        {
            return !flag.test_and_set(std::memory_order_acquire);
        }

        void SpinMutex::unlock()
        {
            flag.clear(std::memory_order_release);
        }
    } // namespace utils
} // namespace library
