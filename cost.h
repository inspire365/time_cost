#ifndef __COST_TIME_H_
#define __COST_TIME_H_

#include <time.h>
#include <inttypes.h>
#include <sys/time.h>

#define rdtsc(low,high) __asm__ \
 __volatile__("rdtsc" : "=a" (low), "=d" (high))

namespace wh { namespace server {

class CostTime
{
public:

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

  // make sure to call this function to init cpu_freq first
  static bool GetCPUFreq();

  static unsigned long long GetCycles();

private:

  static double cpu_freq_;

  unsigned long start_;
  unsigned long stop_;

  uint32_t& cost_;

}; // CostTime

class CostTimeCg
{
public:

  CostTimeCg(uint32_t& cost)
    : cost_(cost)
  {
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

}; // CostTimeGtd

class CostTimeGtd
{
public:

  CostTimeGtd(uint32_t& cost)
    : cost_(cost)
  {
    gettimeofday(&start_,NULL);
  }

  ~CostTimeGtd()
  {
    uint32_t cost_time = 0;
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

}} // ::wh::server

#endif // __COST_TIME_H_

