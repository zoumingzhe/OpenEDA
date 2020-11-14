/*************************************************************************
    > File Name: 
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include "lp_dp/greedy_legalize/src/greedy_legalize.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

DREAMPLACE_BEGIN_NAMESPACE

void GreedyLegal::run()
{
  dreamplacePrint(kINFO, "Starting Greed Legalization\n");
  // exit if no DB
  if (!isCommonDBReady()) {
    return;
  }
}

DREAMPLACE_END_NAMESPACE
