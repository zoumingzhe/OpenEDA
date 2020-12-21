##
# @file   NonLinearPlace.py
# @author Yibo Lin
# @date   Jul 2018
# @brief  Nonlinear placement engine to be called with parameters and placement database 
#

import os 
import sys
import time 
import pickle
import numpy as np 
import logging
import torch 
import gzip 
import copy
import matplotlib.pyplot as plt
if sys.version_info[0] < 3: 
    import cPickle as pickle
else:
    import _pickle as pickle
import BasicPlace
import PlaceObj
import NesterovAcceleratedGradientOptimizer
import EvalMetrics
import pdb 
from dreamplace.ops.draw_place import draw_place
from collections import OrderedDict

class NonLinearPlace (BasicPlace.BasicPlace):
    """
    @brief Nonlinear placement engine. 
    It takes parameters and placement database and runs placement flow. 
    """
    def __init__(self, params, placedb):
        """
        @brief initialization. 
        @param params parameters 
        @param placedb placement database 
        """
        super(NonLinearPlace, self).__init__(params, placedb)
        self.change_method=False
        self.model = None

    def __call__(self, params, placedb):
        """
        @brief Top API to solve placement. 
        @param params parameters 
        @param placedb placement database 
        """
        all_metrics = []
        total_iteration = 0
        if params.global_place_flag:

            total_iteration = self.global_place_electric_potential(params, placedb,all_metrics)
        if params.legalize_detailed_place_flag:
            self.lp_dp(params,placedb,all_metrics,total_iteration)
        else:
            self.post_processing(params,placedb,all_metrics,total_iteration)
        return all_metrics

        
    # a function to initialize learning rate 
    def initialize_learning_rate(self,pos):
        """
        @brief to get initial learning rate from json file,and estimate inital LR. 
        @param pos 
        """ 
        learning_rate = self.model.estimate_initial_learning_rate(pos, self.global_place_params["learning_rate"])
         # update learning rate 
        for param_group in self.optimizer.param_groups:
            param_group['lr'] = learning_rate.data
        

    # gamma stopping criteria 
    def Lgamma_stop_criterion(self,Lgamma_step, metrics,reserved_metric,params,first_globalplacement_loop): 
        """
        @brief stop iteration based on gamma updating. 
        @param params parameters 
        @param placedb placement database 
        @param Lgamma_step: iteration number on gamma
        @param reserved_metric: used to judge if overflow start oscillation
         
        """         
        with torch.no_grad():
            if len(metrics) > 1:                 
                cur_metric = metrics[-1][-1][-1]
                prev_metric = metrics[-2][-1][-1]

                if first_globalplacement_loop:
                    if Lgamma_step > self.Lgamma_step_thr and ((cur_metric.overflow < self.model.global_stop_overflow and cur_metric.hpwl >= prev_metric.hpwl) or cur_metric.max_density < params.target_density):
                        logging.debug("Lgamma stopping criteria: %d > %d and (( %g < %g and %g > %g ) or %g < 1.0)" % (Lgamma_step, self.Lgamma_step_thr,cur_metric.overflow,  self.model.global_stop_overflow,cur_metric.hpwl, prev_metric.hpwl, cur_metric.max_density))
                        return True
                else:
                    if ((cur_metric.overflow < self.model.global_stop_overflow and cur_metric.hpwl >= prev_metric.hpwl) or cur_metric.max_density < params.target_density):
                        logging.debug("Lgamma stopping criteria: %d > %d and (( %g < %g and %g > %g ) or %g < 1.0)" % (Lgamma_step, self.Lgamma_step_thr,cur_metric.overflow,  self.model.global_stop_overflow,cur_metric.hpwl, prev_metric.hpwl, cur_metric.max_density))
                        return True
            
            if len(metrics) > 100:     
                cur_metric = metrics[-1][-1][-1]
                #params.overflow_oscillation_control_threshold: this value by default is a very small number (0.01), so if does not set 
                #overflow_oscillation_control_threshold in purpose, this condition judgement does not work
                if cur_metric.overflow < params.overflow_oscillation_control_threshold and (cur_metric.iteration-reserved_metric.iteration)>20:  
                    logging.debug('oveflow oscillation! drop iteration. Reuse %d postion.'% reserved_metric.iteration)
                    return True
                              
            return False 
    def Llambda_stop_criterion(self,Lgamma_step, Llambda_density_weight_step, metrics): 
        """
        @brief stop iteration based on lambda updating. 
        @param Lambda_step: iteration number on lambda
         
        """            
        with torch.no_grad(): 
            if len(metrics) > 1: 
                cur_metric = metrics[-1][-1]
                prev_metric = metrics[-2][-1]
                if (cur_metric.overflow < self.model.global_stop_overflow and cur_metric.hpwl > prev_metric.hpwl) or cur_metric.max_density < 1.0:
                    logging.debug("Llambda stopping criteria: %d and (( %g < 0.1 and %g > %g ) or %g < 1.0)" % (Llambda_density_weight_step, cur_metric.overflow, cur_metric.hpwl, prev_metric.hpwl, cur_metric.max_density))
                    return True
        return False 



    def Lsub_stop_criterion(self,Lgamma_step, Llambda_density_weight_step, Lsub_step, metrics):
        """
        @brief stop iteration based on Lsub_step updating. 
         
        """          
        # use a moving average window for stopping criteria, for an example window of 3
        # 0, 1, 2, 3, 4, 5, 6
        #    window2
        #             window1
        moving_avg_window = max(min(self.model.Lsub_iteration // 2, 3), 1)
        with torch.no_grad(): 
            if len(metrics) >= moving_avg_window * 2: 
                cur_avg_obj = 0
                prev_avg_obj = 0
                for i in range(moving_avg_window):
                    cur_avg_obj += metrics[-1 - i].objective
                    prev_avg_obj += metrics[-1 - moving_avg_window - i].objective
                cur_avg_obj /= moving_avg_window 
                prev_avg_obj /= moving_avg_window
                threshold = 0.999
                if cur_avg_obj >= prev_avg_obj * threshold:
                    logging.debug("Lsub stopping criteria: %d and %g > %g * %g" % (Lsub_step, cur_avg_obj, prev_avg_obj, threshold))
                    return True 
        return False     

    def one_descent_step(self,Lgamma_step, Llambda_density_weight_step, Lsub_step, iteration, metrics,params,placedb,optimizer_name,total_iteration):
        """
        @brief this is the main step down alone the gradient
         
        """            
        t0 = time.time()
         # metric for this iteration
                     
        cur_metric = EvalMetrics.EvalMetrics(iteration, (Lgamma_step, Llambda_density_weight_step, Lsub_step))
        cur_metric.gamma = self.model.gamma.data
        cur_metric.density_weight = self.model.density_weight.data
        metrics.append(cur_metric)
        pos = self.model.data_collections.pos[0]
                    
        # move any out-of-bound cell back to placement region 
        self.op_collections.move_boundary_op(pos)

        # initialize density weight at the begining of each gamma iteration
        if torch.eq(self.model.density_weight, 0.0):
            self.model.initialize_density_weight(params, placedb)
            logging.info("density_weight = %.6E" % (self.model.density_weight.data))

        self.optimizer.zero_grad()
                    
        # t1 = time.time()
        cur_metric.evaluate(placedb, self.eval_ops, pos)
        self.model.overflow = cur_metric.overflow.data.clone()
       #logging.debug("evaluation %.3f ms" % ((time.time()-t1)*1000))
       #t2 = time.time()
        # shenhai add clone for these two values for deep copy, 2020/08/20
        cur_metric.density_weight = self.model.density_weight.data.clone()
        cur_metric.gamma = self.model.gamma.data.clone() 
        # as nesterov requires line search, we cannot follow the convention of other solvers
        if optimizer_name.lower() in ["sgd", "adam", "sgd_momentum", "sgd_nesterov"]: 
            obj, grad = self.model.obj_and_grad_fn(pos)
            cur_metric.objective = obj.data.clone()
        elif optimizer_name.lower() != "nesterov":
            assert 0, "unsupported optimizer %s" % (optimizer_name)
                   
        # plot placement 
        if params.plot_flag and total_iteration % params.plot_iteration == 0: 
            cur_pos = self.pos[0].data.clone().cpu().numpy()
          #   self.plot(params, placedb, iteration, cur_pos)
          #   self.model.check_gradient(params,placedb,iteration,pos)
            self.plot(params, placedb, total_iteration, cur_pos)
                        
        t3 = time.time()
                   

                    
        loss=self.optimizer.step()

        logging.info("optimizer step %.3f ms" % ((time.time()-t3)*1000))

        # nesterov has already computed the objective of the next step 
        if optimizer_name.lower() == "nesterov":
            cur_metric.objective = self.optimizer.param_groups[0]['obj_k_1'][0].data.clone()

        # actually reports the metric before step 
        logging.info('%s, total_iteration = %d'%(cur_metric,total_iteration))
#        logging.info('HPWL-WL = %.5f  gamma = %.5f density_weight = %.5f',cur_metric.hpwl-cur_metric.wirelength, cur_metric.gamma,cur_metric.density)
        logging.info("full step %.3f ms" % ((time.time()-t0)*1000))
        return #alpha_k #,wirelength_grad_norm, density_grad_norm

    def report_rsmt_wirelength(self, params, placedb):
        logging.info('reporting rsmt wire length:');
        return

    def global_place_electric_potential(self, params, placedb,all_metrics):
        """
        @brief solve placement with electric pentential method
        @param params parameters 
        @param placedb placement database 
        """
        iteration = 0
        # global placement 
        if params.global_place_flag: 
            # global placement may run in multiple stages according to user specification 
            Lbin_step = 0
            for global_place_params in params.global_place_stages:
                self.global_place_params = global_place_params
                # we formulate each stage as a 3-nested optimization problem 
                # f_gamma(g_density(h(x) ; density weight) ; gamma)
                # Lgamma      Llambda        Lsub
                # When optimizing an inner problem, the outer parameters are fixed.
                # This is a generalization to the eplace/RePlAce approach 

                if params.gpu: 
                    torch.cuda.synchronize()
                tt = time.time()
                # construct model and optimizer 
                density_weight = 0.0
                # construct placement model 
                self.model = PlaceObj.PlaceObj(density_weight, params, placedb, self.data_collections, self.op_collections, global_place_params).to(self.data_collections.pos[0].device)
                optimizer_name = global_place_params["optimizer"]

                # determine optimizer
                if optimizer_name.lower() == "adam": 
                    self.optimizer = torch.optim.Adam(self.parameters(), lr=0)
                elif optimizer_name.lower() == "sgd": 
                    self.optimizer = torch.optim.SGD(self.parameters(), lr=0)
                elif optimizer_name.lower() == "sgd_momentum": 
                    self.optimizer = torch.optim.SGD(self.parameters(), lr=0, momentum=0.9, nesterov=False)
                elif optimizer_name.lower() == "sgd_nesterov": 
                    self.optimizer = torch.optim.SGD(self.parameters(), lr=0, momentum=0.9, nesterov=True)
                elif optimizer_name.lower() == "nesterov": 
                    self.optimizer = NesterovAcceleratedGradientOptimizer.NesterovAcceleratedGradientOptimizer(self.parameters(), 
                            lr=0, 
                            obj_and_grad_fn=self.model.obj_and_grad_fn,
                            constraint_fn=self.op_collections.move_boundary_op,
                            )
                else:
                    assert 0, "unknown optimizer %s" % (optimizer_name)

                logging.info("use %s optimizer" % (optimizer_name))
                self.model.train()
                # defining evaluation ops 
                self.eval_ops = {
                        #"wirelength" : self.op_collections.wirelength_op, 
                        #"density" : self.op_collections.density_op, 
                        #"objective" : self.model.obj_fn, 
                        "hpwl" : self.op_collections.hpwl_op, 
                        "overflow" : self.op_collections.density_overflow_op
                        }
                if params.routability_opt_flag:
                    self.eval_ops.update({
                        'route_utilization' : self.op_collections.route_utilization_map_op, 
                        'pin_utilization' : self.op_collections.pin_utilization_map_op
                        })


                if iteration == 0: 
                    if params.gp_noise_ratio > 0.0: 
                        logging.info("add %g%% noise" % (params.gp_noise_ratio*100))
                        self.model.op_collections.noise_op(self.model.data_collections.pos[0], params.gp_noise_ratio)
                        self.initialize_learning_rate(self.model.data_collections.pos[0])
                # the state must be saved after setting learning rate 
                initial_state = copy.deepcopy(self.optimizer.state_dict())

                if params.gpu: 
                    torch.cuda.synchronize()
                logging.info("%s initialization takes %g seconds" % (optimizer_name, (time.time()-tt)))

                # as nesterov requires line search, we cannot follow the convention of other solvers
                if optimizer_name.lower() in {"sgd", "adam", "sgd_momentum", "sgd_nesterov"}: 
                    self.model.obj_and_grad_fn(self.model.data_collections.pos[0])
                elif optimizer_name.lower() != "nesterov":
                    assert 0, "unsupported optimizer %s" % (optimizer_name)

                # stopping criteria 
                def Lgamma_stop_criterion(Lgamma_step, metrics): 
                    with torch.no_grad():
                        if len(metrics) > 1: 
                            cur_metric = metrics[-1][-1][-1]
                            prev_metric = metrics[-2][-1][-1]
                            if Lgamma_step > 100 and ((cur_metric.overflow < params.stop_overflow and cur_metric.hpwl > prev_metric.hpwl) or cur_metric.max_density < params.target_density):
                                logging.debug("Lgamma stopping criteria: %d > 100 and (( %g < 0.1 and %g > %g ) or %g < 1.0)" % (Lgamma_step, cur_metric.overflow, cur_metric.hpwl, prev_metric.hpwl, cur_metric.max_density))
                                return True
                        return False 





                Lgamma_metrics = all_metrics

                if params.routability_opt_flag: 
                    adjust_area_flag = True
                    adjust_route_area_flag = params.adjust_route_area_flag
                    adjust_pin_area_flag = params.adjust_pin_area_flag
                    num_area_adjust = 0

                Llambda_flat_iteration = 0
                
                
                for Lgamma_step in range(self.model.Lgamma_iteration):
                    Lgamma_metrics.append([])
                    Llambda_metrics = Lgamma_metrics[-1]
                    for Llambda_density_weight_step in range(self.model.Llambda_density_weight_iteration):
                        Llambda_metrics.append([])
                        Lsub_metrics = Llambda_metrics[-1]
                        for Lsub_step in range(self.model.Lsub_iteration):
                            self.one_descent_step(Lgamma_step, Llambda_density_weight_step, Lsub_step, iteration, Lsub_metrics,params,placedb,optimizer_name,iteration)

                            iteration += 1
                            # stopping criteria 
                            if self.Lsub_stop_criterion(Lgamma_step, Llambda_density_weight_step, Lsub_step, Lsub_metrics):
                                break 
                        Llambda_flat_iteration += 1
                        # update density weight 
                        if Llambda_flat_iteration > 1: 
                            self.model.op_collections.update_density_weight_op(Llambda_metrics[-1][-1], Llambda_metrics[-2][-1] if len(Llambda_metrics) > 1 else Lgamma_metrics[-2][-1][-1], Llambda_flat_iteration)
                        #logging.debug("update density weight %.3f ms" % ((time.time()-t2)*1000))
                        if self.Llambda_stop_criterion(Lgamma_step, Llambda_density_weight_step, Llambda_metrics):
                            break 

                        if params.routability_opt_flag:
                            # for routability optimization 
                            if self.routability_optimization(initial_state,placedb,num_area_adjust,params,Llambda_metrics[-1][-1],adjust_area_flag,adjust_route_area_flag,adjust_pin_area_flag):
                                break

                        
                    # gradually reduce gamma to tradeoff smoothness and accuracy 
                    self.model.op_collections.update_gamma_op(Lgamma_step, Llambda_metrics[-1][-1].overflow)
                    self.model.op_collections.precondition_op.set_overflow(Llambda_metrics[-1][-1].overflow)
                    if Lgamma_stop_criterion(Lgamma_step, Lgamma_metrics):
                        break 
                        if params.routability_opt_flag:
                            # for routability optimization 
                            if self.routability_optimization(initial_state,placedb,num_area_adjust,params,Llambda_metrics[-1][-1],adjust_area_flag,adjust_route_area_flag,adjust_pin_area_flag):
                                break

                    # update learning rate 
                    if optimizer_name.lower() in ["sgd", "adam", "sgd_momentum", "sgd_nesterov", "cg"]: 
                        if 'learning_rate_decay' in global_place_params: 
                            for param_group in self.optimizer.param_groups:
                                param_group['lr'] *= global_place_params['learning_rate_decay']

                if "rsmt_wirelength_info" in global_place_params:
                    self.report_rsmt_wirelength(params, placedb)

                logging.info("optimizer %s takes %.3f seconds" % (optimizer_name, time.time()-tt))
            # recover node size and pin offset for legalization, since node size is adjusted in global placement
            if params.routability_opt_flag: 
                with torch.no_grad(): 
                    # convert lower left to centers 
                    self.pos[0][:placedb.num_movable_nodes].add_(self.data_collections.node_size_x[:placedb.num_movable_nodes] / 2)
                    self.pos[0][placedb.num_nodes: placedb.num_nodes + placedb.num_movable_nodes].add_(self.data_collections.node_size_y[:placedb.num_movable_nodes] / 2)
                    self.data_collections.node_size_x.copy_(self.data_collections.original_node_size_x)
                    self.data_collections.node_size_y.copy_(self.data_collections.original_node_size_y)
                    # use fixed centers as the anchor 
                    self.pos[0][:placedb.num_movable_nodes].sub_(self.data_collections.node_size_x[:placedb.num_movable_nodes] / 2)
                    self.pos[0][placedb.num_nodes: placedb.num_nodes + placedb.num_movable_nodes].sub_(self.data_collections.node_size_y[:placedb.num_movable_nodes] / 2)
                    self.data_collections.pin_offset_x.copy_(self.data_collections.original_pin_offset_x)
                    self.data_collections.pin_offset_y.copy_(self.data_collections.original_pin_offset_y)

        else: 
            cur_metric = EvalMetrics.EvalMetrics(iteration)
            all_metrics.append(cur_metric)
            cur_metric.evaluate(placedb, {"hpwl" : self.op_collections.hpwl_op}, self.pos[0])
            logging.info(cur_metric)

        
        return iteration


    def routability_optimization(self,initial_state,placedb,num_area_adjust,params,metric,adjust_area_flag,adjust_route_area_flag,adjust_pin_area_flag) :    
        """
        @brief routability checking and optimization
        @param params parameters 
        @param metric: current iteration information 
        """

        if params.routability_opt_flag and num_area_adjust < params.max_num_area_adjust and metric.overflow < params.node_area_adjust_overflow: 
            content = "routability optimization round %d: adjust area flags = (%d, %d, %d)" % (num_area_adjust, adjust_area_flag, adjust_route_area_flag, adjust_pin_area_flag)
            pos = self.model.data_collections.pos[0]
                      
            #cur_metric = EvalMetrics.EvalMetrics(iteration)
            #cur_metric.evaluate(placedb, {
            #    "hpwl" : self.op_collections.hpwl_op, 
            #    "overflow" : self.op_collections.density_overflow_op, 
            #    "route_utilization" : self.op_collections.route_utilization_map_op, 
            #    "pin_utilization" : self.op_collections.pin_utilization_map_op, 
            #    }, 
            #    pos)
            #logging.info(cur_metric)

            route_utilization_map = None 
            pin_utilization_map = None
                            
            if adjust_route_area_flag: 
                route_utilization_map = self.model.op_collections.route_utilization_map_op(pos)
                #route_utilization_map = self.model.op_collections.nctugr_congestion_map_op(pos)
                #if params.plot_flag:
                path = "%s/%s" % (params.result_dir, params.design_name())
                figname = "%s/plot/rudy%d.png" % (path, num_area_adjust)
                os.system("mkdir -p %s" % (os.path.dirname(figname)))
                plt.imsave(figname, route_utilization_map.data.cpu().numpy().T, origin='lower')
                            
            if adjust_pin_area_flag:
                pin_utilization_map = self.model.op_collections.pin_utilization_map_op(pos)
                #if params.plot_flag: 
                path = "%s/%s" % (params.result_dir, params.design_name())
                figname = "%s/plot/pin%d.png" % (path, num_area_adjust)
                os.system("mkdir -p %s" % (os.path.dirname(figname)))
                plt.imsave(figname, pin_utilization_map.data.cpu().numpy().T, origin='lower')
            adjust_area_flag, adjust_route_area_flag, adjust_pin_area_flag = self.model.op_collections.adjust_node_area_op(
                                    pos,
                                    route_utilization_map,
                                    pin_utilization_map
                                    )
            content += " -> (%d, %d, %d)" % (adjust_area_flag, adjust_route_area_flag, adjust_pin_area_flag)
            logging.info(content)
            if adjust_area_flag: 
                num_area_adjust += 1
                # restart Llambda 
                self.model.op_collections.density_op.reset() 
                self.model.op_collections.density_overflow_op.reset()
                self.model.op_collections.pin_utilization_map_op.reset()
                self.model.initialize_density_weight(params, placedb)
                self.model.density_weight.mul_(0.1 / params.density_weight)
                logging.info("density_weight = %.6E" % (self.model.density_weight.data))
                # load state to restart the optimizer 
                self.optimizer.load_state_dict(initial_state)
                # must after loading the state 
                self.initialize_learning_rate(pos)
                # increase iterations of the sub problem to slow down the search 
                self.model.Lsub_iteration = self.model.routability_Lsub_iteration

                                #cur_metric = EvalMetrics.EvalMetrics(iteration)
                                #cur_metric.evaluate(placedb, {
                                #    "hpwl" : self.op_collections.hpwl_op, 
                                #    "overflow" : self.op_collections.density_overflow_op, 
                                #    "route_utilization" : self.op_collections.route_utilization_map_op, 
                                #    "pin_utilization" : self.op_collections.pin_utilization_map_op, 
                                #    }, 
                                #    pos)
                                #logging.info(cur_metric)

                return True
        return False

    def post_processing(self,params,placedb,all_metrics,iteration):
        """
        @brief legalization, detailed placement, dump placement restults, etc.
        @param params parameters 
        @param all_metric: all iteration information 
        """

        # dump global placement solution for legalization 
        if params.dump_global_place_solution_flag: 
            self.dump(params, placedb, self.pos[0].cpu(), "%s.lg.pklz" %(params.design_name()))

        # plot placement 
        if params.plot_flag: 
            self.plot(params, placedb, iteration, self.pos[0].data.clone().cpu().numpy())
        
        # legalization 
        if params.legalize_flag:
            tt = time.time()
            self.pos[0].data.copy_(self.op_collections.legalize_op(self.pos[0]))
            logging.info("legalization takes %.3f seconds" % (time.time()-tt))
            cur_metric = EvalMetrics.EvalMetrics(iteration)
            all_metrics.append(cur_metric)
            cur_metric.evaluate(placedb, {"hpwl" : self.op_collections.hpwl_op}, self.pos[0])
            logging.info(cur_metric)
            iteration += 1

        # plot placement 
        if params.plot_flag: 
            self.plot(params, placedb, iteration, self.pos[0].data.clone().cpu().numpy())

        # dump legalization solution for detailed placement 
        if params.dump_legalize_solution_flag: 
            self.dump(params, placedb, self.pos[0].cpu(), "%s.dp.pklz" %(params.design_name()))

        # detailed placement 
        if params.detailed_place_flag: 
            tt = time.time()
            self.pos[0].data.copy_(self.op_collections.detailed_place_op(self.pos[0]))
            logging.info("detailed placement takes %.3f seconds" % (time.time()-tt))
            cur_metric = EvalMetrics.EvalMetrics(iteration)
            all_metrics.append(cur_metric)
            cur_metric.evaluate(placedb, {"hpwl" : self.op_collections.hpwl_op}, self.pos[0])
            logging.info(cur_metric)
            iteration += 1



        # save results 
        cur_pos = self.pos[0].data.clone().cpu().numpy()
        # apply solution 
        placedb.apply(params, cur_pos[0:placedb.num_movable_nodes], cur_pos[placedb.num_nodes:placedb.num_nodes+placedb.num_movable_nodes])
        # plot placement 
        if params.plot_flag: 
            self.plot(params, placedb, iteration, cur_pos)
        return #all_metrics 


    def lp_dp(self,params,placedb,all_metrics,iteration):
        """
        @brief lp_dp flow: legalization, detailed placement, dump placement restults, etc.
        @param params parameters 
        @param all_metric: all iteration information 
        """
        logging.info("Start legalization and detailed place");
        # dump global placement solution for legalization 
        if params.dump_global_place_solution_flag: 
            self.dump(params, placedb, self.pos[0].cpu(), "%s.gp.pklz" %(params.design_name()))

        # plot placement 
        if params.plot_flag: 
            self.plot(params, placedb, iteration, self.pos[0].data.clone().cpu().numpy())
        
        # legalization and detail
        if params.legalize_flag:
            tt = time.time()
            self.op_collections.lp_dp_op()
            logging.info("Legalization and detailed placement takes %.3f seconds" % (time.time()-tt))
            cur_metric = EvalMetrics.EvalMetrics(iteration)
            all_metrics.append(cur_metric)
            cur_metric.evaluate(placedb, {"hpwl" : self.op_collections.hpwl_op}, self.pos[0])
            logging.info(cur_metric)
            iteration += 1

        # plot placement 
        if params.plot_flag: 
            self.plot(params, placedb, iteration, self.pos[0].data.clone().cpu().numpy())

        # dump legalization solution after lp_dp
        if params.dump_legalize_solution_flag: 
            self.dump(params, placedb, self.pos[0].cpu(), "%s.lp_dp.pklz" %(params.design_name()))

        # save results 
        cur_pos = self.pos[0].data.clone().cpu().numpy()

        # apply solution 
        placedb.apply(params, cur_pos[0:placedb.num_movable_nodes], cur_pos[placedb.num_nodes:placedb.num_nodes+placedb.num_movable_nodes])
        # plot placement 
        if params.plot_flag: 
            self.plot(params, placedb, iteration, cur_pos)
        return #all_metrics 

    
