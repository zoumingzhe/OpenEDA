/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -L ANSI-C -t -H group_hash_func -N lookup_group_name -C -I -k '*' -D group_lookup  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "group_lookup"

#include "group_enum.h"

using namespace Timinglib;
#line 6 "group_lookup"
struct libGroupMap { const char *name; group_enum type; };
#include <string.h>

#define TOTAL_KEYWORDS 228
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 43
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 1256
/* maximum key range = 1255, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
group_hash_func (register const char *str, register size_t len)
{
  static const unsigned short asso_values[] =
    {
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,   30,
        15, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257,   20, 1257,    0,  120,   10,
        70,    5,    0,   50,   99,    0, 1257,  215,    0,   95,
         0,    0,   10,   15,    0,    0,    5,  185,    5,  230,
         5,   24,    0, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257,
      1257, 1257, 1257, 1257, 1257, 1257
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[42]];
      /*FALLTHROUGH*/
      case 42:
        hval += asso_values[(unsigned char)str[41]];
      /*FALLTHROUGH*/
      case 41:
        hval += asso_values[(unsigned char)str[40]];
      /*FALLTHROUGH*/
      case 40:
        hval += asso_values[(unsigned char)str[39]];
      /*FALLTHROUGH*/
      case 39:
        hval += asso_values[(unsigned char)str[38]];
      /*FALLTHROUGH*/
      case 38:
        hval += asso_values[(unsigned char)str[37]];
      /*FALLTHROUGH*/
      case 37:
        hval += asso_values[(unsigned char)str[36]];
      /*FALLTHROUGH*/
      case 36:
        hval += asso_values[(unsigned char)str[35]];
      /*FALLTHROUGH*/
      case 35:
        hval += asso_values[(unsigned char)str[34]];
      /*FALLTHROUGH*/
      case 34:
        hval += asso_values[(unsigned char)str[33]];
      /*FALLTHROUGH*/
      case 33:
        hval += asso_values[(unsigned char)str[32]];
      /*FALLTHROUGH*/
      case 32:
        hval += asso_values[(unsigned char)str[31]];
      /*FALLTHROUGH*/
      case 31:
        hval += asso_values[(unsigned char)str[30]];
      /*FALLTHROUGH*/
      case 30:
        hval += asso_values[(unsigned char)str[29]];
      /*FALLTHROUGH*/
      case 29:
        hval += asso_values[(unsigned char)str[28]];
      /*FALLTHROUGH*/
      case 28:
        hval += asso_values[(unsigned char)str[27]];
      /*FALLTHROUGH*/
      case 27:
        hval += asso_values[(unsigned char)str[26]];
      /*FALLTHROUGH*/
      case 26:
        hval += asso_values[(unsigned char)str[25]];
      /*FALLTHROUGH*/
      case 25:
        hval += asso_values[(unsigned char)str[24]];
      /*FALLTHROUGH*/
      case 24:
        hval += asso_values[(unsigned char)str[23]];
      /*FALLTHROUGH*/
      case 23:
        hval += asso_values[(unsigned char)str[22]];
      /*FALLTHROUGH*/
      case 22:
        hval += asso_values[(unsigned char)str[21]];
      /*FALLTHROUGH*/
      case 21:
        hval += asso_values[(unsigned char)str[20]];
      /*FALLTHROUGH*/
      case 20:
        hval += asso_values[(unsigned char)str[19]];
      /*FALLTHROUGH*/
      case 19:
        hval += asso_values[(unsigned char)str[18]];
      /*FALLTHROUGH*/
      case 18:
        hval += asso_values[(unsigned char)str[17]];
      /*FALLTHROUGH*/
      case 17:
        hval += asso_values[(unsigned char)str[16]];
      /*FALLTHROUGH*/
      case 16:
        hval += asso_values[(unsigned char)str[15]];
      /*FALLTHROUGH*/
      case 15:
        hval += asso_values[(unsigned char)str[14]];
      /*FALLTHROUGH*/
      case 14:
        hval += asso_values[(unsigned char)str[13]];
      /*FALLTHROUGH*/
      case 13:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
      case 11:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct libGroupMap *
