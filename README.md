# time_cost
Cost relative basic classes are C++ wrappers for time consumption in performance monitoring.

the main.cpp provides simple examples of the three wrappers.

According to main.cpp comparision:
- CostTime implemented with [rdtsc](https://www.strchr.com/performance_measurements_with_rdtsc), the x86 Instruction.It's much fast than the below two. With 1KW call, it takes about 80+ms in my PC.
- CostTimeCg implemented with [clock_gettime](https://linux.die.net/man/3/clock_gettime) and CLOCK_MONOTONIC parameter. With 1KW call, it takes about 230ms in my PC.
- CostTimeGtd implemented with [gettimeofday](https://linux.die.net/man/2/gettimeofday), it can be affected by the system time. With 1KW call, it takes about 240ms in my PC.

