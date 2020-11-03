##
# @file   unitlity_mon.py
# @author shenhai
# @date   Sept 2020
# @brief  monitor CPU/GPU runtime, load and memory during placement.
#
#
from __future__ import division
from __future__ import print_function

import os
import socket
import getpass
import sys
import re
import traceback
import shutil
from timeit import default_timer 
from datetime import datetime
import logging
import time 
import psutil
import pathlib
import subprocess
import multiprocessing
from time import sleep
from threading import Thread
import io
import ctypes
import inspect
import torch

START_TIME = default_timer()
START_DATE = datetime.now()

cpu_alert_time = 0
memory_alert_time = 0


def __async_raise(thread_Id, exctype):
    '''
    pop out thread termination
    '''
    thread_Id = ctypes.c_long(thread_Id)
    if not inspect.isclass(exctype):
        exctype = type(exctype)
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_Id, ctypes.py_object(exctype))
    if res == 0:
        raise ValueError("invalid thread id")
    elif res != 1:
        ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_Id, None)
        raise SystemError("PyThreadState_SEtAsyncExc failed")

def terminator(thread):
    '''
    terminate a thread
    '''
    __async_raise(thread.ident, SystemExit)
    
def dir_exists(dir):
    '''
    check if one dir exists
    '''
    return os.path.exists(dir) and os.path.isdir(dir)

def file_exists(file):
    '''
    check if one file exists
    '''

    return os.path.exists(file) and os.path.isfile(file)

def shell_cmd(cmd):
    '''
    execute a shell command and return results
    '''
    response = subprocess.run( cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True, shell=True)
    output = response.stdout.decode('utf-8').rstrip('\n')
    return output

def bytes2human(n):
    """
    >>>bytes2human(10000)
    '9.8k'
    >>>bytes2human(100001221)
    '95.4M'
    """
    
    
    symbols = ('K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y')
    prefix = {}
    for i, s in enumerate(symbols):
        prefix[s] = 1 << (i + 1) * 10
    for s in reversed(symbols):
        if n >= prefix[s]:
            value = float(n) / prefix[s]
            return '%.1f%s' % (value, s)
    return "%sB" % n

def report_gpu_info(fmon):
    if not torch.cuda.is_available():
        return
    fmon.write('\nGPU Information \n')
    fmon.write('-------------------------- \n')
    
    for i in range(torch.cuda.device_count()):

        fmon.write('gpu name:%s\n'%torch.cuda.get_device_name(i))
    current_device = torch.cuda.current_device()    
    fmon.write('cuda current device:%s\n'%current_device)

    return current_device
