##
# @file   GlobalPlaceRun.py
# @author Chenzhong Luo
# @date   Oct 2020
# @brief  python function to run global placement. 
#

import time 
import os
import sys
from datetime import datetime
import logging
import torch
import matplotlib 
matplotlib.use('Agg')
import numpy as np 
root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if root_dir not in sys.path:
    sys.path.append(root_dir)
import configure
import Params
import PlaceDB
import GlobalPlace

def global_placer_run(json_file, db_ptr):
    """
    @brief Top function to run global placement.
    @json json_file for user settings.
    """

    logging.root.name = 'DREAMPlace'
    logging.basicConfig(level=logging.INFO, format='[%(levelname)-7s] %(name)s - %(message)s', stream=sys.stdout)
    params = Params.Params()
    params.load(json_file)
    assert (not params.gpu) or configure.compile_configurations["CUDA_FOUND"] == 'TRUE', \
            "CANNOT enable GPU without CUDA compiled"

    np.random.seed(params.random_seed)

    # read database 
    tt = time.time()
    logging.info('[start reading database at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))

    placedb = PlaceDB.PlaceDB()
    placedb(params, db_ptr)  #db_ptr for open_edi flow

    logging.info("[end reading database at:%s, takes %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt))

    # solve placement 
    tt = time.time()
    logging.info('[start global placement initialization at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))

    placer = GlobalPlace.GlobalPlace(params, placedb)

    logging.info("[end global placement initialization at: %s, elapsed time %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt))

    tt = time.time()
    logging.info('[start global placement at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))

    placer(params, placedb)

    logging.info("[end global placement at: %s, elapsed time %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt))

    # update database
    tt = time.time()
    logging.info('[start update database at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))

    cur_pos = placer.pos[0].data.clone().cpu().numpy()
    placedb.apply(params, cur_pos[0:placedb.num_movable_nodes], cur_pos[placedb.num_nodes:placedb.num_nodes+placedb.num_movable_nodes], db_ptr)  #db_ptr for open_edi flow

    logging.info("[end update database at: %s, elapsed time %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt))

    return

