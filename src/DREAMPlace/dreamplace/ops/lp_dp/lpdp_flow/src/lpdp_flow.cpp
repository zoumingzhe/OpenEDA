/*************************************************************************
    > File Name: lpdp_flow.cpp 
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "utility/src/torch.h"
#include "flow/src/lpdp_place.h"
#include "utility/src/Msg.h"

DREAMPLACE_BEGIN_NAMESPACE
void 
lpdpFlowLauncher(Para& para)
{
   // luncher function for main flow
   LpdpPlace lpdp(para);
   lpdp.run();
}

void 
lpdp_flow_forward(
  int num_bins_x, 
  int num_bins_y,
  int flow_steps, 
  bool save_db,
  bool gpu)
{
   // start time
   hr_clock_rep timer_start = get_globaltime(); 
   Para para(num_bins_x, num_bins_y, flow_steps, save_db, gpu);
   lpdpFlowLauncher(para);
   // stop time
   hr_clock_rep timer_stop = get_globaltime(); 
   dreamplacePrint(kINFO, "Legalization and detailed placement flow takes %g ms\n", 
                   (timer_stop-timer_start)*get_timer_period());

}

DREAMPLACE_END_NAMESPACE

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
  m.def("forward", &DREAMPLACE_NAMESPACE::lpdp_flow_forward, "lpdp forward");
}
