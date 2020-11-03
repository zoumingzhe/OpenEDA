##
# @file   main_flow.py
# @author Fei
# @date   Sep 2020
#

import math 
import torch
from torch import nn
from torch.autograd import Function

#import dreamplace.ops.lp_dp.lpdp_flow.lpdp_flow_cpp as lpdp_flow_cpp
import pdb
#         init_pos.view(init_pos.numel()), 
class LpdpFlowFunction(Function):
    """ legalization and detail placement flow 
    """
    @staticmethod
    def forward(
          num_bins_x, 
          num_bins_y, 
          flow_step,
          save_db,
          gpu
          ):
        #lpdp_flow_cpp.forward(
        #num_bins_x, 
        #num_bins_y, 
        #flow_step,
        #save_db,
        #gpu
        #)
        return

class LpdpFlow(object):
    """ main flow interface 
    """
    def __init__(self, num_bins_x, num_bins_y, flow_step, save_db, gpu):
        super(LpdpFlow, self).__init__()
        self.num_bins_x = num_bins_x
        self.num_bins_y = num_bins_y
        self.flow_step = flow_step
        self.save_db = save_db
        self.gpu = gpu
    def __call__(self): 
        """ 
        @param init_pos the reference position for displacement minization
        @param pos current roughly legal position
        """
        LpdpFlowFunction.forward(
        num_bins_x=self.num_bins_x, 
        num_bins_y=self.num_bins_y,
        flow_step=self.flow_step,
        save_db=self.save_db,
        gpu=self.gpu
        )
        return
