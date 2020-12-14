##
# @file   Placer.py
# @author Yibo Lin
# @date   Apr 2018
# @brief  Main file to run the entire placement flow. 
#
import torch
import matplotlib 
matplotlib.use('Agg')
import os
import sys 
import time 
import numpy as np 
import logging
# for consistency between python2 and python3
root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if root_dir not in sys.path:
	sys.path.append(root_dir)
import dreamplace.configure as configure 
import Params 
import PlaceDB
import NonLinearPlace 
import pdb 
from utility_mon import pthread_monitor, sysload_monitoring,terminator,dir_exists
from datetime import datetime
import psutil

def place(params):
    """
    @brief Top API to run the entire placement flow. 
    @param params parameters 
    """

    assert (not params.gpu) or configure.compile_configurations["CUDA_FOUND"] == 'TRUE', \
            "CANNOT enable GPU without CUDA compiled"

    np.random.seed(params.random_seed)
    # read database 
    tt = time.time()
    
    logging.info('[start reading database at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
    placedb = PlaceDB.PlaceDB()
    placedb(params)
    logging.info("[end reading database at:%s, takes %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt))
    

    # solve placement 
    tt = time.time()
    
    logging.info('[start non-linear placement initialization at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
    placer = NonLinearPlace.NonLinearPlace(params, placedb)
    logging.info("[end non-linear placement initialization at: %s, elapsed time %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt))
    
    tt = time.time()
    
    logging.info('[start non-linear placement at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
    metrics = placer(params, placedb)
    

    # write placement solution 
    if params.place_output_db_name != "":
        gp_out_file = "%s.def" % (params.place_output_db_name)
    else:
        path = "%s/%s" % (params.result_dir, params.design_name())
        if not os.path.exists(path):
            os.system("mkdir -p %s" % (path))
        gp_out_file = os.path.join(path, "%s.gp.%s" % (params.design_name(), params.solution_file_suffix()))
    placedb.write(params, gp_out_file)
    logging.info("[end non-linear placement at: %s, elapsed time %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt))
    
    # call external detailed placement
    # TODO: support more external placers, currently only support 
    # 1. NTUplace3/NTUplace4h with Bookshelf format 
    # 2. NTUplace_4dr with LEF/DEF format 
    if params.detailed_place_engine and os.path.exists(params.detailed_place_engine):
        logging.info("Use external detailed placement engine %s" % (params.detailed_place_engine))
        if params.solution_file_suffix() == "pl" and any(dp_engine in params.detailed_place_engine for dp_engine in ['ntuplace3', 'ntuplace4h']): 
            tt = time.time()
            
            logging.info('[start external detailed placement at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
            dp_out_file = gp_out_file.replace(".gp.pl", "")
            # add target density constraint if provided 
            target_density_cmd = ""
            if params.target_density < 1.0 and not params.routability_opt_flag:
                target_density_cmd = " -util %f" % (params.target_density)
            cmd = "%s -aux %s -loadpl %s %s -out %s -noglobal %s" % (params.detailed_place_engine, params.aux_input, gp_out_file, target_density_cmd, dp_out_file, params.detailed_place_command)
            logging.info("%s" % (cmd))
            
            os.system(cmd)
            
            if params.plot_flag: 
                # read solution and evaluate 
                placedb.read_pl(params, dp_out_file+".ntup.pl")
                iteration = len(metrics)
                pos = placer.init_pos
                pos[0:placedb.num_physical_nodes] = placedb.node_x
                pos[placedb.num_nodes:placedb.num_nodes+placedb.num_physical_nodes] = placedb.node_y
                hpwl, density_overflow, max_density = placer.validate(placedb, pos, iteration)
                logging.info("iteration %4d, HPWL %.3E, overflow %.3E, max density %.3E" % (iteration, hpwl, density_overflow, max_density))
                placer.plot(params, placedb, iteration, pos)
            logging.info("[end external detailed placement at: %s, elapsed time %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt))    
            
        elif 'ntuplace_4dr' in params.detailed_place_engine:
            tt = time.time()
            
            logging.info('[start external detailed placement at: %s]'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
            dp_out_file = gp_out_file.replace(".gp.def", "")
            cmd = "%s" % (params.detailed_place_engine)
            for lef in params.lef_input:
                if "tech.lef" in lef:
                    cmd += " -tech_lef %s" % (lef)
                else:
                    cmd += " -cell_lef %s" % (lef)
            cmd += " -floorplan_def %s" % (gp_out_file)
            cmd += " -verilog %s" % (params.verilog_input)
            cmd += " -out ntuplace_4dr_out"
            cmd += " -placement_constraints %s/placement.constraints" % (os.path.dirname(params.verilog_input))
            cmd += " -noglobal %s ; " % (params.detailed_place_command)
            cmd += "mv ntuplace_4dr_out.fence.plt %s.fense.plt ; " % (dp_out_file)
            cmd += "mv ntuplace_4dr_out.init.plt %s.init.plt ; " % (dp_out_file)
            cmd += "mv ntuplace_4dr_out %s.ntup.def ; " % (dp_out_file)
            cmd += "mv ntuplace_4dr_out.ntup.overflow.plt %s.ntup.overflow.plt ; " % (dp_out_file)
            cmd += "mv ntuplace_4dr_out.ntup.plt %s.ntup.plt ; " % (dp_out_file)
            if os.path.exists("%s/dat" % (os.path.dirname(dp_out_file))):
                cmd += "rm -r %s/dat ; " % (os.path.dirname(dp_out_file))
            cmd += "mv dat %s/ ; " % (os.path.dirname(dp_out_file))
            logging.info("%s" % (cmd))
            
            os.system(cmd)
            logging.info("[end external detailed placement at: %s, elapsed time %.2f seconds]" % (datetime.now().strftime('%Y-%m-%d %H:%M:%S'),time.time()-tt)) 
            
        else:
            logging.warning("External detailed placement only supports NTUplace3/NTUplace4dr API")
           
    elif params.detailed_place_engine:
        logging.warning("External detailed placement engine %s or aux file NOT found" % (params.detailed_place_engine))

    return metrics

import sys


if __name__ == "__main__":
    """
    @brief main function to invoke the entire placement flow. 
    """
    # run placement 
    tt = time.time()
    logging.root.name = 'DREAMPlace'
    logging.basicConfig(level=logging.INFO, format='[%(levelname)-7s] %(name)s - %(message)s', stream=sys.stdout)
    params = Params.Params()
    params.printWelcome()
    if len(sys.argv) == 1 or '-h' in sys.argv[1:] or '--help' in sys.argv[1:]:
        params.printHelp()
        exit()
    elif len(sys.argv) != 2:
        logging.error("One input parameters in json format in required")
        params.printHelp()
        exit()

    # load parameters 
    params.load(sys.argv[1])
    logging.info("parameters = %s" % (params))
    # control numpy multithreading
    os.environ["OMP_NUM_THREADS"] = "%d" % (params.num_threads)

    #shenhai added to start a thread to monitor the CPU load,2020/09/09
    args = sys.argv[1:]
    path = "%s/%s" % (params.result_dir, params.design_name())
    if not dir_exists(path):
        os.system("mkdir -p %s" % (path))
    monfile = path+'/'+params.design_name()+'.mon'
    pid = os.getpid()
    if torch.cuda.is_available():
        device = 'cuda'
    else:
        device = 'cpu'
    pthread_id=pthread_monitor(monfile,pid,device)
    

    
    
    place(params)

    # terminate the thread
    terminator(pthread_id)
    with open(monfile, 'a') as fp:       
        if device=='cuda':
            device_id = torch.cuda.current_device()
            gpu_mem=torch.cuda.max_memory_allocated(device_id)/1e6
        else:
            gpu_mem=0
            device_id=0
        sysload_monitoring(fp,pid,psutil.Process(pid).memory_info().rss/1e6,device,device_id,gpu_mem)   
        if device=='cuda':
            fp.write(torch.cuda.memory_summary(device_id))
        fp.write('\nEnd Time: %s\n'%(datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
        logging.info('monitering file stored in %s'%monfile)
    logging.info("placement takes %.3f seconds" % (time.time()-tt))




