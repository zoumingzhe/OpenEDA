/*************************************************************************
    > File Name: 
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include "flow/src/main_place.h"
#include "utility/src/Msg.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

DREAMPLACE_BEGIN_NAMESPACE

// class LegalDetailedDB member functions
void
MainPlace::run()
{
  // exit if db is not ready
  if (!isDBReady()) {
    return;
  }

  /// global place

  /// lpdp place 
  LpdpPlace lpdp;
  lpdp.run();
}

DREAMPLACE_END_NAMESPACE
