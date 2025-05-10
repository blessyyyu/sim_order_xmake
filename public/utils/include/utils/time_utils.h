/**
 * @file time_utils.h
 * @brief 精确到微秒的时间类
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

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>

#include "utils/os_utils.h"
#include "utils/utils_export.h"

#if _WIN32
#include <intrin.h>
#endif // _WIN32

namespace library
{
    namespace utils
    {
        class UTILS_EXPORT Time
        {
        public:
            Time();
            Time(const std::tm &tm, int millisecond = 0, int microsecond = 0);
            Time(int64_t microsecondsEpoch);
            Time(int year, int month, int day, int hour, int minute, int second, int millisecond = 0, int microsecond = 0);

            // 获取各时间段
            int GetYear() const { return _tm.tm_year + 1900; }
            int GetMonth() const { return _tm.tm_mon + 1; }
            int GetDay() const { return _tm.tm_mday; }
            int GetHour() const { return _tm.tm_hour; }
            int GetMinute() const { return _tm.tm_min; }
            int GetSecond() const { return _tm.tm_sec; }
            int GetMillisecond() const { return _millisecond; }
            int GetMicrosecond() const { return _microsecond; }
            /**
             * @brief 获取从1970年1月1日00:00到当前的微秒数
             * @return int64_t 返回值说明
             */
            int64_t GetMicrosecondsEpoch() const { return _microsecondsEpoch; }
            int64_t GetTimeT() const { return _microsecondsEpoch / 1000000; }

            // 将日期/时间转换成Int
            int GetDateInt() const { return GetYear() * 10000 + GetMonth() * 100 + GetDay(); }
            int GetTimeInt() const { return GetHour() * 10000 + GetMinute() * 100 + GetSecond(); }

            const std::string ToString() const
            {
                char buf[32];
                sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d.%03d%03d", GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond(), GetMillisecond(), GetMicrosecond());
                return buf;
            }

            const std::string ToString(const char *format) const
            {
                char buf[32];
                std::strftime(buf, sizeof(buf) - 1, format, &_tm);
                return buf;
            }

            // 操作符重载
            Time &operator=(const std::tm &tm);
            int64_t operator-(const Time &time) const { return _microsecondsEpoch - time._microsecondsEpoch; }
            Time operator-(const int64_t &span) const;
            Time operator+(const int64_t &span) const;
            const Time &operator-=(const int64_t &span);
            const Time &operator+=(const int64_t &span);
            bool operator==(const Time &time) const { return _microsecondsEpoch == time._microsecondsEpoch; }
            bool operator!=(const Time &time) const { return _microsecondsEpoch != time._microsecondsEpoch; }
            bool operator<(const Time &time) const { return _microsecondsEpoch < time._microsecondsEpoch; }
            bool operator<=(const Time &time) const { return _microsecondsEpoch <= time._microsecondsEpoch; }
            bool operator>(const Time &time) const { return _microsecondsEpoch > time._microsecondsEpoch; }
            bool operator>=(const Time &time) const { return _microsecondsEpoch >= time._microsecondsEpoch; }

            // 获取任意精度的时间（性能一般）
            template <class T>
            inline static int64_t Now()
            {
                auto now = std::chrono::system_clock::now();
                auto epoch = now.time_since_epoch();
                return std::chrono::duration_cast<T>(epoch).count();
            }

            // 获取纳秒时间戳，采用C库系统调用，性能较高
            static int64_t NowNano();

            // 获取CPU时钟数，更高精度的时间统计，两次调用的差值，除以CPU主频
#if _WIN32
            static uint64_t Rdtsc()
            {
                return __rdtsc();
            }
#else
            static uint64_t Rdtsc()
            {
                uint32_t low, high;
                __asm__ volatile("rdtsc"
                                 : "=a"(low), "=d"(high));
                return (uint64_t)high << 32 | low;
            }
#endif // _WIN32

            /**
             * @brief 获取每秒的CPU时钟数
             * @return double 返回值说明
             */
            static double GetCyclesPerSec();

        private:
            std::tm _tm;                // 时间结构
            int64_t _microsecondsEpoch; // 从1970年1月1日00:00到当前的微秒数
            int _millisecond;           // 毫秒
            int _microsecond;           // 微秒
        };

        class UTILS_EXPORT TimeElapsed
        {
        public:
            TimeElapsed(const std::string prefix = "TimeElapsed", uint32_t times = 1)
                : _times(times), _start(Time::Rdtsc()), _prefix(prefix)
            {
                library::utils::os::Serialize();
            }

            TimeElapsed(uint32_t times)
                : _times(times), _start(Time::Rdtsc()), _prefix("TimeElapsed")
            {
            }

            ~TimeElapsed()
            {
                library::utils::os::Serialize();
                auto timeSpan = (Time::Rdtsc() - _start) / Time::GetCyclesPerSec();
                printf("%s{times: %lu, total: %0.2lf seconds, avg: %0.2lf ns}\n", _prefix.c_str(), _times, timeSpan, (timeSpan / _times) * 1e9);
            }

        private:
            int64_t _start;      // 起始时间
            uint32_t _times = 1; // 次数，用于计算平均数
            std::string _prefix; // 自定义前缀
        };
    } // namespace utils
} // namespace library
