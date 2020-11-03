/*************************************************************************
    > File Name: lpdp_place.cpp
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "flow/src/lpdp_place.h"
#include "utility/src/Msg.h"

DREAMPLACE_BEGIN_NAMESPACE

// class LegalDetailedDB member functions
void
LpdpPlace::run()
{
  dreamplacePrint(kINFO, "Starting LP_DP run \n");

  // exit if db is not ready
  if (!isDBReady()) {
    return;
  }

  /// legalization place

  // macro legalize
  if (getFlowSteps() & kMacroLegalize) {
    MacroLegal macroLegalize;
    macroLegalize.run();
  }

  // greedy legalize
  if (getFlowSteps() & kGreedLegalize) {
    GreedyLegal greedyLegalize;
    greedyLegalize.run();
  }

  // legality check
  if (getFlowSteps() & kLegalityCheck) {
    LegalityCheck legalityCheck;
    legalityCheck.run();
  }

  // abacus legalize
  if (getFlowSteps() & kAbacusLegalize) {
    AbacusLegal abacusLegalize;
    abacusLegalize.run();
  }

  /// detail place
  // k reorder
  if (getFlowSteps() & kKReorder1) {
    KReorder kReorder;
    kReorder.run();
  }

  // independent set matching
  if (getFlowSteps() & kIndependentSM) {
    IndependentSetMatching independentSM;
    independentSM.run();
  }

  // global swap
  if (getFlowSteps() & kGlobalSwap) {
    GlobalSwap globalSwap;
    globalSwap.run();
  }

  // k reoder again
  if (getFlowSteps() & kKReorder2) {
    KReorder kReorder;
    kReorder.run();
  }
}

DREAMPLACE_END_NAMESPACE
