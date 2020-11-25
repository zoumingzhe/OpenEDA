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

void MacroLegal::run()
{
  dreamplacePrint(kINFO, "Starting Macro Legalization\n");
  // exit if no DB
  if (!isCommonDBReady()) {
    dreamplacePrint(kINFO, "Error out\n");
    return;
  }
}

DREAMPLACE_END_NAMESPACE
