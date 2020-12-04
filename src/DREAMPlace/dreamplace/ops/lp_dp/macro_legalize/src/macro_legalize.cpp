/*************************************************************************
    > File Name: 
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include "lp_dp/macro_legalize/src/macro_legalize.h"
#include "utility/src/Msg.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

DREAMPLACE_BEGIN_NAMESPACE

inline void
MacroLegal::initLegalizationDB(LegalizationDB<int>& db)
{
  db_->initLegalizationDB(db);
}

void
MacroLegal::run()
{
  hr_clock_rep timer_start = get_globaltime(); 
  dreamplacePrint(kINFO, "Starting Macro Legalization\n");
  // exit if no DB
  if (!isCommonDBReady()) {
    dreamplacePrint(kINFO, "Error out\n");
    return;
  }
  LegalizationDB<int> db;
  initLegalizationDB(db);
  macroLegalizationRun(db);
  hr_clock_rep timer_stop = get_globaltime(); 
  dreamplacePrint(kINFO, "Macro legalization takes %g ms\n", (timer_stop-timer_start)*get_timer_period());
}

DREAMPLACE_END_NAMESPACE