lookup_group_name (register const char *str, register size_t len)
{
  static const struct libGroupMap wordlist[] =
    {
#line 50 "group_lookup"
      {"ff",	kTIMINGLIB_GROUPENUM_ff},
#line 153 "group_lookup"
      {"pin",	kTIMINGLIB_GROUPENUM_pin},
#line 19 "group_lookup"
      {"cell",	kTIMINGLIB_GROUPENUM_cell},
#line 206 "group_lookup"
      {"state",	kTIMINGLIB_GROUPENUM_state},
#line 202 "group_lookup"
      {"seq",	kTIMINGLIB_GROUPENUM_seq},
#line 201 "group_lookup"
      {"sensitization",	kTIMINGLIB_GROUPENUM_sensitization},
#line 231 "group_lookup"
      {"vector",	kTIMINGLIB_GROUPENUM_vector},
#line 21 "group_lookup"
      {"cell_fall",	kTIMINGLIB_GROUPENUM_cell_fall},
#line 48 "group_lookup"
      {"fall_transition",	kTIMINGLIB_GROUPENUM_fall_transition},
#line 218 "group_lookup"
      {"type",	kTIMINGLIB_GROUPENUM_type},
#line 23 "group_lookup"
      {"cell_rise",	kTIMINGLIB_GROUPENUM_cell_rise},
#line 196 "group_lookup"
      {"rise_transition",	kTIMINGLIB_GROUPENUM_rise_transition},
#line 44 "group_lookup"
      {"fall_constraint",	kTIMINGLIB_GROUPENUM_fall_constraint},
#line 212 "group_lookup"
      {"test_cell",	kTIMINGLIB_GROUPENUM_test_cell},
#line 192 "group_lookup"
      {"rise_constraint",	kTIMINGLIB_GROUPENUM_rise_constraint},
#line 11 "group_lookup"
      {"capacitance",	kTIMINGLIB_GROUPENUM_capacitance},
#line 204 "group_lookup"
      {"soft_error_rate",	kTIMINGLIB_GROUPENUM_soft_error_rate},
#line 65 "group_lookup"
      {"intrinsic_parasitic",	kTIMINGLIB_GROUPENUM_intrinsic_parasitic},
#line 66 "group_lookup"
      {"intrinsic_resistance",	kTIMINGLIB_GROUPENUM_intrinsic_resistance},
#line 225 "group_lookup"
      {"va_fall_constraint",	kTIMINGLIB_GROUPENUM_va_fall_constraint},
#line 13 "group_lookup"
      {"ccs_retain_fall",	kTIMINGLIB_GROUPENUM_ccs_retain_fall},
#line 42 "group_lookup"
      {"fall_capacitance",	kTIMINGLIB_GROUPENUM_fall_capacitance},
#line 230 "group_lookup"
      {"va_rise_constraint",	kTIMINGLIB_GROUPENUM_va_rise_constraint},
#line 14 "group_lookup"
      {"ccs_retain_rise",	kTIMINGLIB_GROUPENUM_ccs_retain_rise},
#line 190 "group_lookup"
      {"rise_capacitance",	kTIMINGLIB_GROUPENUM_rise_capacitance},
#line 186 "group_lookup"
      {"retaining_fall",	kTIMINGLIB_GROUPENUM_retaining_fall},
#line 155 "group_lookup"
      {"pin_capacitance",	kTIMINGLIB_GROUPENUM_pin_capacitance},
#line 149 "group_lookup"
      {"pg_pin",	kTIMINGLIB_GROUPENUM_pg_pin},
#line 217 "group_lookup"
      {"total_capacitance",	kTIMINGLIB_GROUPENUM_total_capacitance},
#line 187 "group_lookup"
      {"retaining_rise",	kTIMINGLIB_GROUPENUM_retaining_rise},
#line 64 "group_lookup"
      {"intrinsic_capacitance",	kTIMINGLIB_GROUPENUM_intrinsic_capacitance},
#line 72 "group_lookup"
      {"left_id",	kTIMINGLIB_GROUPENUM_left_id},
#line 200 "group_lookup"
      {"scaling_factors",	kTIMINGLIB_GROUPENUM_scaling_factors},
#line 47 "group_lookup"
      {"fall_propagation",	kTIMINGLIB_GROUPENUM_fall_propagation},
#line 195 "group_lookup"
      {"rise_propagation",	kTIMINGLIB_GROUPENUM_rise_propagation},
#line 68 "group_lookup"
      {"latch",	kTIMINGLIB_GROUPENUM_latch},
#line 177 "group_lookup"
      {"receiver_capacitance",	kTIMINGLIB_GROUPENUM_receiver_capacitance},
#line 22 "group_lookup"
      {"cell_fall_to_pct",	kTIMINGLIB_GROUPENUM_cell_fall_to_pct},
#line 216 "group_lookup"
      {"tlatch",	kTIMINGLIB_GROUPENUM_tlatch},
#line 24 "group_lookup"
      {"cell_rise_to_pct",	kTIMINGLIB_GROUPENUM_cell_rise_to_pct},
#line 96 "group_lookup"
      {"ocv_derate",	kTIMINGLIB_GROUPENUM_ocv_derate},
#line 199 "group_lookup"
      {"scaled_cell",	kTIMINGLIB_GROUPENUM_scaled_cell},
#line 79 "group_lookup"
      {"max_trans",	kTIMINGLIB_GROUPENUM_max_trans},
#line 26 "group_lookup"
      {"clear_condition",	kTIMINGLIB_GROUPENUM_clear_condition},
#line 18 "group_lookup"
      {"ccsn_last_stage",	kTIMINGLIB_GROUPENUM_ccsn_last_stage},
#line 17 "group_lookup"
      {"ccsn_first_stage",	kTIMINGLIB_GROUPENUM_ccsn_first_stage},
#line 188 "group_lookup"
      {"retention_condition",	kTIMINGLIB_GROUPENUM_retention_condition},
#line 182 "group_lookup"
      {"receiver_capacitance_fall",	kTIMINGLIB_GROUPENUM_receiver_capacitance_fall},
#line 161 "group_lookup"
      {"preset_condition",	kTIMINGLIB_GROUPENUM_preset_condition},
#line 78 "group_lookup"
      {"max_cap",	kTIMINGLIB_GROUPENUM_max_cap},
#line 183 "group_lookup"
      {"receiver_capacitance_rise",	kTIMINGLIB_GROUPENUM_receiver_capacitance_rise},
#line 73 "group_lookup"
      {"library",	kTIMINGLIB_GROUPENUM_library},
#line 207 "group_lookup"
      {"statetable",	kTIMINGLIB_GROUPENUM_statetable},
#line 213 "group_lookup"
      {"timing",	kTIMINGLIB_GROUPENUM_timing},
#line 180 "group_lookup"
      {"receiver_capacitance2_fall",	kTIMINGLIB_GROUPENUM_receiver_capacitance2_fall},
#line 45 "group_lookup"
      {"fall_net_delay",	kTIMINGLIB_GROUPENUM_fall_net_delay},
#line 181 "group_lookup"
      {"receiver_capacitance2_rise",	kTIMINGLIB_GROUPENUM_receiver_capacitance2_rise},
#line 43 "group_lookup"
      {"fall_capacitance_range",	kTIMINGLIB_GROUPENUM_fall_capacitance_range},
#line 193 "group_lookup"
      {"rise_net_delay",	kTIMINGLIB_GROUPENUM_rise_net_delay},
#line 37 "group_lookup"
      {"domain",	kTIMINGLIB_GROUPENUM_domain},
#line 191 "group_lookup"
      {"rise_capacitance_range",	kTIMINGLIB_GROUPENUM_rise_capacitance_range},
#line 97 "group_lookup"
      {"ocv_derate_factors",	kTIMINGLIB_GROUPENUM_ocv_derate_factors},
#line 89 "group_lookup"
      {"model",	kTIMINGLIB_GROUPENUM_model},
#line 178 "group_lookup"
      {"receiver_capacitance1_fall",	kTIMINGLIB_GROUPENUM_receiver_capacitance1_fall},
#line 62 "group_lookup"
      {"interconnect_delay",	kTIMINGLIB_GROUPENUM_interconnect_delay},
#line 179 "group_lookup"
      {"receiver_capacitance1_rise",	kTIMINGLIB_GROUPENUM_receiver_capacitance1_rise},
#line 228 "group_lookup"
      {"va_receiver_capacitance2_fall",	kTIMINGLIB_GROUPENUM_va_receiver_capacitance2_fall},
#line 39 "group_lookup"
      {"electromigration",	kTIMINGLIB_GROUPENUM_electromigration},
#line 156 "group_lookup"
      {"poly_template",	kTIMINGLIB_GROUPENUM_poly_template},
#line 76 "group_lookup"
      {"lut",	kTIMINGLIB_GROUPENUM_lut},
#line 229 "group_lookup"
      {"va_receiver_capacitance2_rise",	kTIMINGLIB_GROUPENUM_va_receiver_capacitance2_rise},
#line 135 "group_lookup"
      {"operating_conditions",	kTIMINGLIB_GROUPENUM_operating_conditions},
#line 151 "group_lookup"
      {"pg_setting_transition",	kTIMINGLIB_GROUPENUM_pg_setting_transition},
#line 25 "group_lookup"
      {"char_config",	kTIMINGLIB_GROUPENUM_char_config},
#line 226 "group_lookup"
      {"va_receiver_capacitance1_fall",	kTIMINGLIB_GROUPENUM_va_receiver_capacitance1_fall},
#line 28 "group_lookup"
      {"compact_ccs_fall",	kTIMINGLIB_GROUPENUM_compact_ccs_fall},
#line 227 "group_lookup"
      {"va_receiver_capacitance1_rise",	kTIMINGLIB_GROUPENUM_va_receiver_capacitance1_rise},
#line 32 "group_lookup"
      {"compact_ccs_rise",	kTIMINGLIB_GROUPENUM_compact_ccs_rise},
#line 219 "group_lookup"
      {"upper",	kTIMINGLIB_GROUPENUM_upper},
#line 232 "group_lookup"
      {"voltage_state_range_list",	kTIMINGLIB_GROUPENUM_voltage_state_range_list},
#line 82 "group_lookup"
      {"memory",	kTIMINGLIB_GROUPENUM_memory},
#line 205 "group_lookup"
      {"soft_error_rate_template",	kTIMINGLIB_GROUPENUM_soft_error_rate_template},
#line 221 "group_lookup"
      {"va_compact_ccs_fall",	kTIMINGLIB_GROUPENUM_va_compact_ccs_fall},
#line 215 "group_lookup"
      {"timing_range",	kTIMINGLIB_GROUPENUM_timing_range},
#line 224 "group_lookup"
      {"va_compact_ccs_rise",	kTIMINGLIB_GROUPENUM_va_compact_ccs_rise},
#line 74 "group_lookup"
      {"lower",	kTIMINGLIB_GROUPENUM_lower},
#line 58 "group_lookup"
      {"init_current",	kTIMINGLIB_GROUPENUM_init_current},
#line 30 "group_lookup"
      {"compact_ccs_retain_fall",	kTIMINGLIB_GROUPENUM_compact_ccs_retain_fall},
#line 31 "group_lookup"
      {"compact_ccs_retain_rise",	kTIMINGLIB_GROUPENUM_compact_ccs_retain_rise},
#line 157 "group_lookup"
      {"power",	kTIMINGLIB_GROUPENUM_power},
#line 20 "group_lookup"
      {"cell_degradation",	kTIMINGLIB_GROUPENUM_cell_degradation},
#line 189 "group_lookup"
      {"right_id",	kTIMINGLIB_GROUPENUM_right_id},
#line 108 "group_lookup"
      {"ocv_sigma_fall_constraint",	kTIMINGLIB_GROUPENUM_ocv_sigma_fall_constraint},
#line 150 "group_lookup"
      {"pg_setting_definition",	kTIMINGLIB_GROUPENUM_pg_setting_definition},
#line 113 "group_lookup"
      {"ocv_sigma_rise_constraint",	kTIMINGLIB_GROUPENUM_ocv_sigma_rise_constraint},
#line 222 "group_lookup"
      {"va_compact_ccs_retain_fall",	kTIMINGLIB_GROUPENUM_va_compact_ccs_retain_fall},
#line 154 "group_lookup"
      {"pin_based_variation",	kTIMINGLIB_GROUPENUM_pin_based_variation},
#line 46 "group_lookup"
      {"fall_power",	kTIMINGLIB_GROUPENUM_fall_power},
#line 223 "group_lookup"
      {"va_compact_ccs_retain_rise",	kTIMINGLIB_GROUPENUM_va_compact_ccs_retain_rise},
#line 49 "group_lookup"
      {"fall_transition_degradation",	kTIMINGLIB_GROUPENUM_fall_transition_degradation},
#line 194 "group_lookup"
      {"rise_power",	kTIMINGLIB_GROUPENUM_rise_power},
#line 197 "group_lookup"
      {"rise_transition_degradation",	kTIMINGLIB_GROUPENUM_rise_transition_degradation},
#line 87 "group_lookup"
      {"mode_definition",	kTIMINGLIB_GROUPENUM_mode_definition},
#line 124 "group_lookup"
      {"ocv_std_dev_cell_fall",	kTIMINGLIB_GROUPENUM_ocv_std_dev_cell_fall},
#line 127 "group_lookup"
      {"ocv_std_dev_fall_transition",	kTIMINGLIB_GROUPENUM_ocv_std_dev_fall_transition},
#line 63 "group_lookup"
      {"internal_power",	kTIMINGLIB_GROUPENUM_internal_power},
#line 125 "group_lookup"
      {"ocv_std_dev_cell_rise",	kTIMINGLIB_GROUPENUM_ocv_std_dev_cell_rise},
#line 133 "group_lookup"
      {"ocv_std_dev_rise_transition",	kTIMINGLIB_GROUPENUM_ocv_std_dev_rise_transition},
#line 147 "group_lookup"
      {"pg_current",	kTIMINGLIB_GROUPENUM_pg_current},
#line 126 "group_lookup"
      {"ocv_std_dev_fall_constraint",	kTIMINGLIB_GROUPENUM_ocv_std_dev_fall_constraint},
#line 61 "group_lookup"
      {"input_voltage",	kTIMINGLIB_GROUPENUM_input_voltage},
#line 184 "group_lookup"
      {"retain_fall_slew",	kTIMINGLIB_GROUPENUM_retain_fall_slew},
#line 132 "group_lookup"
      {"ocv_std_dev_rise_constraint",	kTIMINGLIB_GROUPENUM_ocv_std_dev_rise_constraint},
#line 111 "group_lookup"
      {"ocv_sigma_retaining_fall",	kTIMINGLIB_GROUPENUM_ocv_sigma_retaining_fall},
#line 185 "group_lookup"
      {"retain_rise_slew",	kTIMINGLIB_GROUPENUM_retain_rise_slew},
#line 10 "group_lookup"
      {"bus",	kTIMINGLIB_GROUPENUM_bus},
#line 112 "group_lookup"
      {"ocv_sigma_retaining_rise",	kTIMINGLIB_GROUPENUM_ocv_sigma_retaining_rise},
#line 35 "group_lookup"
      {"dc_current",	kTIMINGLIB_GROUPENUM_dc_current},
#line 83 "group_lookup"
      {"memory_read",	kTIMINGLIB_GROUPENUM_memory_read},
#line 146 "group_lookup"
      {"peak_voltage",	kTIMINGLIB_GROUPENUM_peak_voltage},
#line 134 "group_lookup"
      {"ocv_table_template",	kTIMINGLIB_GROUPENUM_ocv_table_template},
#line 233 "group_lookup"
      {"wire_load",	kTIMINGLIB_GROUPENUM_wire_load},
#line 130 "group_lookup"
      {"ocv_std_dev_retaining_fall",	kTIMINGLIB_GROUPENUM_ocv_std_dev_retaining_fall},
#line 98 "group_lookup"
      {"ocv_mean_shift_cell_fall",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_cell_fall},
#line 101 "group_lookup"
      {"ocv_mean_shift_fall_transition",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_fall_transition},
#line 131 "group_lookup"
      {"ocv_std_dev_retaining_rise",	kTIMINGLIB_GROUPENUM_ocv_std_dev_retaining_rise},
#line 99 "group_lookup"
      {"ocv_mean_shift_cell_rise",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_cell_rise},
#line 107 "group_lookup"
      {"ocv_mean_shift_rise_transition",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_rise_transition},
#line 220 "group_lookup"
      {"user_parameters",	kTIMINGLIB_GROUPENUM_user_parameters},
#line 100 "group_lookup"
      {"ocv_mean_shift_fall_constraint",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_fall_constraint},
#line 106 "group_lookup"
      {"ocv_mean_shift_rise_constraint",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_rise_constraint},
#line 27 "group_lookup"
      {"clock_condition",	kTIMINGLIB_GROUPENUM_clock_condition},
#line 8 "group_lookup"
      {"base_curves",	kTIMINGLIB_GROUPENUM_base_curves},
#line 51 "group_lookup"
      {"ff_bank",	kTIMINGLIB_GROUPENUM_ff_bank},
#line 145 "group_lookup"
      {"peak_time",	kTIMINGLIB_GROUPENUM_peak_time},
#line 52 "group_lookup"
      {"gate_leakage",	kTIMINGLIB_GROUPENUM_gate_leakage},
#line 36 "group_lookup"
      {"default_soft_error_rate",	kTIMINGLIB_GROUPENUM_default_soft_error_rate},
#line 59 "group_lookup"
      {"input_ccb",	kTIMINGLIB_GROUPENUM_input_ccb},
#line 67 "group_lookup"
      {"iv_lut_template",	kTIMINGLIB_GROUPENUM_iv_lut_template},
#line 152 "group_lookup"
      {"pg_setting_value",	kTIMINGLIB_GROUPENUM_pg_setting_value},
#line 94 "group_lookup"
      {"noise_lut_template",	kTIMINGLIB_GROUPENUM_noise_lut_template},
#line 203 "group_lookup"
      {"seq_bank",	kTIMINGLIB_GROUPENUM_seq_bank},
#line 9 "group_lookup"
      {"bundle",	kTIMINGLIB_GROUPENUM_bundle},
#line 104 "group_lookup"
      {"ocv_mean_shift_retaining_fall",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_retaining_fall},
#line 234 "group_lookup"
      {"wire_load_selection",	kTIMINGLIB_GROUPENUM_wire_load_selection},
#line 105 "group_lookup"
      {"ocv_mean_shift_retaining_rise",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_retaining_rise},
#line 88 "group_lookup"
      {"mode_value",	kTIMINGLIB_GROUPENUM_mode_value},
#line 41 "group_lookup"
      {"em_max_toggle_rate",	kTIMINGLIB_GROUPENUM_em_max_toggle_rate},
#line 53 "group_lookup"
      {"generated_clock",	kTIMINGLIB_GROUPENUM_generated_clock},
#line 77 "group_lookup"
      {"lut_values",	kTIMINGLIB_GROUPENUM_lut_values},
#line 214 "group_lookup"
      {"timing_based_variation",	kTIMINGLIB_GROUPENUM_timing_based_variation},
#line 210 "group_lookup"
      {"steady_state_current_tristate",	kTIMINGLIB_GROUPENUM_steady_state_current_tristate},
#line 38 "group_lookup"
      {"dynamic_current",	kTIMINGLIB_GROUPENUM_dynamic_current},
#line 167 "group_lookup"
      {"propagated_noise_low",	kTIMINGLIB_GROUPENUM_propagated_noise_low},
#line 148 "group_lookup"
      {"pg_current_template",	kTIMINGLIB_GROUPENUM_pg_current_template},
#line 29 "group_lookup"
      {"compact_ccs_power",	kTIMINGLIB_GROUPENUM_compact_ccs_power},
#line 176 "group_lookup"
      {"propagation_lut_template",	kTIMINGLIB_GROUPENUM_propagation_lut_template},
#line 34 "group_lookup"
      {"compressed_ccs_timing_rise",	kTIMINGLIB_GROUPENUM_compressed_ccs_timing_rise},
#line 159 "group_lookup"
      {"power_poly_template",	kTIMINGLIB_GROUPENUM_power_poly_template},
#line 166 "group_lookup"
      {"propagated_noise_high",	kTIMINGLIB_GROUPENUM_propagated_noise_high},
#line 144 "group_lookup"
      {"peak_current",	kTIMINGLIB_GROUPENUM_peak_current},
#line 40 "group_lookup"
      {"em_lut_template",	kTIMINGLIB_GROUPENUM_em_lut_template},
#line 69 "group_lookup"
      {"latch_bank",	kTIMINGLIB_GROUPENUM_latch_bank},
#line 81 "group_lookup"
      {"maxtrans_lut_template",	kTIMINGLIB_GROUPENUM_maxtrans_lut_template},
#line 141 "group_lookup"
      {"output_voltage",	kTIMINGLIB_GROUPENUM_output_voltage},
#line 235 "group_lookup"
      {"wire_load_table",	kTIMINGLIB_GROUPENUM_wire_load_table},
#line 84 "group_lookup"
      {"memory_write",	kTIMINGLIB_GROUPENUM_memory_write},
#line 80 "group_lookup"
      {"maxcap_lut_template",	kTIMINGLIB_GROUPENUM_maxcap_lut_template},
#line 75 "group_lookup"
      {"lu_table_template",	kTIMINGLIB_GROUPENUM_lu_table_template},
#line 198 "group_lookup"
      {"routing_track",	kTIMINGLIB_GROUPENUM_routing_track},
#line 33 "group_lookup"
      {"compact_lut_template",	kTIMINGLIB_GROUPENUM_compact_lut_template},
#line 160 "group_lookup"
      {"power_supply",	kTIMINGLIB_GROUPENUM_power_supply},
#line 109 "group_lookup"
      {"ocv_sigma_retain_fall_slew",	kTIMINGLIB_GROUPENUM_ocv_sigma_retain_fall_slew},
#line 142 "group_lookup"
      {"output_voltage_fall",	kTIMINGLIB_GROUPENUM_output_voltage_fall},
#line 70 "group_lookup"
      {"leakage_current",	kTIMINGLIB_GROUPENUM_leakage_current},
#line 110 "group_lookup"
      {"ocv_sigma_retain_rise_slew",	kTIMINGLIB_GROUPENUM_ocv_sigma_retain_rise_slew},
#line 143 "group_lookup"
      {"output_voltage_rise",	kTIMINGLIB_GROUPENUM_output_voltage_rise},
#line 12 "group_lookup"
      {"ccs_lu_table_template",	kTIMINGLIB_GROUPENUM_ccs_lu_table_template},
#line 128 "group_lookup"
      {"ocv_std_dev_retain_fall_slew",	kTIMINGLIB_GROUPENUM_ocv_std_dev_retain_fall_slew},
#line 129 "group_lookup"
      {"ocv_std_dev_retain_rise_slew",	kTIMINGLIB_GROUPENUM_ocv_std_dev_retain_rise_slew},
#line 71 "group_lookup"
      {"leakage_power",	kTIMINGLIB_GROUPENUM_leakage_power},
#line 136 "group_lookup"
      {"output_ccb",	kTIMINGLIB_GROUPENUM_output_ccb},
#line 57 "group_lookup"
      {"hyperbolic_noise_low",	kTIMINGLIB_GROUPENUM_hyperbolic_noise_low},
#line 114 "group_lookup"
      {"ocv_skewness_cell_fall",	kTIMINGLIB_GROUPENUM_ocv_skewness_cell_fall},
#line 117 "group_lookup"
      {"ocv_skewness_fall_transition",	kTIMINGLIB_GROUPENUM_ocv_skewness_fall_transition},
#line 115 "group_lookup"
      {"ocv_skewness_cell_rise",	kTIMINGLIB_GROUPENUM_ocv_skewness_cell_rise},
#line 123 "group_lookup"
      {"ocv_skewness_rise_transition",	kTIMINGLIB_GROUPENUM_ocv_skewness_rise_transition},
#line 116 "group_lookup"
      {"ocv_skewness_fall_constraint",	kTIMINGLIB_GROUPENUM_ocv_skewness_fall_constraint},
#line 15 "group_lookup"
      {"ccs_timing_base_curve",	kTIMINGLIB_GROUPENUM_ccs_timing_base_curve},
#line 56 "group_lookup"
      {"hyperbolic_noise_high",	kTIMINGLIB_GROUPENUM_hyperbolic_noise_high},
#line 122 "group_lookup"
      {"ocv_skewness_rise_constraint",	kTIMINGLIB_GROUPENUM_ocv_skewness_rise_constraint},
#line 60 "group_lookup"
      {"input_signal_swing",	kTIMINGLIB_GROUPENUM_input_signal_swing},
#line 86 "group_lookup"
      {"minimum_period",	kTIMINGLIB_GROUPENUM_minimum_period},
#line 102 "group_lookup"
      {"ocv_mean_shift_retain_fall_slew",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_retain_fall_slew},
#line 103 "group_lookup"
      {"ocv_mean_shift_retain_rise_slew",	kTIMINGLIB_GROUPENUM_ocv_mean_shift_retain_rise_slew},
#line 120 "group_lookup"
      {"ocv_skewness_retaining_fall",	kTIMINGLIB_GROUPENUM_ocv_skewness_retaining_fall},
#line 158 "group_lookup"
      {"power_lut_template",	kTIMINGLIB_GROUPENUM_power_lut_template},
#line 121 "group_lookup"
      {"ocv_skewness_retaining_rise",	kTIMINGLIB_GROUPENUM_ocv_skewness_retaining_rise},
#line 209 "group_lookup"
      {"steady_state_current_low",	kTIMINGLIB_GROUPENUM_steady_state_current_low},
#line 95 "group_lookup"
      {"normalized_driver_waveform",	kTIMINGLIB_GROUPENUM_normalized_driver_waveform},
#line 137 "group_lookup"
      {"output_current_fall",	kTIMINGLIB_GROUPENUM_output_current_fall},
#line 208 "group_lookup"
      {"steady_state_current_high",	kTIMINGLIB_GROUPENUM_steady_state_current_high},
#line 138 "group_lookup"
      {"output_current_rise",	kTIMINGLIB_GROUPENUM_output_current_rise},
#line 211 "group_lookup"
      {"switching_group",	kTIMINGLIB_GROUPENUM_switching_group},
#line 93 "group_lookup"
      {"noise_immunity_low",	kTIMINGLIB_GROUPENUM_noise_immunity_low},
#line 92 "group_lookup"
      {"noise_immunity_high",	kTIMINGLIB_GROUPENUM_noise_immunity_high},
#line 165 "group_lookup"
      {"propagated_noise_height_low",	kTIMINGLIB_GROUPENUM_propagated_noise_height_low},
#line 16 "group_lookup"
      {"ccs_timing_base_curve_template",	kTIMINGLIB_GROUPENUM_ccs_timing_base_curve_template},
#line 54 "group_lookup"
      {"hyperbolic_noise_above_high",	kTIMINGLIB_GROUPENUM_hyperbolic_noise_above_high},
#line 164 "group_lookup"
      {"propagated_noise_height_high",	kTIMINGLIB_GROUPENUM_propagated_noise_height_high},
#line 85 "group_lookup"
      {"min_pulse_width",	kTIMINGLIB_GROUPENUM_min_pulse_width},
#line 140 "group_lookup"
      {"output_signal_swing",	kTIMINGLIB_GROUPENUM_output_signal_swing},
#line 139 "group_lookup"
      {"output_current_template",	kTIMINGLIB_GROUPENUM_output_current_template},
#line 118 "group_lookup"
      {"ocv_skewness_retain_fall_slew",	kTIMINGLIB_GROUPENUM_ocv_skewness_retain_fall_slew},
#line 119 "group_lookup"
      {"ocv_skewness_retain_rise_slew",	kTIMINGLIB_GROUPENUM_ocv_skewness_retain_rise_slew},
#line 171 "group_lookup"
      {"propagated_noise_peak_time_ratio_low",	kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_low},
#line 90 "group_lookup"
      {"noise_immunity_above_high",	kTIMINGLIB_GROUPENUM_noise_immunity_above_high},
#line 175 "group_lookup"
      {"propagated_noise_width_low",	kTIMINGLIB_GROUPENUM_propagated_noise_width_low},
#line 170 "group_lookup"
      {"propagated_noise_peak_time_ratio_high",	kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_high},
#line 174 "group_lookup"
      {"propagated_noise_width_high",	kTIMINGLIB_GROUPENUM_propagated_noise_width_high},
#line 162 "group_lookup"
      {"propagated_noise_height_above_high",	kTIMINGLIB_GROUPENUM_propagated_noise_height_above_high},
#line 55 "group_lookup"
      {"hyperbolic_noise_below_low",	kTIMINGLIB_GROUPENUM_hyperbolic_noise_below_low},
#line 168 "group_lookup"
      {"propagated_noise_peak_time_ratio_above_high",	kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_above_high},
#line 172 "group_lookup"
      {"propagated_noise_width_above_high",	kTIMINGLIB_GROUPENUM_propagated_noise_width_above_high},
#line 91 "group_lookup"
      {"noise_immunity_below_low",	kTIMINGLIB_GROUPENUM_noise_immunity_below_low},
#line 163 "group_lookup"
      {"propagated_noise_height_below_low",	kTIMINGLIB_GROUPENUM_propagated_noise_height_below_low},
#line 169 "group_lookup"
      {"propagated_noise_peak_time_ratio_below_low",	kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_below_low},
#line 173 "group_lookup"
      {"propagated_noise_width_below_low",	kTIMINGLIB_GROUPENUM_propagated_noise_width_below_low}
    };

  static const short lookup[] =
    {
       -1,  -1,   0,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,   1,  -1,  -1,  -1,  -1,  -1,   2,
        3,  -1,  -1,   4,  -1,  -1,  -1,  -1,   5,  -1,
       -1,   6,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,   7,   8,  -1,  -1,   9,  10,
       11,  -1,  -1,  -1,  -1,  12,  -1,  -1,  -1,  13,
       14,  15,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  16,  -1,  -1,  -1,  17,
       18,  -1,  -1,  19,  -1,  20,  21,  -1,  22,  -1,
       23,  24,  -1,  -1,  25,  26,  27,  28,  -1,  29,
       -1,  -1,  -1,  -1,  -1,  -1,  30,  31,  -1,  -1,
       32,  33,  -1,  -1,  -1,  -1,  34,  -1,  -1,  35,
       36,  37,  -1,  -1,  -1,  38,  39,  -1,  -1,  -1,
       40,  41,  -1,  -1,  42,  43,  -1,  -1,  -1,  -1,
       44,  45,  -1,  -1,  46,  47,  48,  49,  -1,  -1,
       50,  51,  -1,  -1,  -1,  52,  53,  -1,  -1,  -1,
       -1,  54,  -1,  55,  -1,  -1,  56,  57,  58,  -1,
       -1,  59,  60,  61,  -1,  62,  63,  64,  -1,  -1,
       -1,  65,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  66,
       -1,  67,  68,  69,  70,  71,  72,  -1,  -1,  -1,
       73,  -1,  -1,  -1,  74,  -1,  75,  -1,  -1,  76,
       -1,  77,  -1,  -1,  -1,  78,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  79,  80,  -1,  -1,  -1,  81,
       -1,  -1,  -1,  -1,  82,  -1,  -1,  83,  -1,  84,
       85,  -1,  86,  87,  -1,  -1,  -1,  -1,  88,  -1,
       89,  90,  91,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  92,  93,  -1,  -1,  -1,
       94,  95,  -1,  -1,  96,  97,  98,  99,  -1,  -1,
      100,  -1, 101,  -1,  -1, 102, 103, 104,  -1, 105,
       -1, 106, 107,  -1,  -1, 108,  -1, 109, 110,  -1,
       -1, 111, 112,  -1, 113,  -1, 114,  -1, 115, 116,
       -1,  -1,  -1,  -1,  -1, 117,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 118,  -1, 119, 120,  -1,
       -1,  -1,  -1,  -1, 121,  -1, 122,  -1, 123, 124,
       -1, 125,  -1, 126, 127, 128,  -1,  -1,  -1, 129,
       -1,  -1,  -1,  -1, 130, 131,  -1,  -1,  -1,  -1,
       -1, 132, 133,  -1, 134,  -1,  -1, 135, 136, 137,
       -1,  -1,  -1,  -1,  -1, 138, 139,  -1, 140,  -1,
       -1,  -1,  -1, 141,  -1,  -1, 142,  -1, 143, 144,
       -1,  -1,  -1, 145,  -1, 146,  -1,  -1, 147,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      148,  -1,  -1,  -1,  -1, 149,  -1, 150,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 151,  -1,  -1,  -1,  -1,  -1, 152,
       -1,  -1,  -1,  -1,  -1, 153,  -1,  -1,  -1, 154,
       -1,  -1, 155,  -1, 156,  -1, 157,  -1,  -1,  -1,
       -1,  -1,  -1, 158, 159,  -1,  -1, 160,  -1,  -1,
      161,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 162,
       -1, 163,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 164,
      165, 166,  -1,  -1, 167,  -1,  -1, 168,  -1,  -1,
       -1,  -1,  -1, 169,  -1, 170, 171,  -1,  -1,  -1,
       -1, 172,  -1,  -1, 173, 174, 175,  -1,  -1, 176,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 177,  -1, 178,  -1,  -1,  -1,  -1, 179,  -1,
       -1,  -1,  -1, 180,  -1,  -1,  -1,  -1,  -1,  -1,
      181,  -1,  -1, 182,  -1,  -1,  -1, 183, 184,  -1,
       -1,  -1, 185, 186,  -1,  -1,  -1,  -1, 187,  -1,
       -1, 188, 189, 190,  -1,  -1,  -1,  -1, 191, 192,
       -1,  -1,  -1,  -1,  -1, 193,  -1,  -1,  -1,  -1,
      194,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 195, 196,  -1,
       -1,  -1, 197,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 198,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 199,  -1,  -1, 200,  -1,  -1, 201,  -1, 202,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 203,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 204,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 205,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      206,  -1,  -1,  -1,  -1, 207,  -1,  -1, 208,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 209,
       -1,  -1,  -1,  -1, 210,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 211,
       -1,  -1,  -1, 212,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 213,  -1,  -1,  -1,  -1, 214,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 215,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 216,  -1,  -1, 217,  -1,  -1,  -1,  -1,
      218,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 219,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 220,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 221,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 222,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      223,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 224,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 225,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 226,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 227
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = group_hash_func (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].name;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[index];
            }
        }
    }
  return 0;
}
#line 236 "group_lookup"

