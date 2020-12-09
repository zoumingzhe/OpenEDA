/** 
 * @file command_get_set_property.h
 * @date 2020-11-25
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TIMING_SDC_COMMAND_GET_SET_PROPERTY_H_
#define EDI_DB_TIMING_SDC_COMMAND_GET_SET_PROPERTY_H_

#define COMMAND_GET_SET_VAR(var, Var) \
    void set##Var (const decltype(var##_)& var) { var##_ = var; } \
    const decltype(var##_)& get##Var () const { return var##_; } \
    decltype(var##_)& get##Var () {return var##_;}

#define COMMAND_GET_SET_FLAG(var, Var) \
  private: \
    bool var : 1; \
  public: \
    void set##Var () { var = true; } \
    void reset##Var () { var = false; } \
    bool get##Var () { return var;} \
    bool is##Var () { return var; } 

#define COMMAND_BITSET_FLAG(flags, id, var, Var)  \
    void set##Var () { flags##_.set(id, 1); } \
    void reset##Var () { flags##_.set(id, 0); } \
    bool get##Var () { return flags##_.test(id); } \
    bool is##Var () { return flags##_.test(id); } 


#endif
