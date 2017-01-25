#include <iostream>
#include <sys/time.h>
#include <inttypes.h>
#include <time.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "cost.h"

using namespace std;
using namespace ::common::server;

int main(int argc, char** argv)
{

  CostTime::GetCPUFreq();
  uint32_t cost = 0;
  {
    CostTimeGtd cg(cost);
    //CostTimeCg cg(cost);
    //CostTime t(cost);

    for(int i = 0; i < 10000000; ++i)
    {
      //struct timeval loop;
      //gettimeofday(&loop,NULL);
      CostTime::GetCycles();
    }
  }

  cout << cost << " ms" << endl;

  return 0;
}

