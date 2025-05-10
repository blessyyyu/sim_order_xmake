/**
 * @file cycles.h
 * @brief 计算每秒CPU时钟数
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

namespace library
{
    namespace utils
    {
        class Cycles
        {
        public:
            /**
             * @brief 初始化每秒的CPU时钟数
             */
            static void Init();

            /**
             * @brief 获取每秒的CPU时钟数
             * @return double 返回值说明
             */
            static double GetCyclesPerSec() { return _cyclesPerSec; }

        private:
            static double _cyclesPerSec; // 每秒的CPU时钟数
        };
    } // namespace utils
} // namespace library