def report_system_info(fmon,basedir,pid,device):
    '''
    report sytem information
    '''
    cpu_name = ''
    cpu_cache_size = ''
    cpu_cores = ''
    cpu_mhz = ''
    fmon.write('System Monitoring \n')
    fmon.write('-------------------------- \n')
    fmon.write('Start Time: %s\n'%(START_DATE.strftime('%Y-%m-%d %H:%M:%S')))
    fmon.write('\nCPU Information \n')
    fmon.write('-------------------------- \n')
    try:
      with open('/proc/cpuinfo', 'r') as f:
        while True:
            line = f.readline() 
            if len(line) == 0:
                break
            line = line.lstrip(' ')
            line = line.rstrip('\n')            
            if 'cpu MHz' in line:
                line = re.sub('cpu MHz\s+:', 'CPU MHz:', line)
                cpu_mhz = line
            elif 'model name' in line :
                line = re.sub('model name\s+:', 'Model Name:', line)
                cpu_name = line            
            elif 'cpu cores' in line :
                line = re.sub('cpu cores\s+:', 'CPU Cores:', line)
                cpu_cores = line            
            elif 'cache size' in line:
                line = re.sub('cache size\s+:', 'Cache Size:', line)
                cache_size = line
            
            if len(cpu_name) > 0 and len(cpu_cache_size) > 0 and len(cpu_cores) > 0 and len(cpu_mhz) > 0:               
                break
    except Exception:
        pass

    tmp = ''
    workingdir=''

    if dir_exists(basedir):
        workingdir = shell_cmd('df --block-size=100M %s'%(basedir))

    cpu_core_number= multiprocessing.cpu_count()
    release = ''
    if file_exists('/etc/redhat-release'):
        release = shell_cmd('cat /etc/redhat-release')
    elif file_exists('/etc/SuSE-release'):
        release = shell_cmd('cat /etc/SuSE-release')
    
    

    cpumemory = psutil.virtual_memory()    
    fmon.write('Hostname: %s\n'%socket.gethostname())
    fmon.write('Core Number: %d\n'%cpu_core_number)
    fmon.write('OS kernel: %s\n'%shell_cmd('uname -rs'))
    if len(release) > 0:
        fmon.write('OS release: %s\n'%release)    
    if len(cpu_name) > 0:
        fmon.write(cpu_name + '\n')
    if len(cpu_cores) > 0:
        fmon.write(cpu_cores + '\n')    
    if len(cache_size) > 0:
        fmon.write(cache_size + '\n')
    if len(cpu_mhz) > 0:
        fmon.write(cpu_mhz + '\n')   

    # Get the load average over 
    # the last 1, 5, and 15 minutes  
    cpuload = os.getloadavg()
    fmon.write("Load average over the last 1 minute:%.2f\n"%cpuload[0]) 
    fmon.write("Load average over the last 5 minute:%.2f\n"% cpuload[1]) 
    fmon.write("Load average over the last 15 minute:%.2f\n"%cpuload[2])
     
    fmon.write('Virtual Memory: total= %s, available=%s, percent=%s%%, used=%s, free=%s, active=%s\n'%(bytes2human(cpumemory[0]),bytes2human(cpumemory[1]),cpumemory[2],bytes2human(cpumemory[3]),bytes2human(cpumemory[4]),bytes2human(cpumemory[6])))
    
    
    fmon.write('PID: %d \n'%pid)
    if device=='cuda':
        device_id = report_gpu_info(fmon)
    else:
        device_id=0
    fmon.flush() 
    if len(workingdir) > 0:
        full_workingdir = os.path.abspath(basedir)
        fmon.write('Working directory: %s \n'%(full_workingdir))
    fmon.write('\n')
    if device=='cuda':
        fmon.write('     wall_time  free_MEM  used_MEM CPU_load     MEM(PID)      Peak_MEM(PID)\t     GPU_MEM \t     GPU_Peak_MEM\n')
        fmon.write('     (s)        (GB)      %        %            (MB)          (MB)          \t    (MB)        \t  (MB)\n')
    else:
        fmon.write('     wall_time  free_MEM  used_MEM CPU_load     MEM(PID)      Peak_MEM(PID) \n')
        fmon.write('     (s)        (GB)      %        %            (MB)          (MB)          \n')

    fmon.flush()

    return cpu_core_number,device_id


def sysload_monitoring(fp,pid,peakmemory_pid,device,device_id,peak_memory_gpu):
    '''
    to write cpu/gpu load in cmon file
    '''

    loads = os.getloadavg()
    cpu_load = loads[0]
    memory = psutil.virtual_memory()
    memory_used = memory[2]
    memory_free = memory[4] / 1e9
    memory_pid=round(psutil.Process(pid).memory_info().rss/1e6)
    peak_memory = max(peakmemory_pid,memory_pid)
    tt = default_timer()-START_TIME
    if device =='cuda':
        gpu_mem=torch.cuda.max_memory_allocated(device_id)/1e6
        peak_memory_gpu=max(peak_memory_gpu,torch.cuda.max_memory_allocated(device_id)/1e6)
        try:
        
            fp.write('%10.3f\t%9.1f\t %4.1f\t  %5.2f\t        %5.0f\t     %5.0f\t\t       %5.0f\t\t   %5.0f\n'%(tt, memory_free, memory_used,cpu_load,memory_pid,peak_memory,gpu_mem,peak_memory_gpu)) 
            fp.flush()
        except Exception as err:
            logging.warning('Cannot write mon file ' + monfile)
            return 0
    else:
        try:
        
            fp.write('%10.3f\t%9.1f\t %4.1f\t  %5.2f\t        %5.0f\t     %5.0f\n'%(tt, memory_free, memory_used,cpu_load,memory_pid,peak_memory)) 
            fp.flush()
        except Exception as err:
            logging.warning('Cannot write mon file ' + monfile)
            return 0

    return 1

