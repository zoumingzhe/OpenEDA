##
# @file   GlobalPlace.py
# @author Chenzhong Luo
# @date   Oct 2020
# @brief  Global placement without post_processing 
#

import EvalMetrics
import NonLinearPlace

class GlobalPlace (NonLinearPlace.NonLinearPlace):
    """
    @brief Global placement engine. 
    It inherits from NonLinearPlace, just gets rid of post processing. 
    """
    def __call__(self, params, placedb):
        """
        @brief Top API to solve global placement. 
        @param params parameters 
        @param placedb placement database 
        """
        all_metrics = []
        total_iteration = 0
        if params.global_place_flag:
            total_iteration = self.global_place_electric_potential(params, placedb,all_metrics)
        return all_metrics

