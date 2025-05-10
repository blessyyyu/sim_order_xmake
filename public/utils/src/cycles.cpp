#include "cycles.h"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "utils/func_utils.h"
#include "utils/macro_utils.h"
#include "utils/time_utils.h"

namespace library {
    namespace utils {
        double Cycles::_cyclesPerSec = 0;

        static FUNC_CALL_ON_INIT(cycles, Cycles::Init);

        void Cycles::Init() {
            if (_cyclesPerSec != 0)
                return;

            // Compute the frequency of the fine-grained CPU timer: to do this,
            // take parallel time readings using both rdtsc and clock_gettime.
            // After 10ms have elapsed, take the ratio between these readings.

            struct timespec startTime, stopTime;
            uint64_t startCycles, stopCycles, micros;
            double oldCycles;

            // There is one tricky aspect, which is that we could get interrupted
            // between calling clock_gettime and reading the cycle counter, in which
            // case we won't have corresponding readings.  To handle this (unlikely)
            // case, compute the overall result repeatedly, and wait until we get
            // two successive calculations that are within 0.001% of each other (or
            // in other words, a drift of up to 10µs per second).
            oldCycles = 0;
            while (1) {
                if (clock_gettime(CLOCK_MONOTONIC, &startTime) != 0) {
                    EXIT_ON_ERROR("Cycles::init couldn't read clock: %s", strerror(errno));
                }
                startCycles = Time::Rdtsc();
                while (1) {
                    if (clock_gettime(CLOCK_MONOTONIC, &stopTime) != 0) {
                        EXIT_ON_ERROR("Cycles::init couldn't read clock: %s", strerror(errno));
                    }
                    stopCycles = Time::Rdtsc();
                    micros = (stopTime.tv_nsec - startTime.tv_nsec) / 1000 +
                             (stopTime.tv_sec - startTime.tv_sec) * 1000000;
                    if (micros > 10000) {
                        _cyclesPerSec = static_cast<double>(stopCycles - startCycles);
                        _cyclesPerSec = 1000000.0 * _cyclesPerSec /
                                        static_cast<double>(micros);
                        break;
                    }
                }
                double delta = _cyclesPerSec / 100000.0;
                if ((oldCycles > (_cyclesPerSec - delta)) &&
                    (oldCycles < (_cyclesPerSec + delta))) {
                    goto exit;
                }
                oldCycles = _cyclesPerSec;
            }

        exit:;
        }
    }  // namespace utils
}  // namespace library