def load_alert(max_load_limit, min_space_limit, alert_time_limit):
    '''
    monitor CPU load, if exceed limitation, then error out
    '''
    loads = os.getloadavg()
    cpu_load = loads[0]
    memory = psutil.virtual_memory()
    memory_free = memory[4] / 1e9
    tt = default_timer()-START_TIME
    global cpu_alert_time
    global memory_alert_time

    #check memory
    if memory_free < min_space_limit:
        if memory_alert_time > 0:
            if tt - memory_alert_time > alert_time_limit:
                logging.error('Free memory %5.1f is below %d GB for more than %d minutes.'%(memory_free, min_space_limit, alert_time_limit/60))
                return 0
        else:
            memory_alert_time = tt
    else:
        memory_alert_time = 0

    #check CPU load
    if cpu_load > max_load_limit:
        if cpu_alert_time > 0:
            if tt - cpu_alert_time > alert_time_limit:
                logging.error('CPU load %5.2f exceeds %d for more than %d minutes.'%(cpu_load, max_load_limit, alert_time_limit/60))
                return 0

        else:
            cpu_alert_time = tt
            
    else:
        cpu_alert_time = 0



    return 1

def pmonitor(monfile,min_space_limit,max_time_limit,moninterval,pid,cpu_cores,device,device_id):
    '''
    to monitor CPU/GPU load 
    '''
    try:
        with open(monfile, 'a') as fp:
            max_load_limit = cpu_cores*2
            tt=time.time()
            max_memory = 0
            first_entry = True
            max_memory_gpu=0
            while True:
                peakmemory_pid=max(max_memory,round(psutil.Process(pid).memory_info().rss/1e6))
                if device =='cuda':
                    peak_memory_gpu=max(max_memory_gpu,torch.cuda.max_memory_allocated(device_id)/1e6)
                else:
                    peak_memory_gpu = 0
              #  logging.info('peakmemory_pid=%d',peakmemory_pid)
                if time.time()-tt>=moninterval or first_entry:
                    fp_flag = sysload_monitoring(fp,pid,peakmemory_pid,device,device_id,peak_memory_gpu)
                    mem_result = load_alert(max_load_limit, min_space_limit, max_time_limit)
                    tt=time.time()
                    max_memory=0
                    max_memory_gpu=0
                    first_entry = False
                    if mem_result == 0 or fp_flag==0:     
                        break
                sleep(0.1)
                
    except Exception as err:        
        logging.warning('Cannot open file ' + monfile)
        return 
    

def pthread_monitor(monfile,pid,device):
    '''
    start a thread of pmonitor
    '''
    moninterval = 5
    max_time_limit = 600
    min_space_limit = 2
    basedir = os.path.dirname(monfile)
    
    try:
        with open(monfile, 'w') as fp:
            cpu_cores,device_id = report_system_info(fp,basedir,pid,device)
            
            fp.close()
    except Exception as err:        
        logging.warning('Cannot open file %s, or file writing failed.'% monfile, ' or file writing failed.')   
      
    pthr = Thread(target=pmonitor, args=(monfile, min_space_limit,max_time_limit,moninterval,pid,cpu_cores,device,device_id))
    pthr.daemon = True
    pthr.start()
    
    return pthr


