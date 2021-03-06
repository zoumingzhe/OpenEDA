/**
 * @file group_enum.h
 * @date 2020-09-16
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
#ifndef SRC_DB_TIMING_TIMINGLIB_GROUP_ENUM_H_
#define SRC_DB_TIMING_TIMINGLIB_GROUP_ENUM_H_

namespace Timinglib {

typedef enum {
    kTIMINGLIB_GROUPENUM_base_curves = 1,
    kTIMINGLIB_GROUPENUM_bundle = 2,
    kTIMINGLIB_GROUPENUM_bus = 3,
    kTIMINGLIB_GROUPENUM_capacitance = 4,
    kTIMINGLIB_GROUPENUM_ccs_lu_table_template = 5,
    kTIMINGLIB_GROUPENUM_ccs_retain_fall = 6,
    kTIMINGLIB_GROUPENUM_ccs_retain_rise = 7,
    kTIMINGLIB_GROUPENUM_ccs_timing_base_curve = 8,
    kTIMINGLIB_GROUPENUM_ccs_timing_base_curve_template = 9,
    kTIMINGLIB_GROUPENUM_ccsn_first_stage = 10,
    kTIMINGLIB_GROUPENUM_ccsn_last_stage = 11,
    kTIMINGLIB_GROUPENUM_cell = 12,
    kTIMINGLIB_GROUPENUM_cell_degradation = 13,
    kTIMINGLIB_GROUPENUM_cell_fall = 14,
    kTIMINGLIB_GROUPENUM_cell_fall_to_pct = 15,
    kTIMINGLIB_GROUPENUM_cell_rise = 16,
    kTIMINGLIB_GROUPENUM_cell_rise_to_pct = 17,
    kTIMINGLIB_GROUPENUM_char_config = 18,
    kTIMINGLIB_GROUPENUM_clear_condition = 19,
    kTIMINGLIB_GROUPENUM_clock_condition = 20,
    kTIMINGLIB_GROUPENUM_compact_ccs_fall = 21,
    kTIMINGLIB_GROUPENUM_compact_ccs_power = 22,
    kTIMINGLIB_GROUPENUM_compact_ccs_retain_fall = 23,
    kTIMINGLIB_GROUPENUM_compact_ccs_retain_rise = 24,
    kTIMINGLIB_GROUPENUM_compact_ccs_rise = 25,
    kTIMINGLIB_GROUPENUM_compact_lut_template = 26,
    kTIMINGLIB_GROUPENUM_compressed_ccs_timing_rise = 27,
    kTIMINGLIB_GROUPENUM_dc_current = 28,
    kTIMINGLIB_GROUPENUM_default_soft_error_rate = 29,
    kTIMINGLIB_GROUPENUM_domain = 30,
    kTIMINGLIB_GROUPENUM_dynamic_current = 31,
    kTIMINGLIB_GROUPENUM_electromigration = 32,
    kTIMINGLIB_GROUPENUM_em_lut_template = 33,
    kTIMINGLIB_GROUPENUM_em_max_toggle_rate = 34,
    kTIMINGLIB_GROUPENUM_fall_capacitance = 35,
    kTIMINGLIB_GROUPENUM_fall_capacitance_range = 36,
    kTIMINGLIB_GROUPENUM_fall_constraint = 37,
    kTIMINGLIB_GROUPENUM_fall_net_delay = 38,
    kTIMINGLIB_GROUPENUM_fall_power = 39,
    kTIMINGLIB_GROUPENUM_fall_propagation = 40,
    kTIMINGLIB_GROUPENUM_fall_transition = 41,
    kTIMINGLIB_GROUPENUM_fall_transition_degradation = 42,
    kTIMINGLIB_GROUPENUM_ff = 43,
    kTIMINGLIB_GROUPENUM_ff_bank = 44,
    kTIMINGLIB_GROUPENUM_gate_leakage = 45,
    kTIMINGLIB_GROUPENUM_generated_clock = 46,
    kTIMINGLIB_GROUPENUM_hyperbolic_noise_above_high = 47,
    kTIMINGLIB_GROUPENUM_hyperbolic_noise_below_low = 48,
    kTIMINGLIB_GROUPENUM_hyperbolic_noise_high = 49,
    kTIMINGLIB_GROUPENUM_hyperbolic_noise_low = 50,
    kTIMINGLIB_GROUPENUM_init_current = 51,
    kTIMINGLIB_GROUPENUM_input_ccb = 52,
    kTIMINGLIB_GROUPENUM_input_signal_swing = 53,
    kTIMINGLIB_GROUPENUM_input_voltage = 54,
    kTIMINGLIB_GROUPENUM_interconnect_delay = 55,
    kTIMINGLIB_GROUPENUM_internal_power = 56,
    kTIMINGLIB_GROUPENUM_intrinsic_capacitance = 57,
    kTIMINGLIB_GROUPENUM_intrinsic_parasitic = 58,
    kTIMINGLIB_GROUPENUM_intrinsic_resistance = 59,
    kTIMINGLIB_GROUPENUM_iv_lut_template = 60,
    kTIMINGLIB_GROUPENUM_latch = 61,
    kTIMINGLIB_GROUPENUM_latch_bank = 62,
    kTIMINGLIB_GROUPENUM_leakage_current = 63,
    kTIMINGLIB_GROUPENUM_leakage_power = 64,
    kTIMINGLIB_GROUPENUM_left_id = 65,
    kTIMINGLIB_GROUPENUM_library = 66,
    kTIMINGLIB_GROUPENUM_lower = 67,
    kTIMINGLIB_GROUPENUM_lu_table_template = 68,
    kTIMINGLIB_GROUPENUM_lut = 69,
    kTIMINGLIB_GROUPENUM_lut_values = 70,
    kTIMINGLIB_GROUPENUM_max_cap = 71,
    kTIMINGLIB_GROUPENUM_max_trans = 72,
    kTIMINGLIB_GROUPENUM_maxcap_lut_template = 73,
    kTIMINGLIB_GROUPENUM_maxtrans_lut_template = 74,
    kTIMINGLIB_GROUPENUM_memory = 75,
    kTIMINGLIB_GROUPENUM_memory_read = 76,
    kTIMINGLIB_GROUPENUM_memory_write = 77,
    kTIMINGLIB_GROUPENUM_min_pulse_width = 78,
    kTIMINGLIB_GROUPENUM_minimum_period = 79,
    kTIMINGLIB_GROUPENUM_mode_definition = 80,
    kTIMINGLIB_GROUPENUM_mode_value = 81,
    kTIMINGLIB_GROUPENUM_model = 82,
    kTIMINGLIB_GROUPENUM_noise_immunity_above_high = 83,
    kTIMINGLIB_GROUPENUM_noise_immunity_below_low = 84,
    kTIMINGLIB_GROUPENUM_noise_immunity_high = 85,
    kTIMINGLIB_GROUPENUM_noise_immunity_low = 86,
    kTIMINGLIB_GROUPENUM_noise_lut_template = 87,
    kTIMINGLIB_GROUPENUM_normalized_driver_waveform = 88,
    kTIMINGLIB_GROUPENUM_ocv_derate = 89,
    kTIMINGLIB_GROUPENUM_ocv_derate_factors = 90,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_cell_fall = 91,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_cell_rise = 92,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_fall_constraint = 93,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_fall_transition = 94,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_retain_fall_slew = 95,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_retain_rise_slew = 96,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_retaining_fall = 97,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_retaining_rise = 98,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_rise_constraint = 99,
    kTIMINGLIB_GROUPENUM_ocv_mean_shift_rise_transition = 100,
    kTIMINGLIB_GROUPENUM_ocv_sigma_fall_constraint = 101,
    kTIMINGLIB_GROUPENUM_ocv_sigma_retain_fall_slew = 102,
    kTIMINGLIB_GROUPENUM_ocv_sigma_retain_rise_slew = 103,
    kTIMINGLIB_GROUPENUM_ocv_sigma_retaining_fall = 104,
    kTIMINGLIB_GROUPENUM_ocv_sigma_retaining_rise = 105,
    kTIMINGLIB_GROUPENUM_ocv_sigma_rise_constraint = 106,
    kTIMINGLIB_GROUPENUM_ocv_skewness_cell_fall = 107,
    kTIMINGLIB_GROUPENUM_ocv_skewness_cell_rise = 108,
    kTIMINGLIB_GROUPENUM_ocv_skewness_fall_constraint = 109,
    kTIMINGLIB_GROUPENUM_ocv_skewness_fall_transition = 110,
    kTIMINGLIB_GROUPENUM_ocv_skewness_retain_fall_slew = 111,
    kTIMINGLIB_GROUPENUM_ocv_skewness_retain_rise_slew = 112,
    kTIMINGLIB_GROUPENUM_ocv_skewness_retaining_fall = 113,
    kTIMINGLIB_GROUPENUM_ocv_skewness_retaining_rise = 114,
    kTIMINGLIB_GROUPENUM_ocv_skewness_rise_constraint = 115,
    kTIMINGLIB_GROUPENUM_ocv_skewness_rise_transition = 116,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_cell_fall = 117,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_cell_rise = 118,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_fall_constraint = 119,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_fall_transition = 120,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_retain_fall_slew = 121,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_retain_rise_slew = 122,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_retaining_fall = 123,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_retaining_rise = 124,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_rise_constraint = 125,
    kTIMINGLIB_GROUPENUM_ocv_std_dev_rise_transition = 126,
    kTIMINGLIB_GROUPENUM_ocv_table_template = 127,
    kTIMINGLIB_GROUPENUM_operating_conditions = 128,
    kTIMINGLIB_GROUPENUM_output_ccb = 129,
    kTIMINGLIB_GROUPENUM_output_current_fall = 130,
    kTIMINGLIB_GROUPENUM_output_current_rise = 131,
    kTIMINGLIB_GROUPENUM_output_current_template = 132,
    kTIMINGLIB_GROUPENUM_output_signal_swing = 133,
    kTIMINGLIB_GROUPENUM_output_voltage = 134,
    kTIMINGLIB_GROUPENUM_output_voltage_fall = 135,
    kTIMINGLIB_GROUPENUM_output_voltage_rise = 136,
    kTIMINGLIB_GROUPENUM_peak_current = 137,
    kTIMINGLIB_GROUPENUM_peak_time = 138,
    kTIMINGLIB_GROUPENUM_peak_voltage = 139,
    kTIMINGLIB_GROUPENUM_pg_current = 140,
    kTIMINGLIB_GROUPENUM_pg_current_template = 141,
    kTIMINGLIB_GROUPENUM_pg_pin = 142,
    kTIMINGLIB_GROUPENUM_pg_setting_definition = 143,
    kTIMINGLIB_GROUPENUM_pg_setting_transition = 144,
    kTIMINGLIB_GROUPENUM_pg_setting_value = 145,
    kTIMINGLIB_GROUPENUM_pin = 146,
    kTIMINGLIB_GROUPENUM_pin_based_variation = 147,
    kTIMINGLIB_GROUPENUM_pin_capacitance = 148,
    kTIMINGLIB_GROUPENUM_poly_template = 149,
    kTIMINGLIB_GROUPENUM_power = 150,
    kTIMINGLIB_GROUPENUM_power_lut_template = 151,
    kTIMINGLIB_GROUPENUM_power_poly_template = 152,
    kTIMINGLIB_GROUPENUM_power_supply = 153,
    kTIMINGLIB_GROUPENUM_preset_condition = 154,
    kTIMINGLIB_GROUPENUM_propagated_noise_height_above_high = 155,
    kTIMINGLIB_GROUPENUM_propagated_noise_height_below_low = 156,
    kTIMINGLIB_GROUPENUM_propagated_noise_height_high = 157,
    kTIMINGLIB_GROUPENUM_propagated_noise_height_low = 158,
    kTIMINGLIB_GROUPENUM_propagated_noise_high = 159,
    kTIMINGLIB_GROUPENUM_propagated_noise_low = 160,
    kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_above_high = 161,
    kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_below_low = 162,
    kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_high = 163,
    kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_low = 164,
    kTIMINGLIB_GROUPENUM_propagated_noise_width_above_high = 165,
    kTIMINGLIB_GROUPENUM_propagated_noise_width_below_low = 166,
    kTIMINGLIB_GROUPENUM_propagated_noise_width_high = 167,
    kTIMINGLIB_GROUPENUM_propagated_noise_width_low = 168,
    kTIMINGLIB_GROUPENUM_propagation_lut_template = 169,
    kTIMINGLIB_GROUPENUM_receiver_capacitance = 170,
    kTIMINGLIB_GROUPENUM_receiver_capacitance1_fall = 171,
    kTIMINGLIB_GROUPENUM_receiver_capacitance1_rise = 172,
    kTIMINGLIB_GROUPENUM_receiver_capacitance2_fall = 173,
    kTIMINGLIB_GROUPENUM_receiver_capacitance2_rise = 174,
    kTIMINGLIB_GROUPENUM_receiver_capacitance_fall = 175,
    kTIMINGLIB_GROUPENUM_receiver_capacitance_rise = 176,
    kTIMINGLIB_GROUPENUM_retain_fall_slew = 177,
    kTIMINGLIB_GROUPENUM_retain_rise_slew = 178,
    kTIMINGLIB_GROUPENUM_retaining_fall = 179,
    kTIMINGLIB_GROUPENUM_retaining_rise = 180,
    kTIMINGLIB_GROUPENUM_retention_condition = 181,
    kTIMINGLIB_GROUPENUM_right_id = 182,
    kTIMINGLIB_GROUPENUM_rise_capacitance = 183,
    kTIMINGLIB_GROUPENUM_rise_capacitance_range = 184,
    kTIMINGLIB_GROUPENUM_rise_constraint = 185,
    kTIMINGLIB_GROUPENUM_rise_net_delay = 186,
    kTIMINGLIB_GROUPENUM_rise_power = 187,
    kTIMINGLIB_GROUPENUM_rise_propagation = 188,
    kTIMINGLIB_GROUPENUM_rise_transition = 189,
    kTIMINGLIB_GROUPENUM_rise_transition_degradation = 190,
    kTIMINGLIB_GROUPENUM_routing_track = 191,
    kTIMINGLIB_GROUPENUM_scaled_cell = 192,
    kTIMINGLIB_GROUPENUM_scaling_factors = 193,
    kTIMINGLIB_GROUPENUM_sensitization = 194,
    kTIMINGLIB_GROUPENUM_seq = 195,
    kTIMINGLIB_GROUPENUM_seq_bank = 196,
    kTIMINGLIB_GROUPENUM_soft_error_rate = 197,
    kTIMINGLIB_GROUPENUM_soft_error_rate_template = 198,
    kTIMINGLIB_GROUPENUM_state = 199,
    kTIMINGLIB_GROUPENUM_statetable = 200,
    kTIMINGLIB_GROUPENUM_steady_state_current_high = 201,
    kTIMINGLIB_GROUPENUM_steady_state_current_low = 202,
    kTIMINGLIB_GROUPENUM_steady_state_current_tristate = 203,
    kTIMINGLIB_GROUPENUM_switching_group = 204,
    kTIMINGLIB_GROUPENUM_test_cell = 205,
    kTIMINGLIB_GROUPENUM_timing = 206,
    kTIMINGLIB_GROUPENUM_timing_based_variation = 207,
    kTIMINGLIB_GROUPENUM_timing_range = 208,
    kTIMINGLIB_GROUPENUM_tlatch = 209,
    kTIMINGLIB_GROUPENUM_total_capacitance = 210,
    kTIMINGLIB_GROUPENUM_type = 211,
    kTIMINGLIB_GROUPENUM_upper = 212,
    kTIMINGLIB_GROUPENUM_user_parameters = 213,
    kTIMINGLIB_GROUPENUM_va_compact_ccs_fall = 214,
    kTIMINGLIB_GROUPENUM_va_compact_ccs_retain_fall = 215,
    kTIMINGLIB_GROUPENUM_va_compact_ccs_retain_rise = 216,
    kTIMINGLIB_GROUPENUM_va_compact_ccs_rise = 217,
    kTIMINGLIB_GROUPENUM_va_fall_constraint = 218,
    kTIMINGLIB_GROUPENUM_va_receiver_capacitance1_fall = 219,
    kTIMINGLIB_GROUPENUM_va_receiver_capacitance1_rise = 220,
    kTIMINGLIB_GROUPENUM_va_receiver_capacitance2_fall = 221,
    kTIMINGLIB_GROUPENUM_va_receiver_capacitance2_rise = 222,
    kTIMINGLIB_GROUPENUM_va_rise_constraint = 223,
    kTIMINGLIB_GROUPENUM_vector = 224,
    kTIMINGLIB_GROUPENUM_voltage_state_range_list = 225,
    kTIMINGLIB_GROUPENUM_wire_load = 226,
    kTIMINGLIB_GROUPENUM_wire_load_selection = 227,
    kTIMINGLIB_GROUPENUM_wire_load_table = 228,
} group_enum;

}  // namespace Timinglib

#endif  // SRC_DB_TIMING_TIMINGLIB_GROUP_ENUM_H_
