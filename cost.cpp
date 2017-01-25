#include "cost.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace wh { namespace server {

#define rdtsc(low,high) __asm__ \
 __volatile__("rdtsc" : "=a" (low), "=d" (high))

double CostTime::cpu_freq_ = 0;

bool CostTime::GetCPUFreq()
{
  char buf[1024]={0};
  int ratio = 1;

  const char* cmd = NULL;

  FILE* fp = popen("cat /proc/cpuinfo | grep -m 1 \"model name\"", "r" );
  fgets(buf, 1024, fp);
  fclose( fp );

  if( strstr( buf, "AMD" ) ) 
  {
    cmd =  "cat /proc/cpuinfo | grep -m 1 \"cpu MHz\" | sed -e \'s/.*:[^0-9]//\'";
  }
  else
  {
    cmd = "cat /proc/cpuinfo | grep -m 1 \"model name\" | sed -e \"s/^.*@ //g\" -e \"s/GHz//g\"";
    ratio = 1000;
  }

  fp = popen( cmd, "r" );
  if( fp == NULL )
  {
    return false; 
  }
  else
  {
    fgets( buf, 1024, fp );
    cpu_freq_ = atof(buf) * ratio * 1000;
    fclose( fp );
  }
  return true;
}

unsigned long long CostTime::GetCycles()
{
  unsigned low, high;
  unsigned long long val;
  rdtsc(low,high);
  val = high;
  val = (val << 32) | low;
  return val;
}

}} // ::wh::server

