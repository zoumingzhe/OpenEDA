/*************************************************************************
    > File Name: 
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "lp_dp/greedy_legalize/src/greedy_legalize.h"

DREAMPLACE_BEGIN_NAMESPACE

inline void
GreedyLegal::initLegalizationDB(LegalizationDB<int>& db)
{
  db_->initLegalizationDB(db);
}

void
GreedyLegal::run()
{
  hr_clock_rep timer_start = get_globaltime(); 
  dreamplacePrint(kINFO, "Starting Greed Legalization\n");
  // exit if no DB
  if (!isCommonDBReady()) {
    dreamplacePrint(kINFO, "Exit Greed Legalization w/o DB\n");
    return;
  }
  LegalizationDB<int> db;
  initLegalizationDB(db);
  greedyLegalizationRun(db);
  hr_clock_rep timer_stop = get_globaltime(); 
  dreamplacePrint(kINFO, "Greedy legalization takes %g ms\n", (timer_stop-timer_start)*get_timer_period());
}

DREAMPLACE_END_NAMESPACE
