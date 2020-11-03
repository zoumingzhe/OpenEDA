/**
 * @file timinglib_commondef.h
 * @date 2020-08-26
 * @brief liberty basic structure
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_COMMONDEF_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_COMMONDEF_H_

#include <string>

namespace open_edi {
namespace db {

enum class TimingModel { kNLDM, kCCS, kECSM, kUnknown };

inline std::string toString(TimingModel const &v) {
    switch (v) {
        case TimingModel::kNLDM:
            return "NLDM";
        case TimingModel::kCCS:
            return "CSS";
        case TimingModel::kECSM:
            return "ECSM";
        default:
            return "Unknown";
    }
}

enum class WireLoadTree { kBestCase, kBalanced, kWorstCase, kUnknown };

inline std::string toString(WireLoadTree const &v) {
    switch (v) {
        case WireLoadTree::kBestCase:
            return "best_case";
        case WireLoadTree::kBalanced:
            return "balanced";
        case WireLoadTree::kWorstCase:
            return "worst_case";
        default:
            return "Unknown";
    }
}

enum WireLoadMode { kTop, kSegmented, kEnclosed, kUnknown };
inline std::string toString(WireLoadMode const &w) {
    switch (w) {
        case WireLoadMode::kTop:
            return "top";
        case WireLoadMode::kSegmented:
            return "segmented";
        case WireLoadMode::kEnclosed:
            return "enclosed";
        default:
            return "Unknown";
    }
}

enum class ThresholdVotageGroup { kSVT, kLVT, kHVT, kULVT, kUnknown };

inline std::string toString(ThresholdVotageGroup const &v) {
    switch (v) {
        case ThresholdVotageGroup::kSVT:
            return "svt";
        case ThresholdVotageGroup::kLVT:
            return "lvt";
        case ThresholdVotageGroup::kHVT:
            return "hvt";
        case ThresholdVotageGroup::kULVT:
            return "ulvt";
        default:
            return "Unknown";
    }
}

enum class SwitchCellType { kCoarse_Grain, kFine_Grain, kUnknown };
inline std::string toString(SwitchCellType const &s) {
    switch (s) {
        case SwitchCellType::kCoarse_Grain:
            return "coarse_grain";
        case SwitchCellType::kFine_Grain:
            return "fine_grain";

        default:
            return "Unknown";
    }
}
enum class AntennaDiodeType { kPower, kGround, kPowerAndGround, kUnknown };

inline std::string toString(AntennaDiodeType const &v) {
    switch (v) {
        case AntennaDiodeType::kPower:
            return "power";
        case AntennaDiodeType::kGround:
            return "ground";
        case AntennaDiodeType::kPowerAndGround:
            return "power_and_ground";
        default:
            return "Unknown";
    }
}

enum class PinType {
    kAsync,
    kClear,
    kClock,
    kData,
    kThreeState,
    kScan,
    kClockGatingEnable,
    kClockGatingClock,
    kHierarchy,
    kPreset,
    kThreeStateEnable,
    kThreeStateOutput,
    kUnknown
};
inline std::string toString(PinType const &v) {
    switch (v) {
        case PinType::kAsync:
            return "async";
        case PinType::kClear:
            return "clear";
        case PinType::kClock:
            return "clock";
        case PinType::kData:
            return "data";
        case PinType::kThreeState:
            return "three_state";
        case PinType::kScan:
            return "scan";
        case PinType::kClockGatingEnable:
            return "clock_gating_enable";
        case PinType::kClockGatingClock:
            return "clock_gating_clock";
        case PinType::kHierarchy:
            return "hierarchy";
        case PinType::kPreset:
            return "preset";
        case PinType::kThreeStateEnable:
            return "three_state_enable";
        case PinType::kThreeStateOutput:
            return "three_state_output";
        default:
            return "Unknown";
    }
}

enum class TableAxisVariable {
    kTotal_Output_Net_Capacitance,
    kEqual_Or_Opposite_Output_Net_Capacitance,
    kInput_Net_Transition,
    kInput_Transition_Time,
    kRelated_Pin_Transition,
    kConstrained_Pin_Transition,
    kOutput_Pin_Transition,
    kConnect_Delay,
    kRelated_Out_Total_Output_Net_Capacitance,
    kTime,
    kIv_Output_Voltage,
    kInput_Noise_Width,
    kInput_Noise_Height,
    kInput_Voltage,
    kOutput_Voltage,
    kPath_Depth,
    kPath_Distance,
    kNormalized_Voltage,
    kUnknown
};
inline std::string toString(TableAxisVariable const &t) {
    switch (t) {
        case TableAxisVariable::kTotal_Output_Net_Capacitance:
            return "total_output_net_capacitance";
        case TableAxisVariable::kEqual_Or_Opposite_Output_Net_Capacitance:
            return "equal_or_opposite_output_net_capacitance";
        case TableAxisVariable::kInput_Net_Transition:
            return "input_net_transition";
        case TableAxisVariable::kInput_Transition_Time:
            return "input_transition_time";
        case TableAxisVariable::kRelated_Pin_Transition:
            return "related_pin_transition";
        case TableAxisVariable::kConstrained_Pin_Transition:
            return "constrained_pin_transition";
        case TableAxisVariable::kOutput_Pin_Transition:
            return "output_pin_transition";
        case TableAxisVariable::kConnect_Delay:
            return "connect_delay";
        case TableAxisVariable::kRelated_Out_Total_Output_Net_Capacitance:
            return "related_out_total_output_net_capacitance";
        case TableAxisVariable::kTime:
            return "time";
        case TableAxisVariable::kIv_Output_Voltage:
            return "iv_output_voltage";
        case TableAxisVariable::kInput_Noise_Width:
            return "input_noise_width";
        case TableAxisVariable::kInput_Noise_Height:
            return "input_noise_height";
        case TableAxisVariable::kInput_Voltage:
            return "input_voltage";
        case TableAxisVariable::kOutput_Voltage:
            return "output_voltage";
        case TableAxisVariable::kPath_Depth:
            return "path_depth";
        case TableAxisVariable::kPath_Distance:
            return "path_distance";
        case TableAxisVariable::kNormalized_Voltage:
            return "normalized_voltage";
        default:
            return "Unknown";
    }
}

enum class PGType {
    kPrimary_Power,
    kPrimary_Ground,
    kBackup_Power,
    kBackup_Ground,
    kInternal_Power,
    kInternal_Ground,
    kPwell,
    kNwell,
    kDeepnwell,
    kDeeppwell,
    kUnknown
};
inline std::string toString(PGType const &t) {
    switch (t) {
        case PGType::kPrimary_Power:
            return "primary_power";
        case PGType::kPrimary_Ground:
            return "primary_ground";
        case PGType::kBackup_Power:
            return "backup_power";
        case PGType::kBackup_Ground:
            return "backup_ground";
        case PGType::kInternal_Power:
            return "internal_power";
        case PGType::kInternal_Ground:
            return "internal_ground";
        case PGType::kPwell:
            return "pwell";
        case PGType::kNwell:
            return "nwell";
        case PGType::kDeepnwell:
            return "deepnwell";
        case PGType::kDeeppwell:
            return "deeppwell";
        default:
            return "Unknown";
    }
}

enum class TimingSense {
    kPositive_Unate,
    kNegative_Unate,
    kNon_Unate,
    kNone,
    kUnknown
};
inline std::string toString(TimingSense const &t) {
    switch (t) {
        case TimingSense::kPositive_Unate:
            return "positive_unate";
        case TimingSense::kNegative_Unate:
            return "negative_unate";
        case TimingSense::kNon_Unate:
            return "non_unate";
        case TimingSense::kNone:
            return "none";
        default:
            return "Unknown";
    }
}

enum class TimingType {
    kCombinational,
    kCombinational_Rise,
    kCombinational_Fall,
    kThree_State_Disable,
    kThree_State_Disable_Rise,
    kThree_State_Disable_Fall,
    kThree_State_Enable,
    kThree_State_Enable_Rise,
    kThree_State_Enable_Fall,
    kRising_Edge,
    kFalling_Edge,
    kPreset,
    kClear,
    kHold_Rising,
    kHold_Falling,
    kSetup_Rising,
    kSetup_Falling,
    kRecovery_Rising,
    kRecovery_Falling,
    kSkew_Rising,
    kSkew_Falling,
    kRemoval_Rising,
    kRemoval_Falling,
    kMin_Pulse_Width,
    kMinimum_Period,
    kMax_Clock_Tree_Path,
    kMin_Clock_Tree_Path,
    kNon_Seq_Setup_Rising,
    kNon_Seq_Setup_Falling,
    kNon_Seq_Hold_Falling,
    kNochange_High_High,
    kNochange_High_Low,
    kNochange_Low_High,
    kNochange_Low_Low,
    kUnknown
};
inline std::string toString(TimingType const &t) {
    switch (t) {
        case TimingType::kCombinational:
            return "combinational";
        case TimingType::kCombinational_Rise:
            return "combinational_rise";
        case TimingType::kCombinational_Fall:
            return "combinational_fall";
        case TimingType::kThree_State_Disable:
            return "three_state_disable";
        case TimingType::kThree_State_Disable_Rise:
            return "three_state_disable_rise";
        case TimingType::kThree_State_Disable_Fall:
            return "three_state_disable_fall";
        case TimingType::kThree_State_Enable:
            return "three_state_enable";
        case TimingType::kThree_State_Enable_Rise:
            return "three_state_enable_rise";
        case TimingType::kThree_State_Enable_Fall:
            return "three_state_enable_fall";
        case TimingType::kRising_Edge:
            return "rising_edge";
        case TimingType::kFalling_Edge:
            return "falling_edge";
        case TimingType::kPreset:
            return "preset";
        case TimingType::kClear:
            return "clear";
        case TimingType::kHold_Rising:
            return "hold_rising";
        case TimingType::kHold_Falling:
            return "hold_falling";
        case TimingType::kSetup_Rising:
            return "setup_rising";
        case TimingType::kSetup_Falling:
            return "setup_falling";
        case TimingType::kRecovery_Rising:
            return "recovery_rising";
        case TimingType::kRecovery_Falling:
            return "recovery_falling";
        case TimingType::kSkew_Rising:
            return "skew_rising";
        case TimingType::kSkew_Falling:
            return "skew_falling";
        case TimingType::kRemoval_Rising:
            return "removal_rising";
        case TimingType::kRemoval_Falling:
            return "removal_falling";
        case TimingType::kMin_Pulse_Width:
            return "min_pulse_width";
        case TimingType::kMinimum_Period:
            return "minimum_period";
        case TimingType::kMax_Clock_Tree_Path:
            return "max_clock_tree_path";
        case TimingType::kMin_Clock_Tree_Path:
            return "min_clock_tree_path";
        case TimingType::kNon_Seq_Setup_Rising:
            return "non_seq_setup_rising";
        case TimingType::kNon_Seq_Setup_Falling:
            return "non_seq_setup_falling";
        case TimingType::kNon_Seq_Hold_Falling:
            return "non_seq_hold_falling";
        case TimingType::kNochange_High_High:
            return "nochange_high_high";
        case TimingType::kNochange_High_Low:
            return "nochange_high_low";
        case TimingType::kNochange_Low_High:
            return "nochange_low_high";
        case TimingType::kNochange_Low_Low:
            return "nochange_low_low";
        default:
            return "Unknown";
    }
}

enum class ScaleFactorPvt { kProcess, kVolt, kTemp, kUnknown };
inline std::string toString(ScaleFactorPvt const &s) {
    switch (s) {
        case ScaleFactorPvt::kProcess:
            return "process";
        case ScaleFactorPvt::kVolt:
            return "volt";
        case ScaleFactorPvt::kTemp:
            return "temp";
        default:
            return "Unknown";
    }
}

enum class ScaleFactorType : unsigned {
    kPin_Cap,
    kWire_Cap,
    kWire_Res,
    kMin_Period,
    kCell,
    kHold,
    kSetup,
    kRecovery,
    kRemoval,
    kNochange,
    kSkew,
    kLeakage_Power,
    kInternal_Power,
    kTransition,
    kMin_Pulse_Width,
    kUnknown
};
inline std::string toString(ScaleFactorType const &t) {
    switch (t) {
        case ScaleFactorType::kPin_Cap:
            return "pin_cap";
        case ScaleFactorType::kWire_Cap:
            return "wire_cap";
        case ScaleFactorType::kWire_Res:
            return "wire_res";
        case ScaleFactorType::kMin_Period:
            return "min_period";
        case ScaleFactorType::kCell:
            return "cell";
        case ScaleFactorType::kHold:
            return "hold";
        case ScaleFactorType::kSetup:
            return "setup";
        case ScaleFactorType::kRecovery:
            return "recovery";
        case ScaleFactorType::kRemoval:
            return "removal";
        case ScaleFactorType::kNochange:
            return "nochange";
        case ScaleFactorType::kSkew:
            return "skew";
        case ScaleFactorType::kLeakage_Power:
            return "leakage_power";
        case ScaleFactorType::kInternal_Power:
            return "internal_power";
        case ScaleFactorType::kTransition:
            return "transition";
        case ScaleFactorType::kMin_Pulse_Width:
            return "min_pulse_width";
        case ScaleFactorType::kUnknown:
            return "Unknown";
        default:
            return "Unknown";
    }
    return "Unknown";
}

enum class ScaleFactorRiseFall { kRise, kFall, kUnknown };
inline std::string toString(ScaleFactorRiseFall const &s) {
    switch (s) {
        case ScaleFactorRiseFall::kRise:
            return "rise";
        case ScaleFactorRiseFall::kFall:
            return "fall";
        default:
            return "Unknown";
    }
}

enum class ClockGateIntegratedType {
    kGeneric,
    kLatch_Posedge,
    kLatch_Posedge_Precontrol,
    kLatch_Posedge_Postcontrol,
    kLatch_Posedge_Precontrol_Obs,
    kLatch_Posedge_Postcontrol_Obs,
    kLatch_Negedge,
    kLatch_Negedge_Precontrol,
    kLatch_Negedge_Postcontrol,
    kLatch_Negedge_Precontrol_Obs,
    kLatch_Negedge_Postcontrol_Obs,
    kFF_Posedge,
    kFF_Posedge_Precontrol,
    kFF_Posedge_Postcontrol,
    kFF_Posedge_Precontrol_Obs,
    kFF_Posedge_Postcontrol_Obs,
    kFF_Negedge,
    kFF_Negedge_Precontrol,
    kFF_Negedge_Postcontrol,
    kFF_Negedge_Precontrol_Obs,
    kFF_Negedge_Postcontrol_Obs,
    kNone_Posedge,
    kNone_Posedge_Control,
    kNone_Posedge_Control_Obs,
    kNone_Negedge,
    kNone_Negedge_Control,
    kNone_Negedge_Control_Obs,
    kTruegeneric,
    kUnknown
};
inline std::string toString(ClockGateIntegratedType const &c) {
    switch (c) {
        case ClockGateIntegratedType::kGeneric:
            return "generic";
        case ClockGateIntegratedType::kLatch_Posedge:
            return "latch_posedge";
        case ClockGateIntegratedType::kLatch_Posedge_Precontrol:
            return "latch_posedge_precontrol";
        case ClockGateIntegratedType::kLatch_Posedge_Postcontrol:
            return "latch_posedge_postcontrol";
        case ClockGateIntegratedType::kLatch_Posedge_Precontrol_Obs:
            return "latch_posedge_precontrol_obs";
        case ClockGateIntegratedType::kLatch_Posedge_Postcontrol_Obs:
            return "latch_posedge_postcontrol_obs";
        case ClockGateIntegratedType::kLatch_Negedge:
            return "latch_negedge";
        case ClockGateIntegratedType::kLatch_Negedge_Precontrol:
            return "latch_negedge_precontrol";
        case ClockGateIntegratedType::kLatch_Negedge_Postcontrol:
            return "latch_negedge_postcontrol";
        case ClockGateIntegratedType::kLatch_Negedge_Precontrol_Obs:
            return "latch_negedge_precontrol_obs";
        case ClockGateIntegratedType::kLatch_Negedge_Postcontrol_Obs:
            return "latch_negedge_postcontrol_obs";
        case ClockGateIntegratedType::kFF_Posedge:
            return "ff_posedge";
        case ClockGateIntegratedType::kFF_Posedge_Precontrol:
            return "ff_posedge_precontrol";
        case ClockGateIntegratedType::kFF_Posedge_Postcontrol:
            return "ff_posedge_postcontrol";
        case ClockGateIntegratedType::kFF_Posedge_Precontrol_Obs:
            return "ff_posedge_precontrol_obs";
        case ClockGateIntegratedType::kFF_Posedge_Postcontrol_Obs:
            return "ff_posedge_postcontrol_obs";
        case ClockGateIntegratedType::kFF_Negedge:
            return "ff_negedge";
        case ClockGateIntegratedType::kFF_Negedge_Precontrol:
            return "ff_negedge_precontrol";
        case ClockGateIntegratedType::kFF_Negedge_Postcontrol:
            return "ff_negedge_postcontrol";
        case ClockGateIntegratedType::kFF_Negedge_Precontrol_Obs:
            return "ff_negedge_precontrol_obs";
        case ClockGateIntegratedType::kFF_Negedge_Postcontrol_Obs:
            return "ff_negedge_postcontrol_obs";
        case ClockGateIntegratedType::kNone_Posedge:
            return "none_posedge";
        case ClockGateIntegratedType::kNone_Posedge_Control:
            return "none_posedge_control";
        case ClockGateIntegratedType::kNone_Posedge_Control_Obs:
            return "none_posedge_control_obs";
        case ClockGateIntegratedType::kNone_Negedge:
            return "none_negedge";
        case ClockGateIntegratedType::kNone_Negedge_Control:
            return "none_negedge_control";
        case ClockGateIntegratedType::kNone_Negedge_Control_Obs:
            return "none_negedge_control_obs";
        case ClockGateIntegratedType::kTruegeneric:
            return "truegeneric";
        default:
            return "Unknown";
    }
}

enum class FuncOpType {
    kOP_ZERO,
    kOP_ONE,
    kOP_ADD,
    kOP_OR,
    kOP_NOT,
    kOP_XOR,
    kOP_TTERM,
    kUnknown
};
inline std::string toString(FuncOpType const &f) {
    switch (f) {
        case FuncOpType::kOP_ZERO:
            return "0";
        case FuncOpType::kOP_ONE:
            return "1";
        case FuncOpType::kOP_ADD:
            return "*";
        case FuncOpType::kOP_OR:
            return "+";
        case FuncOpType::kOP_NOT:
            return "!";
        case FuncOpType::kOP_XOR:
            return "^";
        case FuncOpType::kOP_TTERM:
            return "tterm";
        default:
            return "Unknown";
    }
}

enum class PinDirection {
    kInout,
    kInput,
    kInternal,
    kOutput,
    kTristate,
    kFeedthru,
    kUnknown
};
inline std::string toString(PinDirection const &p) {
    switch (p) {
        case PinDirection::kInout:
            return "inout";
        case PinDirection::kInput:
            return "input";
        case PinDirection::kInternal:
            return "internal";
        case PinDirection::kOutput:
            return "output";
        case PinDirection::kTristate:
            return "tristate";
        default:
            return "Unknown";
    }
}
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_COMMONDEF_H_
