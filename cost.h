#ifndef __COST_TIME_H_
#define __COST_TIME_H_

#include <time.h>
#include <inttypes.h>
#include <sys/time.h>

namespace common { namespace server {

/*! \class CostTime
    \brief A wrapper to get cost time
 *
 *  CostTime is implemented with x86 Instruction
 *  It's more effective than the below
 *  CostTimeGtd and CostTimeCg
 */
class CostTime
{
public:

  /**
   * CostTime Constructor
   * @param cost  return the time consume between the constructor and destructor, in millisecond
   */
  CostTime(uint32_t& cost)
    : start_(0)
    , stop_(0)
    , cost_(cost)
  {
    start_ = GetCycles();
  }

  ~CostTime()
  {
    stop_ = GetCycles();
    cost_ = (stop_ - start_) / cpu_freq_;
  }

  /**
   * Init the CPU freq variable
   * make sure to call GetCPUFreq
   * before using this class
   */
  static bool GetCPUFreq();

  /**
   * call rdtsc to get Time-Stamp Counter
   * ref: https://www.ccsl.carleton.ca/~jamuir/rdtscpm1.pdf
   * @return CPU cycle
   */
  static unsigned long long GetCycles();

private:

  static double cpu_freq_;

  unsigned long start_;
  unsigned long stop_;

  uint32_t& cost_;

}; // CostTime


/*! \class CostTimeCg
    \brief A wrapper to get cost time commonich is implemented with clock_gettime
 *
 *  similar to CostTime, but less effective
 */
class CostTimeCg
{
public:

  /**
   * CostTime Constructor
   * @param cost  return the time consume between the constructor and destructor, in millisecond
   */
  CostTimeCg(uint32_t& cost)
    : cost_(cost)
  {
    // NOTE: CLOCK_MONOTONIC is more effective than CLOCK_PROCESS_CPUTIME_ID and CLOCK_THREAD_CPUTIME_ID
    // CLOCK_REALTIME is almost as effective as CLOCK_MONOTONIC, but CLOCK_REALTIME represents the
    // machine's best-guess as to the current wall-clock, it can jump forwards and backwards as the system
    // time-of-day clock is changed, including by NTP.
    clock_gettime(CLOCK_MONOTONIC, &start_);
  }

  ~CostTimeCg()
  {
    clock_gettime(CLOCK_MONOTONIC, &stop_);
    if (start_.tv_nsec > stop_.tv_nsec)
    {
      cost_ = (stop_.tv_sec-start_.tv_sec-1)*1000+(stop_.tv_nsec+1000000000-start_.tv_nsec)/1000000;
    }
    else
    {
      cost_ = (stop_.tv_sec-start_.tv_sec)*1000+(stop_.tv_nsec-start_.tv_nsec)/1000000;
    }
  }

private:

  uint32_t& cost_;
  timespec  start_;
  timespec  stop_;

}; // CostTimeCg

/*! \class CostTimeGtd
    \brief A wrapper to get cost time commonich is implemented with gettimeofday
 *
 *  similar to CostTime, but less effective, the system time can affect the result
 *  In according to my profile with 1kw call, CostTimeCg is 10ms fast than CostTimeGtd
 *  CostTimeCg takes 235ms in my Intel I5 PC, ubuntu 13.04
 */
class CostTimeGtd
{
public:

  /**
   * CostTime Constructor
   * @param cost  return the time consume between the constructor and destructor, in millisecond
   */
  CostTimeGtd(uint32_t& cost)
    : cost_(cost)
  {
    gettimeofday(&start_,NULL);
  }

  ~CostTimeGtd()
  {
    gettimeofday(&stop_, NULL);
    if (start_.tv_usec > stop_.tv_usec)
    {
      cost_ = (stop_.tv_sec-start_.tv_sec-1)*1000+(stop_.tv_usec+1000000-start_.tv_usec)/1000;
    }
    else
    {
      cost_ = (stop_.tv_sec-start_.tv_sec)*1000+(stop_.tv_usec-start_.tv_usec)/1000;
    }
  }

private:

  uint32_t& cost_;
  timeval   start_;
  timeval   stop_;

}; // CostTimeGtd

}} // ::common::server

#endif // __COST_TIME_H_

