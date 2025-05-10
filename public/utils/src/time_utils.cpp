/**
 * @file time_utils.cpp
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
#include "utils/time_utils.h"

#include <chrono>

#include "cycles.h"

namespace library
{
    namespace utils
    {
        Time::Time()
        {
            struct timespec timestamp;
            clock_gettime(CLOCK_REALTIME, &timestamp);
            _microsecondsEpoch = timestamp.tv_sec * 1000000LL + timestamp.tv_nsec / 1000;

#if defined _MSC_VER && _MSC_VER >= 1400
            localtime_s(&_tm, &timestamp.tv_sec);
#elif defined __GNUC__
            localtime_r(&timestamp.tv_sec, &_tm);
#else
            _tm = *std::localtime(&timestamp.tv_sec);
#endif
            _millisecond = _microsecondsEpoch / 1000 % 1000;
            _microsecond = _microsecondsEpoch % 1000;
        }

        Time::Time(int64_t microsecondsEpoch)
            : _microsecondsEpoch(microsecondsEpoch)
        {
            int64_t secondsEpoch = _microsecondsEpoch / 1000000;

#if defined _MSC_VER && _MSC_VER >= 1400
            localtime_s(&_tm, &secondsEpoch);
#elif defined __GNUC__
            localtime_r(&secondsEpoch, &_tm);
#else
            _tm = *std::localtime(&secondsEpoch);
#endif
            _millisecond = _microsecondsEpoch / 1000 % 1000;
            _microsecond = _microsecondsEpoch % 1000;
        }

        Time::Time(const std::tm &tm, int millisecond, int microsecond)
            : _tm(tm), _millisecond(millisecond), _microsecond(microsecond)
        {
            _microsecondsEpoch = (int64_t)std::mktime(&_tm) * 1000000 + _millisecond * 1000 + _microsecond;
        }

        Time::Time(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
            : _millisecond(millisecond), _microsecond(microsecond)
        {
            _tm.tm_year = year - 1900;
            _tm.tm_mon = month - 1;
            _tm.tm_mday = day;
            _tm.tm_hour = hour;
            _tm.tm_min = minute;
            _tm.tm_sec = second;
            _microsecondsEpoch = (int64_t)std::mktime(&_tm) * 1000000 + _millisecond * 1000 + _microsecond;
        }

        Time &Time::operator=(const std::tm &tm)
        {
            _tm = tm;
            _millisecond = 0;
            _microsecond = 0;
            _microsecondsEpoch = (int64_t)std::mktime(&_tm) * 1000000 + _millisecond * 1000 + _microsecond;
            return *this;
        }

        Time Time::operator-(const int64_t &span) const
        {
            return Time(_microsecondsEpoch - span);
        }

        Time Time::operator+(const int64_t &span) const
        {
            return Time(_microsecondsEpoch + span);
        }

        const Time &Time::operator-=(const int64_t &span)
        {
            _microsecondsEpoch -= span;
            int64_t secondsEpoch = _microsecondsEpoch / 1000000;

#if defined _MSC_VER && _MSC_VER >= 1400
            localtime_s(&_tm, &secondsEpoch);
#elif defined __GNUC__
            localtime_r(&secondsEpoch, &_tm);
#else
            _tm = *std::localtime(&secondsEpoch);
#endif
            _millisecond = _microsecondsEpoch / 1000 % 1000;
            _microsecond = _microsecondsEpoch % 1000;
            return *this;
        }

        const Time &Time::operator+=(const int64_t &span)
        {
            _microsecondsEpoch += span;
            int64_t secondsEpoch = _microsecondsEpoch / 1000000;

#if defined _MSC_VER && _MSC_VER >= 1400
            localtime_s(&_tm, &secondsEpoch);
#elif defined __GNUC__
            localtime_r(&secondsEpoch, &_tm);
#else
            _tm = *std::localtime(&secondsEpoch);
#endif
            _millisecond = _microsecondsEpoch / 1000 % 1000;
            _microsecond = _microsecondsEpoch % 1000;
            return *this;
        }

        int64_t Time::NowNano()
        {
            // C库性能更高，平均24ns
            struct timespec timestamp;
            clock_gettime(CLOCK_REALTIME, &timestamp);
            return timestamp.tv_sec * 1000000000LL + timestamp.tv_nsec;

            // C++11性能差一些，平均36ns
            // auto now = std::chrono::system_clock::now();
            // auto epoch = now.time_since_epoch();
            // return std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
        }

        double Time::GetCyclesPerSec()
        {
            return Cycles::GetCyclesPerSec();
        }
    } // namespace utils
} // namespace library
