/*************************************************************************
    > File Name: 
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include "lp_dp/legality_check/src/legality_check.h"
#include "utility/src/Msg.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

DREAMPLACE_BEGIN_NAMESPACE

void LegalityCheck::run()
{
  dreamplacePrint(kINFO, "Starting Legality Check\n");
  // exit if no DB
  if (!isCommonDBReady()) {
    dreamplacePrint(kINFO, "Error out\n");
    return;
  }
}

DREAMPLACE_END_NAMESPACE
