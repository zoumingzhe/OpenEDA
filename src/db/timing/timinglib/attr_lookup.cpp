/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -L ANSI-C -t -H attr_hash_func -N lookup_attr_name -C -I -k '*' -D attr_lookup  */

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

#line 1 "attr_lookup"

#include "attr_enum.h"

using namespace Timinglib;
#line 6 "attr_lookup"
struct libGroupMap { const char *name; attr_enum type; };
#include <string.h>

#define TOTAL_KEYWORDS 678
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 54
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 4761
/* maximum key range = 4758, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
attr_hash_func (register const char *str, register size_t len)
{
  static const unsigned short asso_values[] =
    {
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,    0,  135,
        15,   10,    0,   25,   35,   30, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762,    0,  715,    0,   85,   25,
       120,    0,    5,  710,  205,    0,  888,  155,    0,  120,
         0,   45,   25,  335,    0,    0,    5,  445,  275,  766,
       995, 1224,   30,    0, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762, 4762,
      4762, 4762, 4762, 4762, 4762, 4762, 4762
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[53]];
      /*FALLTHROUGH*/
      case 53:
        hval += asso_values[(unsigned char)str[52]];
      /*FALLTHROUGH*/
      case 52:
        hval += asso_values[(unsigned char)str[51]];
      /*FALLTHROUGH*/
      case 51:
        hval += asso_values[(unsigned char)str[50]];
      /*FALLTHROUGH*/
      case 50:
        hval += asso_values[(unsigned char)str[49]];
      /*FALLTHROUGH*/
      case 49:
        hval += asso_values[(unsigned char)str[48]];
      /*FALLTHROUGH*/
      case 48:
        hval += asso_values[(unsigned char)str[47]];
      /*FALLTHROUGH*/
      case 47:
        hval += asso_values[(unsigned char)str[46]];
      /*FALLTHROUGH*/
      case 46:
        hval += asso_values[(unsigned char)str[45]];
      /*FALLTHROUGH*/
      case 45:
        hval += asso_values[(unsigned char)str[44]];
      /*FALLTHROUGH*/
      case 44:
        hval += asso_values[(unsigned char)str[43]];
      /*FALLTHROUGH*/
      case 43:
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
        hval += asso_values[(unsigned char)str[7]+1];
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
lookup_attr_name (register const char *str, register size_t len)
{
  static const struct libGroupMap wordlist[] =
    {
#line 19 "attr_lookup"
      {"area",	kTIMINGLIB_ATTRENUM_area},
#line 49 "attr_lookup"
      {"clear",	kTIMINGLIB_ATTRENUM_clear},
#line 43 "attr_lookup"
      {"cell_name",	kTIMINGLIB_ATTRENUM_cell_name},
#line 496 "attr_lookup"
      {"preset",	kTIMINGLIB_ATTRENUM_preset},
#line 253 "attr_lookup"
      {"is_filler_cell",	kTIMINGLIB_ATTRENUM_is_filler_cell},
#line 558 "attr_lookup"
      {"rise_resistance",	kTIMINGLIB_ATTRENUM_rise_resistance},
#line 268 "attr_lookup"
      {"is_soi",	kTIMINGLIB_ATTRENUM_is_soi},
#line 172 "attr_lookup"
      {"fall_resistance",	kTIMINGLIB_ATTRENUM_fall_resistance},
#line 549 "attr_lookup"
      {"retention_pin",	kTIMINGLIB_ATTRENUM_retention_pin},
#line 548 "attr_lookup"
      {"retention_cell",	kTIMINGLIB_ATTRENUM_retention_cell},
#line 588 "attr_lookup"
      {"slope",	kTIMINGLIB_ATTRENUM_slope},
#line 64 "attr_lookup"
      {"coefs",	kTIMINGLIB_ATTRENUM_coefs},
#line 540 "attr_lookup"
      {"resistance",	kTIMINGLIB_ATTRENUM_resistance},
#line 161 "attr_lookup"
      {"enable",	kTIMINGLIB_ATTRENUM_enable},
#line 612 "attr_lookup"
      {"table",	kTIMINGLIB_ATTRENUM_table},
#line 184 "attr_lookup"
      {"force_01",	kTIMINGLIB_ATTRENUM_force_01},
#line 267 "attr_lookup"
      {"is_pll_reference_pin",	kTIMINGLIB_ATTRENUM_is_pll_reference_pin},
#line 498 "attr_lookup"
      {"process",	kTIMINGLIB_ATTRENUM_process},
#line 557 "attr_lookup"
      {"rise_pin_resistance",	kTIMINGLIB_ATTRENUM_rise_pin_resistance},
#line 477 "attr_lookup"
      {"piece_type",	kTIMINGLIB_ATTRENUM_piece_type},
#line 171 "attr_lookup"
      {"fall_pin_resistance",	kTIMINGLIB_ATTRENUM_fall_pin_resistance},
#line 81 "attr_lookup"
      {"date",	kTIMINGLIB_ATTRENUM_date},
#line 20 "attr_lookup"
      {"area_coefficient",	kTIMINGLIB_ATTRENUM_area_coefficient},
#line 78 "attr_lookup"
      {"data_in",	kTIMINGLIB_ATTRENUM_data_in},
#line 26 "attr_lookup"
      {"bit_to",	kTIMINGLIB_ATTRENUM_bit_to},
#line 25 "attr_lookup"
      {"bit_from",	kTIMINGLIB_ATTRENUM_bit_from},
#line 41 "attr_lookup"
      {"cell_footprint",	kTIMINGLIB_ATTRENUM_cell_footprint},
#line 262 "attr_lookup"
      {"is_pad",	kTIMINGLIB_ATTRENUM_is_pad},
#line 403 "attr_lookup"
      {"lsi_pad",	kTIMINGLIB_ATTRENUM_lsi_pad},
#line 264 "attr_lookup"
      {"is_pll_cell",	kTIMINGLIB_ATTRENUM_is_pll_cell},
#line 481 "attr_lookup"
      {"pin_names",	kTIMINGLIB_ATTRENUM_pin_names},
#line 269 "attr_lookup"
      {"is_tap_cell",	kTIMINGLIB_ATTRENUM_is_tap_cell},
#line 482 "attr_lookup"
      {"pin_opposite",	kTIMINGLIB_ATTRENUM_pin_opposite},
#line 492 "attr_lookup"
      {"preferred",	kTIMINGLIB_ATTRENUM_preferred},
#line 463 "attr_lookup"
      {"parameter4",	kTIMINGLIB_ATTRENUM_parameter4},
#line 589 "attr_lookup"
      {"slope_fall",	kTIMINGLIB_ATTRENUM_slope_fall},
#line 449 "attr_lookup"
      {"orders",	kTIMINGLIB_ATTRENUM_orders},
#line 462 "attr_lookup"
      {"parameter3",	kTIMINGLIB_ATTRENUM_parameter3},
#line 461 "attr_lookup"
      {"parameter2",	kTIMINGLIB_ATTRENUM_parameter2},
#line 574 "attr_lookup"
      {"set_node",	kTIMINGLIB_ATTRENUM_set_node},
#line 133 "attr_lookup"
      {"direction",	kTIMINGLIB_ATTRENUM_direction},
#line 261 "attr_lookup"
      {"is_no_enable",	kTIMINGLIB_ATTRENUM_is_no_enable},
#line 464 "attr_lookup"
      {"parameter5",	kTIMINGLIB_ATTRENUM_parameter5},
#line 632 "attr_lookup"
      {"tracks",	kTIMINGLIB_ATTRENUM_tracks},
#line 476 "attr_lookup"
      {"piece_define",	kTIMINGLIB_ATTRENUM_piece_define},
#line 573 "attr_lookup"
      {"sensitization_master",	kTIMINGLIB_ATTRENUM_sensitization_master},
#line 32 "attr_lookup"
      {"capacitance",	kTIMINGLIB_ATTRENUM_capacitance},
#line 568 "attr_lookup"
      {"sdf_cond",	kTIMINGLIB_ATTRENUM_sdf_cond},
#line 506 "attr_lookup"
      {"rail_connection",	kTIMINGLIB_ATTRENUM_rail_connection},
#line 259 "attr_lookup"
      {"is_macro_cell",	kTIMINGLIB_ATTRENUM_is_macro_cell},
#line 254 "attr_lookup"
      {"is_illegal",	kTIMINGLIB_ATTRENUM_is_illegal},
#line 183 "attr_lookup"
      {"force_00",	kTIMINGLIB_ATTRENUM_force_00},
#line 570 "attr_lookup"
      {"sdf_cond_start",	kTIMINGLIB_ATTRENUM_sdf_cond_start},
#line 576 "attr_lookup"
      {"shifts",	kTIMINGLIB_ATTRENUM_shifts},
#line 257 "attr_lookup"
      {"is_isolation_cell",	kTIMINGLIB_ATTRENUM_is_isolation_cell},
#line 613 "attr_lookup"
      {"tdisable",	kTIMINGLIB_ATTRENUM_tdisable},
#line 31 "attr_lookup"
      {"calc_mode",	kTIMINGLIB_ATTRENUM_calc_mode},
#line 162 "attr_lookup"
      {"enable_also",	kTIMINGLIB_ATTRENUM_enable_also},
#line 408 "attr_lookup"
      {"map_only",	kTIMINGLIB_ATTRENUM_map_only},
#line 186 "attr_lookup"
      {"force_11",	kTIMINGLIB_ATTRENUM_force_11},
#line 431 "attr_lookup"
      {"min_transition",	kTIMINGLIB_ATTRENUM_min_transition},
#line 274 "attr_lookup"
      {"isolation_cell_enable_pin",	kTIMINGLIB_ATTRENUM_isolation_cell_enable_pin},
#line 250 "attr_lookup"
      {"is_clock_isolation_cell",	kTIMINGLIB_ATTRENUM_is_clock_isolation_cell},
#line 270 "attr_lookup"
      {"is_three_state",	kTIMINGLIB_ATTRENUM_is_three_state},
#line 10 "attr_lookup"
      {"address",	kTIMINGLIB_ATTRENUM_address},
#line 517 "attr_lookup"
      {"reference_capacitance",	kTIMINGLIB_ATTRENUM_reference_capacitance},
#line 260 "attr_lookup"
      {"is_needed",	kTIMINGLIB_ATTRENUM_is_needed},
#line 52 "attr_lookup"
      {"clock",	kTIMINGLIB_ATTRENUM_clock},
#line 631 "attr_lookup"
      {"total_track_area",	kTIMINGLIB_ATTRENUM_total_track_area},
#line 577 "attr_lookup"
      {"short",	kTIMINGLIB_ATTRENUM_short},
#line 130 "attr_lookup"
      {"define_cell_area",	kTIMINGLIB_ATTRENUM_define_cell_area},
#line 520 "attr_lookup"
      {"reference_time",	kTIMINGLIB_ATTRENUM_reference_time},
#line 256 "attr_lookup"
      {"is_isolated",	kTIMINGLIB_ATTRENUM_is_isolated},
#line 273 "attr_lookup"
      {"isolation_cell_data_pin",	kTIMINGLIB_ATTRENUM_isolation_cell_data_pin},
#line 659 "attr_lookup"
      {"vil",	kTIMINGLIB_ATTRENUM_vil},
#line 424 "attr_lookup"
      {"miller_cap_rise",	kTIMINGLIB_ATTRENUM_miller_cap_rise},
#line 423 "attr_lookup"
      {"miller_cap_fall",	kTIMINGLIB_ATTRENUM_miller_cap_fall},
#line 246 "attr_lookup"
      {"invert",	kTIMINGLIB_ATTRENUM_invert},
#line 432 "attr_lookup"
      {"mode",	kTIMINGLIB_ATTRENUM_mode},
#line 458 "attr_lookup"
      {"pad_cell",	kTIMINGLIB_ATTRENUM_pad_cell},
#line 279 "attr_lookup"
      {"k_process_cell_rise",	kTIMINGLIB_ATTRENUM_k_process_cell_rise},
#line 460 "attr_lookup"
      {"parameter1",	kTIMINGLIB_ATTRENUM_parameter1},
#line 242 "attr_lookup"
      {"internal_node",	kTIMINGLIB_ATTRENUM_internal_node},
#line 277 "attr_lookup"
      {"k_process_cell_fall",	kTIMINGLIB_ATTRENUM_k_process_cell_fall},
#line 480 "attr_lookup"
      {"pin_name_map",	kTIMINGLIB_ATTRENUM_pin_name_map},
#line 291 "attr_lookup"
      {"k_process_intrinsic_rise",	kTIMINGLIB_ATTRENUM_k_process_intrinsic_rise},
#line 290 "attr_lookup"
      {"k_process_intrinsic_fall",	kTIMINGLIB_ATTRENUM_k_process_intrinsic_fall},
#line 640 "attr_lookup"
      {"va_parameters",	kTIMINGLIB_ATTRENUM_va_parameters},
#line 66 "attr_lookup"
      {"comment",	kTIMINGLIB_ATTRENUM_comment},
#line 663 "attr_lookup"
      {"vol",	kTIMINGLIB_ATTRENUM_vol},
#line 425 "attr_lookup"
      {"min_capacitance",	kTIMINGLIB_ATTRENUM_min_capacitance},
#line 569 "attr_lookup"
      {"sdf_cond_end",	kTIMINGLIB_ATTRENUM_sdf_cond_end},
#line 419 "attr_lookup"
      {"members",	kTIMINGLIB_ATTRENUM_members},
#line 307 "attr_lookup"
      {"k_process_rise_transition",	kTIMINGLIB_ATTRENUM_k_process_rise_transition},
#line 305 "attr_lookup"
      {"k_process_rise_pin_resistance",	kTIMINGLIB_ATTRENUM_k_process_rise_pin_resistance},
#line 286 "attr_lookup"
      {"k_process_fall_transition",	kTIMINGLIB_ATTRENUM_k_process_fall_transition},
#line 284 "attr_lookup"
      {"k_process_fall_pin_resistance",	kTIMINGLIB_ATTRENUM_k_process_fall_pin_resistance},
#line 313 "attr_lookup"
      {"k_process_slope_rise",	kTIMINGLIB_ATTRENUM_k_process_slope_rise},
#line 297 "attr_lookup"
      {"k_process_pin_cap",	kTIMINGLIB_ATTRENUM_k_process_pin_cap},
#line 312 "attr_lookup"
      {"k_process_slope_fall",	kTIMINGLIB_ATTRENUM_k_process_slope_fall},
#line 592 "attr_lookup"
      {"soft_error_rate_confidence",	kTIMINGLIB_ATTRENUM_soft_error_rate_confidence},
#line 299 "attr_lookup"
      {"k_process_pin_rise_cap",	kTIMINGLIB_ATTRENUM_k_process_pin_rise_cap},
#line 185 "attr_lookup"
      {"force_10",	kTIMINGLIB_ATTRENUM_force_10},
#line 656 "attr_lookup"
      {"vector",	kTIMINGLIB_ATTRENUM_vector},
#line 298 "attr_lookup"
      {"k_process_pin_fall_cap",	kTIMINGLIB_ATTRENUM_k_process_pin_fall_cap},
#line 51 "attr_lookup"
      {"clear_preset_var2",	kTIMINGLIB_ATTRENUM_clear_preset_var2},
#line 440 "attr_lookup"
      {"nom_process",	kTIMINGLIB_ATTRENUM_nom_process},
#line 550 "attr_lookup"
      {"revision",	kTIMINGLIB_ATTRENUM_revision},
#line 655 "attr_lookup"
      {"variables",	kTIMINGLIB_ATTRENUM_variables},
#line 650 "attr_lookup"
      {"variable_4",	kTIMINGLIB_ATTRENUM_variable_4},
#line 595 "attr_lookup"
      {"state_function",	kTIMINGLIB_ATTRENUM_state_function},
#line 345 "attr_lookup"
      {"k_temp_rise_transition",	kTIMINGLIB_ATTRENUM_k_temp_rise_transition},
#line 648 "attr_lookup"
      {"variable_3",	kTIMINGLIB_ATTRENUM_variable_3},
#line 343 "attr_lookup"
      {"k_temp_rise_pin_resistance",	kTIMINGLIB_ATTRENUM_k_temp_rise_pin_resistance},
#line 646 "attr_lookup"
      {"variable_2",	kTIMINGLIB_ATTRENUM_variable_2},
#line 351 "attr_lookup"
      {"k_temp_slope_rise",	kTIMINGLIB_ATTRENUM_k_temp_slope_rise},
#line 434 "attr_lookup"
      {"mpm_name",	kTIMINGLIB_ATTRENUM_mpm_name},
#line 661 "attr_lookup"
      {"vimin",	kTIMINGLIB_ATTRENUM_vimin},
#line 350 "attr_lookup"
      {"k_temp_slope_fall",	kTIMINGLIB_ATTRENUM_k_temp_slope_fall},
#line 596 "attr_lookup"
      {"std_cell_main_rail",	kTIMINGLIB_ATTRENUM_std_cell_main_rail},
#line 399 "attr_lookup"
      {"level_shifter_enable_pin",	kTIMINGLIB_ATTRENUM_level_shifter_enable_pin},
#line 275 "attr_lookup"
      {"isolation_enable_condition",	kTIMINGLIB_ATTRENUM_isolation_enable_condition},
#line 552 "attr_lookup"
      {"rise_capacitance",	kTIMINGLIB_ATTRENUM_rise_capacitance},
#line 166 "attr_lookup"
      {"fall_capacitance",	kTIMINGLIB_ATTRENUM_fall_capacitance},
#line 319 "attr_lookup"
      {"k_temp_cell_rise",	kTIMINGLIB_ATTRENUM_k_temp_cell_rise},
#line 670 "attr_lookup"
      {"vomin",	kTIMINGLIB_ATTRENUM_vomin},
#line 317 "attr_lookup"
      {"k_temp_cell_fall",	kTIMINGLIB_ATTRENUM_k_temp_cell_fall},
#line 398 "attr_lookup"
      {"level_shifter_data_pin",	kTIMINGLIB_ATTRENUM_level_shifter_data_pin},
#line 135 "attr_lookup"
      {"dont_fault",	kTIMINGLIB_ATTRENUM_dont_fault},
#line 411 "attr_lookup"
      {"master_pin",	kTIMINGLIB_ATTRENUM_master_pin},
#line 211 "attr_lookup"
      {"hold_state",	kTIMINGLIB_ATTRENUM_hold_state},
#line 223 "attr_lookup"
      {"input",	kTIMINGLIB_ATTRENUM_input},
#line 50 "attr_lookup"
      {"clear_preset_var1",	kTIMINGLIB_ATTRENUM_clear_preset_var1},
#line 658 "attr_lookup"
      {"vih",	kTIMINGLIB_ATTRENUM_vih},
#line 328 "attr_lookup"
      {"k_temp_hold_rise",	kTIMINGLIB_ATTRENUM_k_temp_hold_rise},
#line 327 "attr_lookup"
      {"k_temp_hold_fall",	kTIMINGLIB_ATTRENUM_k_temp_hold_fall},
#line 210 "attr_lookup"
      {"hold_coefficient",	kTIMINGLIB_ATTRENUM_hold_coefficient},
#line 644 "attr_lookup"
      {"variable_1",	kTIMINGLIB_ATTRENUM_variable_1},
#line 332 "attr_lookup"
      {"k_temp_min_period",	kTIMINGLIB_ATTRENUM_k_temp_min_period},
#line 251 "attr_lookup"
      {"is_decap_cell",	kTIMINGLIB_ATTRENUM_is_decap_cell},
#line 212 "attr_lookup"
      {"hold_state_also",	kTIMINGLIB_ATTRENUM_hold_state_also},
#line 433 "attr_lookup"
      {"mpm_libname",	kTIMINGLIB_ATTRENUM_mpm_libname},
#line 662 "attr_lookup"
      {"voh",	kTIMINGLIB_ATTRENUM_voh},
#line 541 "attr_lookup"
      {"resistance_unit",	kTIMINGLIB_ATTRENUM_resistance_unit},
#line 180 "attr_lookup"
      {"fanout_resistance",	kTIMINGLIB_ATTRENUM_fanout_resistance},
#line 383 "attr_lookup"
      {"k_volt_rise_transition",	kTIMINGLIB_ATTRENUM_k_volt_rise_transition},
#line 381 "attr_lookup"
      {"k_volt_rise_pin_resistance",	kTIMINGLIB_ATTRENUM_k_volt_rise_pin_resistance},
#line 389 "attr_lookup"
      {"k_volt_slope_rise",	kTIMINGLIB_ATTRENUM_k_volt_slope_rise},
#line 565 "attr_lookup"
      {"save_condition",	kTIMINGLIB_ATTRENUM_save_condition},
#line 388 "attr_lookup"
      {"k_volt_slope_fall",	kTIMINGLIB_ATTRENUM_k_volt_slope_fall},
#line 201 "attr_lookup"
      {"function",	kTIMINGLIB_ATTRENUM_function},
#line 575 "attr_lookup"
      {"setup_coefficient",	kTIMINGLIB_ATTRENUM_setup_coefficient},
#line 292 "attr_lookup"
      {"k_process_min_period",	kTIMINGLIB_ATTRENUM_k_process_min_period},
#line 200 "attr_lookup"
      {"fsim_map",	kTIMINGLIB_ATTRENUM_fsim_map},
#line 176 "attr_lookup"
      {"fanout_area",	kTIMINGLIB_ATTRENUM_fanout_area},
#line 322 "attr_lookup"
      {"k_temp_drive_rise",	kTIMINGLIB_ATTRENUM_k_temp_drive_rise},
#line 321 "attr_lookup"
      {"k_temp_drive_fall",	kTIMINGLIB_ATTRENUM_k_temp_drive_fall},
#line 491 "attr_lookup"
      {"prefer_tied",	kTIMINGLIB_ATTRENUM_prefer_tied},
#line 439 "attr_lookup"
      {"nom_calc_mode",	kTIMINGLIB_ATTRENUM_nom_calc_mode},
#line 657 "attr_lookup"
      {"vhdl_name",	kTIMINGLIB_ATTRENUM_vhdl_name},
#line 426 "attr_lookup"
      {"min_fanout",	kTIMINGLIB_ATTRENUM_min_fanout},
#line 518 "attr_lookup"
      {"reference_input",	kTIMINGLIB_ATTRENUM_reference_input},
#line 357 "attr_lookup"
      {"k_volt_cell_rise",	kTIMINGLIB_ATTRENUM_k_volt_cell_rise},
#line 559 "attr_lookup"
      {"rise_time_after_threshold",	kTIMINGLIB_ATTRENUM_rise_time_after_threshold},
#line 355 "attr_lookup"
      {"k_volt_cell_fall",	kTIMINGLIB_ATTRENUM_k_volt_cell_fall},
#line 173 "attr_lookup"
      {"fall_time_after_threshold",	kTIMINGLIB_ATTRENUM_fall_time_after_threshold},
#line 258 "attr_lookup"
      {"is_level_shifter",	kTIMINGLIB_ATTRENUM_is_level_shifter},
#line 137 "attr_lookup"
      {"dont_use",	kTIMINGLIB_ATTRENUM_dont_use},
#line 288 "attr_lookup"
      {"k_process_hold_rise",	kTIMINGLIB_ATTRENUM_k_process_hold_rise},
#line 287 "attr_lookup"
      {"k_process_hold_fall",	kTIMINGLIB_ATTRENUM_k_process_hold_fall},
#line 252 "attr_lookup"
      {"is_differential_level_shifter",	kTIMINGLIB_ATTRENUM_is_differential_level_shifter},
#line 366 "attr_lookup"
      {"k_volt_hold_rise",	kTIMINGLIB_ATTRENUM_k_volt_hold_rise},
#line 365 "attr_lookup"
      {"k_volt_hold_fall",	kTIMINGLIB_ATTRENUM_k_volt_hold_fall},
#line 282 "attr_lookup"
      {"k_process_drive_rise",	kTIMINGLIB_ATTRENUM_k_process_drive_rise},
#line 617 "attr_lookup"
      {"three_state",	kTIMINGLIB_ATTRENUM_three_state},
#line 281 "attr_lookup"
      {"k_process_drive_fall",	kTIMINGLIB_ATTRENUM_k_process_drive_fall},
#line 60 "attr_lookup"
      {"clock_isolation_cell_clock_pin",	kTIMINGLIB_ATTRENUM_clock_isolation_cell_clock_pin},
#line 370 "attr_lookup"
      {"k_volt_min_period",	kTIMINGLIB_ATTRENUM_k_volt_min_period},
#line 337 "attr_lookup"
      {"k_temp_pin_cap",	kTIMINGLIB_ATTRENUM_k_temp_pin_cap},
#line 623 "attr_lookup"
      {"threshold",	kTIMINGLIB_ATTRENUM_threshold},
#line 139 "attr_lookup"
      {"drive_current",	kTIMINGLIB_ATTRENUM_drive_current},
#line 226 "attr_lookup"
      {"input_map",	kTIMINGLIB_ATTRENUM_input_map},
#line 303 "attr_lookup"
      {"k_process_removal_rise",	kTIMINGLIB_ATTRENUM_k_process_removal_rise},
#line 177 "attr_lookup"
      {"fanout_capacitance",	kTIMINGLIB_ATTRENUM_fanout_capacitance},
#line 302 "attr_lookup"
      {"k_process_removal_fall",	kTIMINGLIB_ATTRENUM_k_process_removal_fall},
#line 272 "attr_lookup"
      {"is_unconnected",	kTIMINGLIB_ATTRENUM_is_unconnected},
#line 642 "attr_lookup"
      {"value",	kTIMINGLIB_ATTRENUM_value},
#line 643 "attr_lookup"
      {"values",	kTIMINGLIB_ATTRENUM_values},
#line 182 "attr_lookup"
      {"fault_model",	kTIMINGLIB_ATTRENUM_fault_model},
#line 136 "attr_lookup"
      {"dont_touch",	kTIMINGLIB_ATTRENUM_dont_touch},
#line 239 "attr_lookup"
      {"interdependence_id",	kTIMINGLIB_ATTRENUM_interdependence_id},
#line 248 "attr_lookup"
      {"is_analog",	kTIMINGLIB_ATTRENUM_is_analog},
#line 560 "attr_lookup"
      {"rise_time_before_threshold",	kTIMINGLIB_ATTRENUM_rise_time_before_threshold},
#line 594 "attr_lookup"
      {"start_setting",	kTIMINGLIB_ATTRENUM_start_setting},
#line 309 "attr_lookup"
      {"k_process_setup_rise",	kTIMINGLIB_ATTRENUM_k_process_setup_rise},
#line 174 "attr_lookup"
      {"fall_time_before_threshold",	kTIMINGLIB_ATTRENUM_fall_time_before_threshold},
#line 75 "attr_lookup"
      {"curve_id",	kTIMINGLIB_ATTRENUM_curve_id},
#line 308 "attr_lookup"
      {"k_process_setup_fall",	kTIMINGLIB_ATTRENUM_k_process_setup_fall},
#line 341 "attr_lookup"
      {"k_temp_removal_rise",	kTIMINGLIB_ATTRENUM_k_temp_removal_rise},
#line 340 "attr_lookup"
      {"k_temp_removal_fall",	kTIMINGLIB_ATTRENUM_k_temp_removal_fall},
#line 360 "attr_lookup"
      {"k_volt_drive_rise",	kTIMINGLIB_ATTRENUM_k_volt_drive_rise},
#line 359 "attr_lookup"
      {"k_volt_drive_fall",	kTIMINGLIB_ATTRENUM_k_volt_drive_fall},
#line 593 "attr_lookup"
      {"stage_type",	kTIMINGLIB_ATTRENUM_stage_type},
#line 179 "attr_lookup"
      {"fanout_load",	kTIMINGLIB_ATTRENUM_fanout_load},
#line 564 "attr_lookup"
      {"save_action",	kTIMINGLIB_ATTRENUM_save_action},
#line 347 "attr_lookup"
      {"k_temp_setup_rise",	kTIMINGLIB_ATTRENUM_k_temp_setup_rise},
#line 346 "attr_lookup"
      {"k_temp_setup_fall",	kTIMINGLIB_ATTRENUM_k_temp_setup_fall},
#line 181 "attr_lookup"
      {"faster_factor",	kTIMINGLIB_ATTRENUM_faster_factor},
#line 402 "attr_lookup"
      {"low",	kTIMINGLIB_ATTRENUM_low},
#line 447 "attr_lookup"
      {"open_drain_function",	kTIMINGLIB_ATTRENUM_open_drain_function},
#line 159 "attr_lookup"
      {"edges",	kTIMINGLIB_ATTRENUM_edges},
#line 572 "attr_lookup"
      {"segment",	kTIMINGLIB_ATTRENUM_segment},
#line 629 "attr_lookup"
      {"timing_sense",	kTIMINGLIB_ATTRENUM_timing_sense},
#line 545 "attr_lookup"
      {"restore_action",	kTIMINGLIB_ATTRENUM_restore_action},
#line 187 "attr_lookup"
      {"force_inverter_removal",	kTIMINGLIB_ATTRENUM_force_inverter_removal},
#line 241 "attr_lookup"
      {"intermediate_values",	kTIMINGLIB_ATTRENUM_intermediate_values},
#line 405 "attr_lookup"
      {"lsi_pad_rise",	kTIMINGLIB_ATTRENUM_lsi_pad_rise},
#line 534 "attr_lookup"
      {"related_pin",	kTIMINGLIB_ATTRENUM_related_pin},
#line 404 "attr_lookup"
      {"lsi_pad_fall",	kTIMINGLIB_ATTRENUM_lsi_pad_fall},
#line 375 "attr_lookup"
      {"k_volt_pin_cap",	kTIMINGLIB_ATTRENUM_k_volt_pin_cap},
#line 410 "attr_lookup"
      {"mapping",	kTIMINGLIB_ATTRENUM_mapping},
#line 625 "attr_lookup"
      {"tied_off",	kTIMINGLIB_ATTRENUM_tied_off},
#line 478 "attr_lookup"
      {"pin_equal",	kTIMINGLIB_ATTRENUM_pin_equal},
#line 578 "attr_lookup"
      {"sigma_type",	kTIMINGLIB_ATTRENUM_sigma_type},
#line 519 "attr_lookup"
      {"reference_pg_pin",	kTIMINGLIB_ATTRENUM_reference_pg_pin},
#line 435 "attr_lookup"
      {"multicell_pad_pin",	kTIMINGLIB_ATTRENUM_multicell_pad_pin},
#line 422 "attr_lookup"
      {"merge_tolerance_rel",	kTIMINGLIB_ATTRENUM_merge_tolerance_rel},
#line 499 "attr_lookup"
      {"process_lable",	kTIMINGLIB_ATTRENUM_process_lable},
#line 628 "attr_lookup"
      {"timing_report",	kTIMINGLIB_ATTRENUM_timing_report},
#line 420 "attr_lookup"
      {"merge_selection",	kTIMINGLIB_ATTRENUM_merge_selection},
#line 236 "attr_lookup"
      {"input_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_input_threshold_pct_rise},
#line 490 "attr_lookup"
      {"power_rail",	kTIMINGLIB_ATTRENUM_power_rail},
#line 245 "attr_lookup"
      {"intrinsic_rise",	kTIMINGLIB_ATTRENUM_intrinsic_rise},
#line 235 "attr_lookup"
      {"input_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_input_threshold_pct_fall},
#line 227 "attr_lookup"
      {"input_map_shift",	kTIMINGLIB_ATTRENUM_input_map_shift},
#line 244 "attr_lookup"
      {"intrinsic_fall",	kTIMINGLIB_ATTRENUM_intrinsic_fall},
#line 379 "attr_lookup"
      {"k_volt_removal_rise",	kTIMINGLIB_ATTRENUM_k_volt_removal_rise},
#line 582 "attr_lookup"
      {"slew_control",	kTIMINGLIB_ATTRENUM_slew_control},
#line 378 "attr_lookup"
      {"k_volt_removal_fall",	kTIMINGLIB_ATTRENUM_k_volt_removal_fall},
#line 202 "attr_lookup"
      {"geometry_print",	kTIMINGLIB_ATTRENUM_geometry_print},
#line 448 "attr_lookup"
      {"open_source_function",	kTIMINGLIB_ATTRENUM_open_source_function},
#line 207 "attr_lookup"
      {"has_pass_gate",	kTIMINGLIB_ATTRENUM_has_pass_gate},
#line 521 "attr_lookup"
      {"related_bias_pin",	kTIMINGLIB_ATTRENUM_related_bias_pin},
#line 590 "attr_lookup"
      {"slope_rise",	kTIMINGLIB_ATTRENUM_slope_rise},
#line 571 "attr_lookup"
      {"sdf_edges",	kTIMINGLIB_ATTRENUM_sdf_edges},
#line 65 "attr_lookup"
      {"column_address",	kTIMINGLIB_ATTRENUM_column_address},
#line 677 "attr_lookup"
      {"when",	kTIMINGLIB_ATTRENUM_when},
#line 385 "attr_lookup"
      {"k_volt_setup_rise",	kTIMINGLIB_ATTRENUM_k_volt_setup_rise},
#line 69 "attr_lookup"
      {"constraint",	kTIMINGLIB_ATTRENUM_constraint},
#line 678 "attr_lookup"
      {"when_end",	kTIMINGLIB_ATTRENUM_when_end},
#line 240 "attr_lookup"
      {"interface_timing",	kTIMINGLIB_ATTRENUM_interface_timing},
#line 384 "attr_lookup"
      {"k_volt_setup_fall",	kTIMINGLIB_ATTRENUM_k_volt_setup_fall},
#line 138 "attr_lookup"
      {"downto",	kTIMINGLIB_ATTRENUM_downto},
#line 421 "attr_lookup"
      {"merge_tolerance_abs",	kTIMINGLIB_ATTRENUM_merge_tolerance_abs},
#line 255 "attr_lookup"
      {"is_inverting",	kTIMINGLIB_ATTRENUM_is_inverting},
#line 641 "attr_lookup"
      {"va_values",	kTIMINGLIB_ATTRENUM_va_values},
#line 266 "attr_lookup"
      {"is_pll_output_pin",	kTIMINGLIB_ATTRENUM_is_pll_output_pin},
#line 206 "attr_lookup"
      {"has_builtin_pad",	kTIMINGLIB_ATTRENUM_has_builtin_pad},
#line 546 "attr_lookup"
      {"restore_condition",	kTIMINGLIB_ATTRENUM_restore_condition},
#line 47 "attr_lookup"
      {"clamp_latch_function",	kTIMINGLIB_ATTRENUM_clamp_latch_function},
#line 538 "attr_lookup"
      {"required_condition",	kTIMINGLIB_ATTRENUM_required_condition},
#line 315 "attr_lookup"
      {"k_process_wire_res",	kTIMINGLIB_ATTRENUM_k_process_wire_res},
#line 664 "attr_lookup"
      {"voltage",	kTIMINGLIB_ATTRENUM_voltage},
#line 14 "attr_lookup"
      {"altitude_unit",	kTIMINGLIB_ATTRENUM_altitude_unit},
#line 615 "attr_lookup"
      {"temperature",	kTIMINGLIB_ATTRENUM_temperature},
#line 674 "attr_lookup"
      {"wave_rise",	kTIMINGLIB_ATTRENUM_wave_rise},
#line 638 "attr_lookup"
      {"user_function_class",	kTIMINGLIB_ATTRENUM_user_function_class},
#line 563 "attr_lookup"
      {"row_address",	kTIMINGLIB_ATTRENUM_row_address},
#line 68 "attr_lookup"
      {"connection_class",	kTIMINGLIB_ATTRENUM_connection_class},
#line 320 "attr_lookup"
      {"k_temp_drive_current",	kTIMINGLIB_ATTRENUM_k_temp_drive_current},
#line 679 "attr_lookup"
      {"when_start",	kTIMINGLIB_ATTRENUM_when_start},
#line 516 "attr_lookup"
      {"receiver_capacitance_rise_threshold_pct",	kTIMINGLIB_ATTRENUM_receiver_capacitance_rise_threshold_pct},
#line 515 "attr_lookup"
      {"receiver_capacitance_fall_threshold_pct",	kTIMINGLIB_ATTRENUM_receiver_capacitance_fall_threshold_pct},
#line 539 "attr_lookup"
      {"required_condition_also",	kTIMINGLIB_ATTRENUM_required_condition_also},
#line 314 "attr_lookup"
      {"k_process_wire_cap",	kTIMINGLIB_ATTRENUM_k_process_wire_cap},
#line 441 "attr_lookup"
      {"nom_temperature",	kTIMINGLIB_ATTRENUM_nom_temperature},
#line 651 "attr_lookup"
      {"variable_4_range",	kTIMINGLIB_ATTRENUM_variable_4_range},
#line 326 "attr_lookup"
      {"k_temp_fall_transition",	kTIMINGLIB_ATTRENUM_k_temp_fall_transition},
#line 324 "attr_lookup"
      {"k_temp_fall_pin_resistance",	kTIMINGLIB_ATTRENUM_k_temp_fall_pin_resistance},
#line 649 "attr_lookup"
      {"variable_3_range",	kTIMINGLIB_ATTRENUM_variable_3_range},
#line 57 "attr_lookup"
      {"clock_gate_test_pin",	kTIMINGLIB_ATTRENUM_clock_gate_test_pin},
#line 437 "attr_lookup"
      {"next_state",	kTIMINGLIB_ATTRENUM_next_state},
#line 647 "attr_lookup"
      {"variable_2_range",	kTIMINGLIB_ATTRENUM_variable_2_range},
#line 409 "attr_lookup"
      {"map_to_logic",	kTIMINGLIB_ATTRENUM_map_to_logic},
#line 469 "attr_lookup"
      {"pg_pin_condition",	kTIMINGLIB_ATTRENUM_pg_pin_condition},
#line 489 "attr_lookup"
      {"power_model",	kTIMINGLIB_ATTRENUM_power_model},
#line 652 "attr_lookup"
      {"variable_5_range",	kTIMINGLIB_ATTRENUM_variable_5_range},
#line 289 "attr_lookup"
      {"k_process_internal_power",	kTIMINGLIB_ATTRENUM_k_process_internal_power},
#line 654 "attr_lookup"
      {"variable_7_range",	kTIMINGLIB_ATTRENUM_variable_7_range},
#line 221 "attr_lookup"
      {"index_4",	kTIMINGLIB_ATTRENUM_index_4},
#line 209 "attr_lookup"
      {"high",	kTIMINGLIB_ATTRENUM_high},
#line 653 "attr_lookup"
      {"variable_6_range",	kTIMINGLIB_ATTRENUM_variable_6_range},
#line 488 "attr_lookup"
      {"power_level",	kTIMINGLIB_ATTRENUM_power_level},
#line 220 "attr_lookup"
      {"index_3",	kTIMINGLIB_ATTRENUM_index_3},
#line 306 "attr_lookup"
      {"k_process_rise_propagation",	kTIMINGLIB_ATTRENUM_k_process_rise_propagation},
#line 219 "attr_lookup"
      {"index_2",	kTIMINGLIB_ATTRENUM_index_2},
#line 62 "attr_lookup"
      {"clocked_on",	kTIMINGLIB_ATTRENUM_clocked_on},
#line 285 "attr_lookup"
      {"k_process_fall_propagation",	kTIMINGLIB_ATTRENUM_k_process_fall_propagation},
#line 553 "attr_lookup"
      {"rise_capacitance_range",	kTIMINGLIB_ATTRENUM_rise_capacitance_range},
#line 280 "attr_lookup"
      {"k_process_drive_current",	kTIMINGLIB_ATTRENUM_k_process_drive_current},
#line 217 "attr_lookup"
      {"in_place_swap_mode",	kTIMINGLIB_ATTRENUM_in_place_swap_mode},
#line 167 "attr_lookup"
      {"fall_capacitance_range",	kTIMINGLIB_ATTRENUM_fall_capacitance_range},
#line 524 "attr_lookup"
      {"related_ccb_node",	kTIMINGLIB_ATTRENUM_related_ccb_node},
#line 265 "attr_lookup"
      {"is_pll_feedback_pin",	kTIMINGLIB_ATTRENUM_is_pll_feedback_pin},
#line 554 "attr_lookup"
      {"rise_current_slope_after_threshold",	kTIMINGLIB_ATTRENUM_rise_current_slope_after_threshold},
#line 61 "attr_lookup"
      {"clock_pin",	kTIMINGLIB_ATTRENUM_clock_pin},
#line 168 "attr_lookup"
      {"fall_current_slope_after_threshold",	kTIMINGLIB_ATTRENUM_fall_current_slope_after_threshold},
#line 671 "attr_lookup"
      {"wave_fall",	kTIMINGLIB_ATTRENUM_wave_fall},
#line 468 "attr_lookup"
      {"pg_pin_active_state",	kTIMINGLIB_ATTRENUM_pg_pin_active_state},
#line 54 "attr_lookup"
      {"clock_gate_enable_pin",	kTIMINGLIB_ATTRENUM_clock_gate_enable_pin},
#line 165 "attr_lookup"
      {"failure_rate",	kTIMINGLIB_ATTRENUM_failure_rate},
#line 344 "attr_lookup"
      {"k_temp_rise_propagation",	kTIMINGLIB_ATTRENUM_k_temp_rise_propagation},
#line 63 "attr_lookup"
      {"clocked_on_also",	kTIMINGLIB_ATTRENUM_clocked_on_also},
#line 311 "attr_lookup"
      {"k_process_skew_rise",	kTIMINGLIB_ATTRENUM_k_process_skew_rise},
#line 310 "attr_lookup"
      {"k_process_skew_fall",	kTIMINGLIB_ATTRENUM_k_process_skew_fall},
#line 680 "attr_lookup"
      {"width_coefficient",	kTIMINGLIB_ATTRENUM_width_coefficient},
#line 681 "attr_lookup"
      {"wire_load_from_area",	kTIMINGLIB_ATTRENUM_wire_load_from_area},
#line 428 "attr_lookup"
      {"min_period",	kTIMINGLIB_ATTRENUM_min_period},
#line 44 "attr_lookup"
      {"char_when",	kTIMINGLIB_ATTRENUM_char_when},
#line 55 "attr_lookup"
      {"clock_gate_obs_pin",	kTIMINGLIB_ATTRENUM_clock_gate_obs_pin},
#line 645 "attr_lookup"
      {"variable_1_range",	kTIMINGLIB_ATTRENUM_variable_1_range},
#line 418 "attr_lookup"
      {"max_transition",	kTIMINGLIB_ATTRENUM_max_transition},
#line 567 "attr_lookup"
      {"scan_group",	kTIMINGLIB_ATTRENUM_scan_group},
#line 331 "attr_lookup"
      {"k_temp_intrinsic_rise",	kTIMINGLIB_ATTRENUM_k_temp_intrinsic_rise},
#line 349 "attr_lookup"
      {"k_temp_skew_rise",	kTIMINGLIB_ATTRENUM_k_temp_skew_rise},
#line 330 "attr_lookup"
      {"k_temp_intrinsic_fall",	kTIMINGLIB_ATTRENUM_k_temp_intrinsic_fall},
#line 358 "attr_lookup"
      {"k_volt_drive_current",	kTIMINGLIB_ATTRENUM_k_volt_drive_current},
#line 348 "attr_lookup"
      {"k_temp_skew_fall",	kTIMINGLIB_ATTRENUM_k_temp_skew_fall},
#line 218 "attr_lookup"
      {"index_1",	kTIMINGLIB_ATTRENUM_index_1},
#line 634 "attr_lookup"
      {"type",	kTIMINGLIB_ATTRENUM_type},
#line 296 "attr_lookup"
      {"k_process_nochange_rise",	kTIMINGLIB_ATTRENUM_k_process_nochange_rise},
#line 247 "attr_lookup"
      {"inverted_output",	kTIMINGLIB_ATTRENUM_inverted_output},
#line 551 "attr_lookup"
      {"rf_type",	kTIMINGLIB_ATTRENUM_rf_type},
#line 364 "attr_lookup"
      {"k_volt_fall_transition",	kTIMINGLIB_ATTRENUM_k_volt_fall_transition},
#line 295 "attr_lookup"
      {"k_process_nochange_fall",	kTIMINGLIB_ATTRENUM_k_process_nochange_fall},
#line 362 "attr_lookup"
      {"k_volt_fall_pin_resistance",	kTIMINGLIB_ATTRENUM_k_volt_fall_pin_resistance},
#line 72 "attr_lookup"
      {"contention_condition",	kTIMINGLIB_ATTRENUM_contention_condition},
#line 622 "attr_lookup"
      {"three_state_disable_monitor_node",	kTIMINGLIB_ATTRENUM_three_state_disable_monitor_node},
#line 153 "attr_lookup"
      {"edge_rate_rise",	kTIMINGLIB_ATTRENUM_edge_rate_rise},
#line 163 "attr_lookup"
      {"end_setting",	kTIMINGLIB_ATTRENUM_end_setting},
#line 150 "attr_lookup"
      {"edge_rate_fall",	kTIMINGLIB_ATTRENUM_edge_rate_fall},
#line 555 "attr_lookup"
      {"rise_current_slope_before_threshold",	kTIMINGLIB_ATTRENUM_rise_current_slope_before_threshold},
#line 444 "attr_lookup"
      {"observe_node",	kTIMINGLIB_ATTRENUM_observe_node},
#line 169 "attr_lookup"
      {"fall_current_slope_before_threshold",	kTIMINGLIB_ATTRENUM_fall_current_slope_before_threshold},
#line 276 "attr_lookup"
      {"k_process_cell_degradation",	kTIMINGLIB_ATTRENUM_k_process_cell_degradation},
#line 544 "attr_lookup"
      {"resistive_function",	kTIMINGLIB_ATTRENUM_resistive_function},
#line 353 "attr_lookup"
      {"k_temp_wire_res",	kTIMINGLIB_ATTRENUM_k_temp_wire_res},
#line 542 "attr_lookup"
      {"resistive_0_function",	kTIMINGLIB_ATTRENUM_resistive_0_function},
#line 412 "attr_lookup"
      {"max_capacitance",	kTIMINGLIB_ATTRENUM_max_capacitance},
#line 28 "attr_lookup"
      {"bus_hold_function",	kTIMINGLIB_ATTRENUM_bus_hold_function},
#line 527 "attr_lookup"
      {"related_input",	kTIMINGLIB_ATTRENUM_related_input},
#line 528 "attr_lookup"
      {"related_inputs",	kTIMINGLIB_ATTRENUM_related_inputs},
#line 99 "attr_lookup"
      {"default_intrinsic_rise",	kTIMINGLIB_ATTRENUM_default_intrinsic_rise},
#line 53 "attr_lookup"
      {"clock_gate_clock_pin",	kTIMINGLIB_ATTRENUM_clock_gate_clock_pin},
#line 609 "attr_lookup"
      {"switch_pin",	kTIMINGLIB_ATTRENUM_switch_pin},
#line 98 "attr_lookup"
      {"default_intrinsic_fall",	kTIMINGLIB_ATTRENUM_default_intrinsic_fall},
#line 208 "attr_lookup"
      {"height_coefficient",	kTIMINGLIB_ATTRENUM_height_coefficient},
#line 336 "attr_lookup"
      {"k_temp_nochange_rise",	kTIMINGLIB_ATTRENUM_k_temp_nochange_rise},
#line 382 "attr_lookup"
      {"k_volt_rise_propagation",	kTIMINGLIB_ATTRENUM_k_volt_rise_propagation},
#line 335 "attr_lookup"
      {"k_temp_nochange_fall",	kTIMINGLIB_ATTRENUM_k_temp_nochange_fall},
#line 352 "attr_lookup"
      {"k_temp_wire_cap",	kTIMINGLIB_ATTRENUM_k_temp_wire_cap},
#line 117 "attr_lookup"
      {"default_rise_pin_resistance",	kTIMINGLIB_ATTRENUM_default_rise_pin_resistance},
#line 91 "attr_lookup"
      {"default_fall_pin_resistance",	kTIMINGLIB_ATTRENUM_default_fall_pin_resistance},
#line 120 "attr_lookup"
      {"default_slope_rise",	kTIMINGLIB_ATTRENUM_default_slope_rise},
#line 119 "attr_lookup"
      {"default_slope_fall",	kTIMINGLIB_ATTRENUM_default_slope_fall},
#line 395 "attr_lookup"
      {"key_seed",	kTIMINGLIB_ATTRENUM_key_seed},
#line 660 "attr_lookup"
      {"vimax",	kTIMINGLIB_ATTRENUM_vimax},
#line 394 "attr_lookup"
      {"key_file",	kTIMINGLIB_ATTRENUM_key_file},
#line 228 "attr_lookup"
      {"input_pins",	kTIMINGLIB_ATTRENUM_input_pins},
#line 685 "attr_lookup"
      {"xnf_schnm",	kTIMINGLIB_ATTRENUM_xnf_schnm},
#line 9 "attr_lookup"
      {"active_output_ccb",	kTIMINGLIB_ATTRENUM_active_output_ccb},
#line 523 "attr_lookup"
      {"related_bus_pins",	kTIMINGLIB_ATTRENUM_related_bus_pins},
#line 459 "attr_lookup"
      {"pad_type",	kTIMINGLIB_ATTRENUM_pad_type},
#line 369 "attr_lookup"
      {"k_volt_intrinsic_rise",	kTIMINGLIB_ATTRENUM_k_volt_intrinsic_rise},
#line 387 "attr_lookup"
      {"k_volt_skew_rise",	kTIMINGLIB_ATTRENUM_k_volt_skew_rise},
#line 368 "attr_lookup"
      {"k_volt_intrinsic_fall",	kTIMINGLIB_ATTRENUM_k_volt_intrinsic_fall},
#line 45 "attr_lookup"
      {"clamp_0_function",	kTIMINGLIB_ATTRENUM_clamp_0_function},
#line 386 "attr_lookup"
      {"k_volt_skew_fall",	kTIMINGLIB_ATTRENUM_k_volt_skew_fall},
#line 669 "attr_lookup"
      {"vomax",	kTIMINGLIB_ATTRENUM_vomax},
#line 316 "attr_lookup"
      {"k_temp_cell_degradation",	kTIMINGLIB_ATTRENUM_k_temp_cell_degradation},
#line 213 "attr_lookup"
      {"hysteresis",	kTIMINGLIB_ATTRENUM_hysteresis},
#line 543 "attr_lookup"
      {"resistive_1_function",	kTIMINGLIB_ATTRENUM_resistive_1_function},
#line 114 "attr_lookup"
      {"default_rc_rise_coefficient",	kTIMINGLIB_ATTRENUM_default_rc_rise_coefficient},
#line 445 "attr_lookup"
      {"ocv_arc_depth",	kTIMINGLIB_ATTRENUM_ocv_arc_depth},
#line 115 "attr_lookup"
      {"default_reference_capacitance",	kTIMINGLIB_ATTRENUM_default_reference_capacitance},
#line 48 "attr_lookup"
      {"clamp_z_function",	kTIMINGLIB_ATTRENUM_clamp_z_function},
#line 113 "attr_lookup"
      {"default_rc_fall_coefficient",	kTIMINGLIB_ATTRENUM_default_rc_fall_coefficient},
#line 152 "attr_lookup"
      {"edge_rate_load_rise",	kTIMINGLIB_ATTRENUM_edge_rate_load_rise},
#line 151 "attr_lookup"
      {"edge_rate_load_fall",	kTIMINGLIB_ATTRENUM_edge_rate_load_fall},
#line 205 "attr_lookup"
      {"handle_negative_constraint",	kTIMINGLIB_ATTRENUM_handle_negative_constraint},
#line 263 "attr_lookup"
      {"is_pass_gate",	kTIMINGLIB_ATTRENUM_is_pass_gate},
#line 626 "attr_lookup"
      {"time_unit",	kTIMINGLIB_ATTRENUM_time_unit},
#line 392 "attr_lookup"
      {"key_bit",	kTIMINGLIB_ATTRENUM_key_bit},
#line 85 "attr_lookup"
      {"default_connection_class",	kTIMINGLIB_ATTRENUM_default_connection_class},
#line 391 "attr_lookup"
      {"k_volt_wire_res",	kTIMINGLIB_ATTRENUM_k_volt_wire_res},
#line 237 "attr_lookup"
      {"input_voltage",	kTIMINGLIB_ATTRENUM_input_voltage},
#line 580 "attr_lookup"
      {"simulation",	kTIMINGLIB_ATTRENUM_simulation},
#line 374 "attr_lookup"
      {"k_volt_nochange_rise",	kTIMINGLIB_ATTRENUM_k_volt_nochange_rise},
#line 243 "attr_lookup"
      {"internal_power_calculation",	kTIMINGLIB_ATTRENUM_internal_power_calculation},
#line 373 "attr_lookup"
      {"k_volt_nochange_fall",	kTIMINGLIB_ATTRENUM_k_volt_nochange_fall},
#line 390 "attr_lookup"
      {"k_volt_wire_cap",	kTIMINGLIB_ATTRENUM_k_volt_wire_cap},
#line 556 "attr_lookup"
      {"rise_delay_intercept",	kTIMINGLIB_ATTRENUM_rise_delay_intercept},
#line 566 "attr_lookup"
      {"scaling_factors",	kTIMINGLIB_ATTRENUM_scaling_factors},
#line 170 "attr_lookup"
      {"fall_delay_intercept",	kTIMINGLIB_ATTRENUM_fall_delay_intercept},
#line 178 "attr_lookup"
      {"fanout_length",	kTIMINGLIB_ATTRENUM_fanout_length},
#line 400 "attr_lookup"
      {"level_shifter_type",	kTIMINGLIB_ATTRENUM_level_shifter_type},
#line 46 "attr_lookup"
      {"clamp_1_function",	kTIMINGLIB_ATTRENUM_clamp_1_function},
#line 249 "attr_lookup"
      {"is_clock_gating_cell",	kTIMINGLIB_ATTRENUM_is_clock_gating_cell},
#line 633 "attr_lookup"
      {"tree_type",	kTIMINGLIB_ATTRENUM_tree_type},
#line 427 "attr_lookup"
      {"min_input_noise_width",	kTIMINGLIB_ATTRENUM_min_input_noise_width},
#line 56 "attr_lookup"
      {"clock_gate_out_pin",	kTIMINGLIB_ATTRENUM_clock_gate_out_pin},
#line 536 "attr_lookup"
      {"related_rising_pin",	kTIMINGLIB_ATTRENUM_related_rising_pin},
#line 581 "attr_lookup"
      {"single_bit_degenerate",	kTIMINGLIB_ATTRENUM_single_bit_degenerate},
#line 525 "attr_lookup"
      {"related_falling_pin",	kTIMINGLIB_ATTRENUM_related_falling_pin},
#line 600 "attr_lookup"
      {"steady_state_resistance_float_min",	kTIMINGLIB_ATTRENUM_steady_state_resistance_float_min},
#line 413 "attr_lookup"
      {"max_fanout",	kTIMINGLIB_ATTRENUM_max_fanout},
#line 591 "attr_lookup"
      {"slower_factor",	kTIMINGLIB_ATTRENUM_slower_factor},
#line 67 "attr_lookup"
      {"complementary_pin",	kTIMINGLIB_ATTRENUM_complementary_pin},
#line 148 "attr_lookup"
      {"edge_rate_breakpoint_r0",	kTIMINGLIB_ATTRENUM_edge_rate_breakpoint_r0},
#line 533 "attr_lookup"
      {"related_pg_pin",	kTIMINGLIB_ATTRENUM_related_pg_pin},
#line 146 "attr_lookup"
      {"edge_rate_breakpoint_f0",	kTIMINGLIB_ATTRENUM_edge_rate_breakpoint_f0},
#line 131 "attr_lookup"
      {"delay_model",	kTIMINGLIB_ATTRENUM_delay_model},
#line 354 "attr_lookup"
      {"k_volt_cell_degradation",	kTIMINGLIB_ATTRENUM_k_volt_cell_degradation},
#line 529 "attr_lookup"
      {"related_internal_pg_pin",	kTIMINGLIB_ATTRENUM_related_internal_pg_pin},
#line 27 "attr_lookup"
      {"bit_width",	kTIMINGLIB_ATTRENUM_bit_width},
#line 442 "attr_lookup"
      {"nom_voltage",	kTIMINGLIB_ATTRENUM_nom_voltage},
#line 74 "attr_lookup"
      {"current_unit",	kTIMINGLIB_ATTRENUM_current_unit},
#line 24 "attr_lookup"
      {"base_type",	kTIMINGLIB_ATTRENUM_base_type},
#line 636 "attr_lookup"
      {"unrelated_output_net_capacitance",	kTIMINGLIB_ATTRENUM_unrelated_output_net_capacitance},
#line 304 "attr_lookup"
      {"k_process_rise_delay_intercept",	kTIMINGLIB_ATTRENUM_k_process_rise_delay_intercept},
#line 283 "attr_lookup"
      {"k_process_fall_delay_intercept",	kTIMINGLIB_ATTRENUM_k_process_fall_delay_intercept},
#line 80 "attr_lookup"
      {"data_type",	kTIMINGLIB_ATTRENUM_data_type},
#line 483 "attr_lookup"
      {"poly_convert",	kTIMINGLIB_ATTRENUM_poly_convert},
#line 467 "attr_lookup"
      {"pg_function",	kTIMINGLIB_ATTRENUM_pg_function},
#line 396 "attr_lookup"
      {"key_version",	kTIMINGLIB_ATTRENUM_key_version},
#line 535 "attr_lookup"
      {"related_power_pin",	kTIMINGLIB_ATTRENUM_related_power_pin},
#line 342 "attr_lookup"
      {"k_temp_rise_delay_intercept",	kTIMINGLIB_ATTRENUM_k_temp_rise_delay_intercept},
#line 76 "attr_lookup"
      {"curve_x",	kTIMINGLIB_ATTRENUM_curve_x},
#line 149 "attr_lookup"
      {"edge_rate_breakpoint_r1",	kTIMINGLIB_ATTRENUM_edge_rate_breakpoint_r1},
#line 147 "attr_lookup"
      {"edge_rate_breakpoint_f1",	kTIMINGLIB_ATTRENUM_edge_rate_breakpoint_f1},
#line 474 "attr_lookup"
      {"physical_connection",	kTIMINGLIB_ATTRENUM_physical_connection},
#line 71 "attr_lookup"
      {"constraint_low",	kTIMINGLIB_ATTRENUM_constraint_low},
#line 93 "attr_lookup"
      {"default_hold_coefficient",	kTIMINGLIB_ATTRENUM_default_hold_coefficient},
#line 83 "attr_lookup"
      {"default_cell_failure_rate",	kTIMINGLIB_ATTRENUM_default_cell_failure_rate},
#line 446 "attr_lookup"
      {"ocv_derate_group",	kTIMINGLIB_ATTRENUM_ocv_derate_group},
#line 30 "attr_lookup"
      {"bus_type",	kTIMINGLIB_ATTRENUM_bus_type},
#line 465 "attr_lookup"
      {"path_type",	kTIMINGLIB_ATTRENUM_path_type},
#line 8 "attr_lookup"
      {"active_input_ccb",	kTIMINGLIB_ATTRENUM_active_input_ccb},
#line 271 "attr_lookup"
      {"is_unbuffered",	kTIMINGLIB_ATTRENUM_is_unbuffered},
#line 530 "attr_lookup"
      {"related_output",	kTIMINGLIB_ATTRENUM_related_output},
#line 532 "attr_lookup"
      {"related_outputs",	kTIMINGLIB_ATTRENUM_related_outputs},
#line 42 "attr_lookup"
      {"cell_leakage_power",	kTIMINGLIB_ATTRENUM_cell_leakage_power},
#line 132 "attr_lookup"
      {"derate_type",	kTIMINGLIB_ATTRENUM_derate_type},
#line 96 "attr_lookup"
      {"default_inout_pin_rise_res",	kTIMINGLIB_ATTRENUM_default_inout_pin_rise_res},
#line 493 "attr_lookup"
      {"preferred_input_pad_voltage",	kTIMINGLIB_ATTRENUM_preferred_input_pad_voltage},
#line 95 "attr_lookup"
      {"default_inout_pin_fall_res",	kTIMINGLIB_ATTRENUM_default_inout_pin_fall_res},
#line 301 "attr_lookup"
      {"k_process_recovery_rise",	kTIMINGLIB_ATTRENUM_k_process_recovery_rise},
#line 141 "attr_lookup"
      {"driver_waveform",	kTIMINGLIB_ATTRENUM_driver_waveform},
#line 300 "attr_lookup"
      {"k_process_recovery_fall",	kTIMINGLIB_ATTRENUM_k_process_recovery_fall},
#line 531 "attr_lookup"
      {"related_output_pin",	kTIMINGLIB_ATTRENUM_related_output_pin},
#line 144 "attr_lookup"
      {"driver_waveform_rise",	kTIMINGLIB_ATTRENUM_driver_waveform_rise},
#line 215 "attr_lookup"
      {"illegal_clamp_condition",	kTIMINGLIB_ATTRENUM_illegal_clamp_condition},
#line 142 "attr_lookup"
      {"driver_waveform_fall",	kTIMINGLIB_ATTRENUM_driver_waveform_fall},
#line 97 "attr_lookup"
      {"default_input_pin_cap",	kTIMINGLIB_ATTRENUM_default_input_pin_cap},
#line 479 "attr_lookup"
      {"pin_func_type",	kTIMINGLIB_ATTRENUM_pin_func_type},
#line 37 "attr_lookup"
      {"capacitive_load_unit",	kTIMINGLIB_ATTRENUM_capacitive_load_unit},
#line 94 "attr_lookup"
      {"default_inout_pin_cap",	kTIMINGLIB_ATTRENUM_default_inout_pin_cap},
#line 666 "attr_lookup"
      {"voltage_name",	kTIMINGLIB_ATTRENUM_voltage_name},
#line 537 "attr_lookup"
      {"related_switch_pin",	kTIMINGLIB_ATTRENUM_related_switch_pin},
#line 443 "attr_lookup"
      {"nominal_va_values",	kTIMINGLIB_ATTRENUM_nominal_va_values},
#line 325 "attr_lookup"
      {"k_temp_fall_propagation",	kTIMINGLIB_ATTRENUM_k_temp_fall_propagation},
#line 339 "attr_lookup"
      {"k_temp_recovery_rise",	kTIMINGLIB_ATTRENUM_k_temp_recovery_rise},
#line 338 "attr_lookup"
      {"k_temp_recovery_fall",	kTIMINGLIB_ATTRENUM_k_temp_recovery_fall},
#line 118 "attr_lookup"
      {"default_setup_coefficient",	kTIMINGLIB_ATTRENUM_default_setup_coefficient},
#line 665 "attr_lookup"
      {"voltage_map",	kTIMINGLIB_ATTRENUM_voltage_map},
#line 380 "attr_lookup"
      {"k_volt_rise_delay_intercept",	kTIMINGLIB_ATTRENUM_k_volt_rise_delay_intercept},
#line 406 "attr_lookup"
      {"lsi_rounding_cutoff",	kTIMINGLIB_ATTRENUM_lsi_rounding_cutoff},
#line 682 "attr_lookup"
      {"word_width",	kTIMINGLIB_ATTRENUM_word_width},
#line 475 "attr_lookup"
      {"physical_variant_cells",	kTIMINGLIB_ATTRENUM_physical_variant_cells},
#line 222 "attr_lookup"
      {"index_output",	kTIMINGLIB_ATTRENUM_index_output},
#line 504 "attr_lookup"
      {"pulling_resistance",	kTIMINGLIB_ATTRENUM_pulling_resistance},
#line 620 "attr_lookup"
      {"three_state_disable_current_threshold_rel",	kTIMINGLIB_ATTRENUM_three_state_disable_current_threshold_rel},
#line 164 "attr_lookup"
      {"equal_or_opposite_output",	kTIMINGLIB_ATTRENUM_equal_or_opposite_output},
#line 92 "attr_lookup"
      {"default_fanout_load",	kTIMINGLIB_ATTRENUM_default_fanout_load},
#line 143 "attr_lookup"
      {"driver_waveform_name",	kTIMINGLIB_ATTRENUM_driver_waveform_name},
#line 77 "attr_lookup"
      {"curve_y",	kTIMINGLIB_ATTRENUM_curve_y},
#line 450 "attr_lookup"
      {"output_signal_level",	kTIMINGLIB_ATTRENUM_output_signal_level},
#line 193 "attr_lookup"
      {"fpga_family",	kTIMINGLIB_ATTRENUM_fpga_family},
#line 473 "attr_lookup"
      {"pg_type",	kTIMINGLIB_ATTRENUM_pg_type},
#line 82 "attr_lookup"
      {"dcm_timing",	kTIMINGLIB_ATTRENUM_dcm_timing},
#line 225 "attr_lookup"
      {"input_low_value",	kTIMINGLIB_ATTRENUM_input_low_value},
#line 278 "attr_lookup"
      {"k_process_cell_leakage_power",	kTIMINGLIB_ATTRENUM_k_process_cell_leakage_power},
#line 233 "attr_lookup"
      {"input_stimulus_transition",	kTIMINGLIB_ATTRENUM_input_stimulus_transition},
#line 17 "attr_lookup"
      {"antenna_diode_related_power_pins",	kTIMINGLIB_ATTRENUM_antenna_diode_related_power_pins},
#line 15 "attr_lookup"
      {"always_on",	kTIMINGLIB_ATTRENUM_always_on},
#line 619 "attr_lookup"
      {"three_state_disable_current_threshold_abs",	kTIMINGLIB_ATTRENUM_three_state_disable_current_threshold_abs},
#line 610 "attr_lookup"
      {"switching_interval",	kTIMINGLIB_ATTRENUM_switching_interval},
#line 329 "attr_lookup"
      {"k_temp_internal_power",	kTIMINGLIB_ATTRENUM_k_temp_internal_power},
#line 192 "attr_lookup"
      {"fpga_degenerate_output",	kTIMINGLIB_ATTRENUM_fpga_degenerate_output},
#line 11 "attr_lookup"
      {"address_width",	kTIMINGLIB_ATTRENUM_address_width},
#line 363 "attr_lookup"
      {"k_volt_fall_propagation",	kTIMINGLIB_ATTRENUM_k_volt_fall_propagation},
#line 377 "attr_lookup"
      {"k_volt_recovery_rise",	kTIMINGLIB_ATTRENUM_k_volt_recovery_rise},
#line 455 "attr_lookup"
      {"output_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_output_threshold_pct_rise},
#line 23 "attr_lookup"
      {"base_curves_group",	kTIMINGLIB_ATTRENUM_base_curves_group},
#line 376 "attr_lookup"
      {"k_volt_recovery_fall",	kTIMINGLIB_ATTRENUM_k_volt_recovery_fall},
#line 454 "attr_lookup"
      {"output_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_output_threshold_pct_fall},
#line 621 "attr_lookup"
      {"three_state_disable_measurement_method",	kTIMINGLIB_ATTRENUM_three_state_disable_measurement_method},
#line 160 "attr_lookup"
      {"em_temp_degradation_factor",	kTIMINGLIB_ATTRENUM_em_temp_degradation_factor},
#line 22 "attr_lookup"
      {"base_curve_type",	kTIMINGLIB_ATTRENUM_base_curve_type},
#line 140 "attr_lookup"
      {"driver_type",	kTIMINGLIB_ATTRENUM_driver_type},
#line 70 "attr_lookup"
      {"constraint_high",	kTIMINGLIB_ATTRENUM_constraint_high},
#line 79 "attr_lookup"
      {"data_in_type",	kTIMINGLIB_ATTRENUM_data_in_type},
#line 500 "attr_lookup"
      {"propagating_ccb",	kTIMINGLIB_ATTRENUM_propagating_ccb},
#line 635 "attr_lookup"
      {"typical_capacitances",	kTIMINGLIB_ATTRENUM_typical_capacitances},
#line 484 "attr_lookup"
      {"power_cell_type",	kTIMINGLIB_ATTRENUM_power_cell_type},
#line 122 "attr_lookup"
      {"default_timing",	kTIMINGLIB_ATTRENUM_default_timing},
#line 112 "attr_lookup"
      {"default_power_rail",	kTIMINGLIB_ATTRENUM_default_power_rail},
#line 318 "attr_lookup"
      {"k_temp_cell_leakage_power",	kTIMINGLIB_ATTRENUM_k_temp_cell_leakage_power},
#line 190 "attr_lookup"
      {"fpga_cell_type",	kTIMINGLIB_ATTRENUM_fpga_cell_type},
#line 501 "attr_lookup"
      {"pull_down_function",	kTIMINGLIB_ATTRENUM_pull_down_function},
#line 676 "attr_lookup"
      {"wave_rise_timing_interval",	kTIMINGLIB_ATTRENUM_wave_rise_timing_interval},
#line 502 "attr_lookup"
      {"pull_up_function",	kTIMINGLIB_ATTRENUM_pull_up_function},
#line 494 "attr_lookup"
      {"preferred_output_pad_slew_rate_control",	kTIMINGLIB_ATTRENUM_preferred_output_pad_slew_rate_control},
#line 417 "attr_lookup"
      {"max_time_borrow",	kTIMINGLIB_ATTRENUM_max_time_borrow},
#line 526 "attr_lookup"
      {"related_ground_pin",	kTIMINGLIB_ATTRENUM_related_ground_pin},
#line 216 "attr_lookup"
      {"illegal_transition_if_undefined",	kTIMINGLIB_ATTRENUM_illegal_transition_if_undefined},
#line 668 "attr_lookup"
      {"voltage_unit",	kTIMINGLIB_ATTRENUM_voltage_unit},
#line 407 "attr_lookup"
      {"lsi_rounding_digit",	kTIMINGLIB_ATTRENUM_lsi_rounding_digit},
#line 238 "attr_lookup"
      {"input_voltage_range",	kTIMINGLIB_ATTRENUM_input_voltage_range},
#line 367 "attr_lookup"
      {"k_volt_internal_power",	kTIMINGLIB_ATTRENUM_k_volt_internal_power},
#line 604 "attr_lookup"
      {"steady_state_resistance_low",	kTIMINGLIB_ATTRENUM_steady_state_resistance_low},
#line 232 "attr_lookup"
      {"input_stimulus_interval",	kTIMINGLIB_ATTRENUM_input_stimulus_interval},
#line 587 "attr_lookup"
      {"slew_upper_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_slew_upper_threshold_pct_rise},
#line 124 "attr_lookup"
      {"default_wire_load",	kTIMINGLIB_ATTRENUM_default_wire_load},
#line 586 "attr_lookup"
      {"slew_upper_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_slew_upper_threshold_pct_fall},
#line 125 "attr_lookup"
      {"default_wire_load_area",	kTIMINGLIB_ATTRENUM_default_wire_load_area},
#line 466 "attr_lookup"
      {"permit_power_down",	kTIMINGLIB_ATTRENUM_permit_power_down},
#line 608 "attr_lookup"
      {"switch_function",	kTIMINGLIB_ATTRENUM_switch_function},
#line 614 "attr_lookup"
      {"technology",	kTIMINGLIB_ATTRENUM_technology},
#line 197 "attr_lookup"
      {"fpga_min_degen_input_size",	kTIMINGLIB_ATTRENUM_fpga_min_degen_input_size},
#line 627 "attr_lookup"
      {"timing_model_type",	kTIMINGLIB_ATTRENUM_timing_model_type},
#line 18 "attr_lookup"
      {"antenna_diode_type",	kTIMINGLIB_ATTRENUM_antenna_diode_type},
#line 128 "attr_lookup"
      {"default_wire_load_resistance",	kTIMINGLIB_ATTRENUM_default_wire_load_resistance},
#line 393 "attr_lookup"
      {"key_feature",	kTIMINGLIB_ATTRENUM_key_feature},
#line 485 "attr_lookup"
      {"power_down_function",	kTIMINGLIB_ATTRENUM_power_down_function},
#line 673 "attr_lookup"
      {"wave_fall_timing_interval",	kTIMINGLIB_ATTRENUM_wave_fall_timing_interval},
#line 110 "attr_lookup"
      {"default_output_pin_rise_res",	kTIMINGLIB_ATTRENUM_default_output_pin_rise_res},
#line 618 "attr_lookup"
      {"three_state_cap_add_to_load_index",	kTIMINGLIB_ATTRENUM_three_state_cap_add_to_load_index},
#line 109 "attr_lookup"
      {"default_output_pin_fall_res",	kTIMINGLIB_ATTRENUM_default_output_pin_fall_res},
#line 129 "attr_lookup"
      {"default_wire_load_selection",	kTIMINGLIB_ATTRENUM_default_wire_load_selection},
#line 356 "attr_lookup"
      {"k_volt_cell_leakage_power",	kTIMINGLIB_ATTRENUM_k_volt_cell_leakage_power},
#line 495 "attr_lookup"
      {"preferred_output_pad_voltage",	kTIMINGLIB_ATTRENUM_preferred_output_pad_voltage},
#line 107 "attr_lookup"
      {"default_operating_conditions",	kTIMINGLIB_ATTRENUM_default_operating_conditions},
#line 470 "attr_lookup"
      {"pg_setting",	kTIMINGLIB_ATTRENUM_pg_setting},
#line 126 "attr_lookup"
      {"default_wire_load_capacitance",	kTIMINGLIB_ATTRENUM_default_wire_load_capacitance},
#line 606 "attr_lookup"
      {"steady_state_resistance_low_min",	kTIMINGLIB_ATTRENUM_steady_state_resistance_low_min},
#line 108 "attr_lookup"
      {"default_output_pin_cap",	kTIMINGLIB_ATTRENUM_default_output_pin_cap},
#line 229 "attr_lookup"
      {"input_signal_level",	kTIMINGLIB_ATTRENUM_input_signal_level},
#line 503 "attr_lookup"
      {"pulling_current",	kTIMINGLIB_ATTRENUM_pulling_current},
#line 487 "attr_lookup"
      {"power_gating_pin",	kTIMINGLIB_ATTRENUM_power_gating_pin},
#line 486 "attr_lookup"
      {"power_gating_cell",	kTIMINGLIB_ATTRENUM_power_gating_cell},
#line 607 "attr_lookup"
      {"switch_cell_type",	kTIMINGLIB_ATTRENUM_switch_cell_type},
#line 583 "attr_lookup"
      {"slew_derate_from_library",	kTIMINGLIB_ATTRENUM_slew_derate_from_library},
#line 505 "attr_lookup"
      {"pulling_resistance_unit",	kTIMINGLIB_ATTRENUM_pulling_resistance_unit},
#line 36 "attr_lookup"
      {"capacitance_voltage_upper_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_capacitance_voltage_upper_threshold_pct_rise},
#line 158 "attr_lookup"
      {"edge_type",	kTIMINGLIB_ATTRENUM_edge_type},
#line 35 "attr_lookup"
      {"capacitance_voltage_upper_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_capacitance_voltage_upper_threshold_pct_fall},
#line 579 "attr_lookup"
      {"signal_type",	kTIMINGLIB_ATTRENUM_signal_type},
#line 683 "attr_lookup"
      {"x_function",	kTIMINGLIB_ATTRENUM_x_function},
#line 684 "attr_lookup"
      {"xnf_device_name",	kTIMINGLIB_ATTRENUM_xnf_device_name},
#line 323 "attr_lookup"
      {"k_temp_fall_delay_intercept",	kTIMINGLIB_ATTRENUM_k_temp_fall_delay_intercept},
#line 522 "attr_lookup"
      {"related_bus_equivalent",	kTIMINGLIB_ATTRENUM_related_bus_equivalent},
#line 73 "attr_lookup"
      {"current_type",	kTIMINGLIB_ATTRENUM_current_type},
#line 585 "attr_lookup"
      {"slew_lower_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_slew_lower_threshold_pct_rise},
#line 88 "attr_lookup"
      {"default_edge_rate_breakpoint_r0",	kTIMINGLIB_ATTRENUM_default_edge_rate_breakpoint_r0},
#line 584 "attr_lookup"
      {"slew_lower_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_slew_lower_threshold_pct_fall},
#line 86 "attr_lookup"
      {"default_edge_rate_breakpoint_f0",	kTIMINGLIB_ATTRENUM_default_edge_rate_breakpoint_f0},
#line 103 "attr_lookup"
      {"default_max_transition",	kTIMINGLIB_ATTRENUM_default_max_transition},
#line 667 "attr_lookup"
      {"voltage_state_range",	kTIMINGLIB_ATTRENUM_voltage_state_range},
#line 58 "attr_lookup"
      {"clock_gating_flag",	kTIMINGLIB_ATTRENUM_clock_gating_flag},
#line 401 "attr_lookup"
      {"library_features",	kTIMINGLIB_ATTRENUM_library_features},
#line 16 "attr_lookup"
      {"antenna_diode_related_ground_pins",	kTIMINGLIB_ATTRENUM_antenna_diode_related_ground_pins},
#line 456 "attr_lookup"
      {"output_voltage",	kTIMINGLIB_ATTRENUM_output_voltage},
#line 430 "attr_lookup"
      {"min_pulse_width_low",	kTIMINGLIB_ATTRENUM_min_pulse_width_low},
#line 637 "attr_lookup"
      {"use_for_size_only",	kTIMINGLIB_ATTRENUM_use_for_size_only},
#line 127 "attr_lookup"
      {"default_wire_load_mode",	kTIMINGLIB_ATTRENUM_default_wire_load_mode},
#line 101 "attr_lookup"
      {"default_max_capacitance",	kTIMINGLIB_ATTRENUM_default_max_capacitance},
#line 630 "attr_lookup"
      {"timing_type",	kTIMINGLIB_ATTRENUM_timing_type},
#line 601 "attr_lookup"
      {"steady_state_resistance_high",	kTIMINGLIB_ATTRENUM_steady_state_resistance_high},
#line 416 "attr_lookup"
      {"max_input_noise_width",	kTIMINGLIB_ATTRENUM_max_input_noise_width},
#line 194 "attr_lookup"
      {"fpga_lut_insert_before_sequential",	kTIMINGLIB_ATTRENUM_fpga_lut_insert_before_sequential},
#line 599 "attr_lookup"
      {"steady_state_resistance_float_max",	kTIMINGLIB_ATTRENUM_steady_state_resistance_float_max},
#line 472 "attr_lookup"
      {"pg_setting_condition",	kTIMINGLIB_ATTRENUM_pg_setting_condition},
#line 89 "attr_lookup"
      {"default_edge_rate_breakpoint_r1",	kTIMINGLIB_ATTRENUM_default_edge_rate_breakpoint_r1},
#line 123 "attr_lookup"
      {"default_value_selection_method",	kTIMINGLIB_ATTRENUM_default_value_selection_method},
#line 87 "attr_lookup"
      {"default_edge_rate_breakpoint_f1",	kTIMINGLIB_ATTRENUM_default_edge_rate_breakpoint_f1},
#line 195 "attr_lookup"
      {"fpga_lut_output",	kTIMINGLIB_ATTRENUM_fpga_lut_output},
#line 361 "attr_lookup"
      {"k_volt_fall_delay_intercept",	kTIMINGLIB_ATTRENUM_k_volt_fall_delay_intercept},
#line 514 "attr_lookup"
      {"receiver_capacitance2_voltage_upper_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_receiver_capacitance2_voltage_upper_threshold_pct_rise},
#line 513 "attr_lookup"
      {"receiver_capacitance2_voltage_upper_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_receiver_capacitance2_voltage_upper_threshold_pct_fall},
#line 59 "attr_lookup"
      {"clock_gating_integrated_cell",	kTIMINGLIB_ATTRENUM_clock_gating_integrated_cell},
#line 603 "attr_lookup"
      {"steady_state_resistance_high_min",	kTIMINGLIB_ATTRENUM_steady_state_resistance_high_min},
#line 134 "attr_lookup"
      {"divided_by",	kTIMINGLIB_ATTRENUM_divided_by},
#line 234 "attr_lookup"
      {"input_switching_condition",	kTIMINGLIB_ATTRENUM_input_switching_condition},
#line 471 "attr_lookup"
      {"pg_setting_active_state",	kTIMINGLIB_ATTRENUM_pg_setting_active_state},
#line 191 "attr_lookup"
      {"fpga_complex_degenerate",	kTIMINGLIB_ATTRENUM_fpga_complex_degenerate},
#line 616 "attr_lookup"
      {"test_output_only",	kTIMINGLIB_ATTRENUM_test_output_only},
#line 13 "attr_lookup"
      {"alive_during_power_up",	kTIMINGLIB_ATTRENUM_alive_during_power_up},
#line 334 "attr_lookup"
      {"k_temp_min_pulse_width_low",	kTIMINGLIB_ATTRENUM_k_temp_min_pulse_width_low},
#line 34 "attr_lookup"
      {"capacitance_voltage_lower_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_capacitance_voltage_lower_threshold_pct_rise},
#line 438 "attr_lookup"
      {"nextstate_type",	kTIMINGLIB_ATTRENUM_nextstate_type},
#line 33 "attr_lookup"
      {"capacitance_voltage_lower_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_capacitance_voltage_lower_threshold_pct_fall},
#line 116 "attr_lookup"
      {"default_rise_delay_intercept",	kTIMINGLIB_ATTRENUM_default_rise_delay_intercept},
#line 199 "attr_lookup"
      {"fpga_timing_type",	kTIMINGLIB_ATTRENUM_fpga_timing_type},
#line 90 "attr_lookup"
      {"default_fall_delay_intercept",	kTIMINGLIB_ATTRENUM_default_fall_delay_intercept},
#line 510 "attr_lookup"
      {"receiver_capacitance1_voltage_upper_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_receiver_capacitance1_voltage_upper_threshold_pct_rise},
#line 111 "attr_lookup"
      {"default_pg_setting",	kTIMINGLIB_ATTRENUM_default_pg_setting},
#line 509 "attr_lookup"
      {"receiver_capacitance1_voltage_upper_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_receiver_capacitance1_voltage_upper_threshold_pct_fall},
#line 105 "attr_lookup"
      {"default_min_porosity",	kTIMINGLIB_ATTRENUM_default_min_porosity},
#line 294 "attr_lookup"
      {"k_process_min_pulse_width_low",	kTIMINGLIB_ATTRENUM_k_process_min_pulse_width_low},
#line 452 "attr_lookup"
      {"output_signal_level_low",	kTIMINGLIB_ATTRENUM_output_signal_level_low},
#line 156 "attr_lookup"
      {"edge_rate_sensitivity_r0",	kTIMINGLIB_ATTRENUM_edge_rate_sensitivity_r0},
#line 154 "attr_lookup"
      {"edge_rate_sensitivity_f0",	kTIMINGLIB_ATTRENUM_edge_rate_sensitivity_f0},
#line 429 "attr_lookup"
      {"min_pulse_width_high",	kTIMINGLIB_ATTRENUM_min_pulse_width_high},
#line 21 "attr_lookup"
      {"auxiliary_pad_cell",	kTIMINGLIB_ATTRENUM_auxiliary_pad_cell},
#line 547 "attr_lookup"
      {"restore_edge_type",	kTIMINGLIB_ATTRENUM_restore_edge_type},
#line 372 "attr_lookup"
      {"k_volt_min_pulse_width_low",	kTIMINGLIB_ATTRENUM_k_volt_min_pulse_width_low},
#line 397 "attr_lookup"
      {"leakage_power_unit",	kTIMINGLIB_ATTRENUM_leakage_power_unit},
#line 198 "attr_lookup"
      {"fpga_prefer_undegenerated_gates",	kTIMINGLIB_ATTRENUM_fpga_prefer_undegenerated_gates},
#line 102 "attr_lookup"
      {"default_max_fanout",	kTIMINGLIB_ATTRENUM_default_max_fanout},
#line 436 "attr_lookup"
      {"multiplied_by",	kTIMINGLIB_ATTRENUM_multiplied_by},
#line 157 "attr_lookup"
      {"edge_rate_sensitivity_r1",	kTIMINGLIB_ATTRENUM_edge_rate_sensitivity_r1},
#line 155 "attr_lookup"
      {"edge_rate_sensitivity_f1",	kTIMINGLIB_ATTRENUM_edge_rate_sensitivity_f1},
#line 453 "attr_lookup"
      {"output_switching_condition",	kTIMINGLIB_ATTRENUM_output_switching_condition},
#line 597 "attr_lookup"
      {"steady_state_resistance_above_high",	kTIMINGLIB_ATTRENUM_steady_state_resistance_above_high},
#line 512 "attr_lookup"
      {"receiver_capacitance2_voltage_lower_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_receiver_capacitance2_voltage_lower_threshold_pct_rise},
#line 104 "attr_lookup"
      {"default_max_utilization",	kTIMINGLIB_ATTRENUM_default_max_utilization},
#line 511 "attr_lookup"
      {"receiver_capacitance2_voltage_lower_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_receiver_capacitance2_voltage_lower_threshold_pct_fall},
#line 624 "attr_lookup"
      {"threshold_voltage_group",	kTIMINGLIB_ATTRENUM_threshold_voltage_group},
#line 106 "attr_lookup"
      {"default_ocv_derate_group",	kTIMINGLIB_ATTRENUM_default_ocv_derate_group},
#line 333 "attr_lookup"
      {"k_temp_min_pulse_width_high",	kTIMINGLIB_ATTRENUM_k_temp_min_pulse_width_high},
#line 189 "attr_lookup"
      {"fpga_bridge_inputs",	kTIMINGLIB_ATTRENUM_fpga_bridge_inputs},
#line 675 "attr_lookup"
      {"wave_rise_sampling_index",	kTIMINGLIB_ATTRENUM_wave_rise_sampling_index},
#line 84 "attr_lookup"
      {"default_cell_leakage_power",	kTIMINGLIB_ATTRENUM_default_cell_leakage_power},
#line 497 "attr_lookup"
      {"primary_output",	kTIMINGLIB_ATTRENUM_primary_output},
#line 508 "attr_lookup"
      {"receiver_capacitance1_voltage_lower_threshold_pct_rise",	kTIMINGLIB_ATTRENUM_receiver_capacitance1_voltage_lower_threshold_pct_rise},
#line 507 "attr_lookup"
      {"receiver_capacitance1_voltage_lower_threshold_pct_fall",	kTIMINGLIB_ATTRENUM_receiver_capacitance1_voltage_lower_threshold_pct_fall},
#line 293 "attr_lookup"
      {"k_process_min_pulse_width_high",	kTIMINGLIB_ATTRENUM_k_process_min_pulse_width_high},
#line 451 "attr_lookup"
      {"output_signal_level_high",	kTIMINGLIB_ATTRENUM_output_signal_level_high},
#line 598 "attr_lookup"
      {"steady_state_resistance_below_low",	kTIMINGLIB_ATTRENUM_steady_state_resistance_below_low},
#line 562 "attr_lookup"
      {"routing_layers",	kTIMINGLIB_ATTRENUM_routing_layers},
#line 672 "attr_lookup"
      {"wave_fall_sampling_index",	kTIMINGLIB_ATTRENUM_wave_fall_sampling_index},
#line 639 "attr_lookup"
      {"user_pg_type",	kTIMINGLIB_ATTRENUM_user_pg_type},
#line 145 "attr_lookup"
      {"duty_cycle",	kTIMINGLIB_ATTRENUM_duty_cycle},
#line 231 "attr_lookup"
      {"input_signal_level_low",	kTIMINGLIB_ATTRENUM_input_signal_level_low},
#line 371 "attr_lookup"
      {"k_volt_min_pulse_width_high",	kTIMINGLIB_ATTRENUM_k_volt_min_pulse_width_high},
#line 12 "attr_lookup"
      {"alive_during_partial_power_down",	kTIMINGLIB_ATTRENUM_alive_during_partial_power_down},
#line 224 "attr_lookup"
      {"input_high_value",	kTIMINGLIB_ATTRENUM_input_high_value},
#line 38 "attr_lookup"
      {"ccs_timing_delay_tolerance_rel",	kTIMINGLIB_ATTRENUM_ccs_timing_delay_tolerance_rel},
#line 457 "attr_lookup"
      {"output_voltage_range",	kTIMINGLIB_ATTRENUM_output_voltage_range},
#line 605 "attr_lookup"
      {"steady_state_resistance_low_max",	kTIMINGLIB_ATTRENUM_steady_state_resistance_low_max},
#line 561 "attr_lookup"
      {"rising_together_group",	kTIMINGLIB_ATTRENUM_rising_together_group},
#line 414 "attr_lookup"
      {"max_input_delta_overdrive_high",	kTIMINGLIB_ATTRENUM_max_input_delta_overdrive_high},
#line 196 "attr_lookup"
      {"fpga_max_degen_input_size",	kTIMINGLIB_ATTRENUM_fpga_max_degen_input_size},
#line 230 "attr_lookup"
      {"input_signal_level_high",	kTIMINGLIB_ATTRENUM_input_signal_level_high},
#line 29 "attr_lookup"
      {"bus_naming_style",	kTIMINGLIB_ATTRENUM_bus_naming_style},
#line 175 "attr_lookup"
      {"falling_together_group",	kTIMINGLIB_ATTRENUM_falling_together_group},
#line 415 "attr_lookup"
      {"max_input_delta_underdrive_high",	kTIMINGLIB_ATTRENUM_max_input_delta_underdrive_high},
#line 602 "attr_lookup"
      {"steady_state_resistance_high_max",	kTIMINGLIB_ATTRENUM_steady_state_resistance_high_max},
#line 40 "attr_lookup"
      {"ccs_timing_voltage_margin_tolerance_rel",	kTIMINGLIB_ATTRENUM_ccs_timing_voltage_margin_tolerance_rel},
#line 39 "attr_lookup"
      {"ccs_timing_segment_voltage_tolerance_rel",	kTIMINGLIB_ATTRENUM_ccs_timing_segment_voltage_tolerance_rel},
#line 611 "attr_lookup"
      {"switching_together_group",	kTIMINGLIB_ATTRENUM_switching_together_group},
#line 188 "attr_lookup"
      {"fpga_allow_duplicate_degenerated_gates",	kTIMINGLIB_ATTRENUM_fpga_allow_duplicate_degenerated_gates},
#line 204 "attr_lookup"
      {"ground_output_voltage_range",	kTIMINGLIB_ATTRENUM_ground_output_voltage_range},
#line 121 "attr_lookup"
      {"default_threshold_voltage_group",	kTIMINGLIB_ATTRENUM_default_threshold_voltage_group},
#line 100 "attr_lookup"
      {"default_leakage_power_density",	kTIMINGLIB_ATTRENUM_default_leakage_power_density},
#line 203 "attr_lookup"
      {"ground_input_voltage_range",	kTIMINGLIB_ATTRENUM_ground_input_voltage_range},
#line 214 "attr_lookup"
      {"ignore_verify_icg_type",	kTIMINGLIB_ATTRENUM_ignore_verify_icg_type}
    };

  static const short lookup[] =
    {
       -1,  -1,  -1,  -1,   0,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        1,  -1,  -1,  -1,   2,  -1,   3,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   4,
        5,   6,  -1,  -1,  -1,   7,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,   8,   9,  10,  -1,  -1,  -1,  -1,
       11,  -1,  -1,  -1,  -1,  12,  -1,  -1,  -1,  -1,
       -1,  13,  -1,  -1,  -1,  14,  -1,  -1,  15,  -1,
       16,  -1,  17,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  18,
       19,  -1,  -1,  -1,  20,  -1,  -1,  -1,  -1,  21,
       -1,  22,  23,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  24,  -1,  25,  26,
       -1,  27,  28,  -1,  -1,  -1,  29,  -1,  -1,  30,
       -1,  31,  32,  -1,  33,  34,  -1,  -1,  -1,  -1,
       35,  36,  -1,  -1,  -1,  37,  -1,  -1,  -1,  -1,
       38,  -1,  -1,  39,  40,  -1,  -1,  41,  -1,  -1,
       42,  43,  44,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       45,  46,  -1,  47,  -1,  48,  -1,  -1,  49,  -1,
       -1,  -1,  -1,  -1,  -1,  50,  -1,  -1,  51,  52,
       -1,  53,  54,  55,  56,  -1,  57,  -1,  58,  -1,
       -1,  -1,  -1,  59,  60,  61,  -1,  -1,  62,  63,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  64,  -1,  -1,
       -1,  65,  -1,  -1,  66,  67,  68,  -1,  -1,  -1,
       69,  70,  -1,  -1,  71,  -1,  72,  -1,  -1,  -1,
       -1,  -1,  -1,  73,  -1,  -1,  -1,  -1,  74,  -1,
       75,  -1,  -1,  -1,  -1,  76,  77,  -1,  -1,  78,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  79,  80,
       81,  -1,  -1,  82,  83,  -1,  -1,  84,  -1,  85,
       -1,  -1,  -1,  -1,  86,  -1,  -1,  -1,  87,  -1,
       -1,  -1,  88,  89,  -1,  90,  -1,  91,  -1,  -1,
       -1,  -1,  92,  -1,  -1,  93,  -1,  -1,  -1,  94,
       95,  -1,  -1,  -1,  96,  97,  -1,  98,  -1,  -1,
       99, 100, 101, 102,  -1,  -1, 103, 104,  -1,  -1,
       -1,  -1, 105,  -1,  -1,  -1, 106,  -1,  -1,  -1,
       -1,  -1,  -1, 107, 108, 109,  -1,  -1,  -1, 110,
       -1,  -1, 111,  -1,  -1, 112, 113,  -1,  -1,  -1,
      114,  -1,  -1,  -1,  -1,  -1,  -1, 115, 116,  -1,
      117,  -1, 118,  -1,  -1,  -1,  -1,  -1, 119,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 120,
       -1,  -1,  -1,  -1,  -1,  -1, 121,  -1,  -1,  -1,
       -1, 122,  -1,  -1,  -1,  -1, 123,  -1,  -1,  -1,
       -1, 124,  -1,  -1,  -1, 125, 126,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 127,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 128,  -1,  -1,  -1,  -1,
      129,  -1,  -1,  -1,  -1, 130,  -1,  -1,  -1,  -1,
      131,  -1, 132, 133,  -1,  -1, 134,  -1,  -1,  -1,
       -1, 135,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 136,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      137,  -1, 138,  -1,  -1,  -1,  -1,  -1, 139,  -1,
       -1,  -1,  -1,  -1,  -1, 140, 141,  -1, 142,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      143,  -1,  -1,  -1,  -1,  -1,  -1, 144,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 145,  -1,  -1,
       -1, 146,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 147,  -1, 148,  -1,  -1, 149, 150,  -1,
       -1,  -1, 151,  -1,  -1, 152,  -1,  -1, 153,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 154, 155,  -1,  -1,
       -1,  -1, 156,  -1,  -1,  -1, 157,  -1, 158, 159,
      160,  -1,  -1,  -1,  -1, 161, 162,  -1,  -1,  -1,
      163, 164,  -1,  -1,  -1, 165, 166,  -1, 167,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 168,  -1,  -1,  -1,  -1, 169,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 170,
       -1, 171,  -1,  -1,  -1,  -1, 172,  -1,  -1,  -1,
      173, 174,  -1,  -1,  -1, 175,  -1,  -1,  -1,  -1,
      176,  -1,  -1,  -1,  -1,  -1,  -1, 177,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 178,  -1,  -1,  -1,  -1, 179,
       -1,  -1,  -1, 180, 181,  -1,  -1, 182, 183,  -1,
       -1,  -1, 184,  -1, 185, 186, 187,  -1,  -1,  -1,
       -1, 188,  -1,  -1,  -1, 189,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 190, 191,  -1, 192,  -1, 193,  -1,
      194, 195,  -1, 196,  -1, 197,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 198,  -1,  -1,  -1,  -1, 199,
       -1,  -1, 200,  -1,  -1,  -1,  -1, 201,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 202,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 203,  -1,  -1,  -1,
       -1, 204, 205,  -1,  -1,  -1,  -1, 206, 207,  -1,
       -1,  -1,  -1,  -1, 208,  -1,  -1,  -1,  -1, 209,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 210,  -1,  -1,  -1,  -1,
       -1,  -1, 211,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 212,  -1, 213,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 214,  -1,  -1,  -1,  -1,  -1,  -1, 215,
       -1,  -1, 216,  -1,  -1,  -1, 217, 218,  -1, 219,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 220,  -1,  -1,
       -1,  -1,  -1, 221,  -1,  -1,  -1,  -1,  -1, 222,
      223, 224, 225,  -1, 226,  -1,  -1,  -1, 227,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 228,  -1, 229,  -1,  -1,  -1, 230,
       -1, 231, 232,  -1, 233, 234,  -1, 235,  -1, 236,
       -1,  -1,  -1, 237, 238,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 239,  -1,  -1,  -1,  -1,  -1,
      240,  -1,  -1, 241,  -1,  -1, 242,  -1, 243, 244,
       -1,  -1,  -1,  -1, 245, 246,  -1, 247, 248, 249,
       -1, 250, 251,  -1,  -1,  -1,  -1, 252,  -1, 253,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 254,  -1,  -1,  -1,  -1,  -1,  -1, 255,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 256,  -1,  -1,
      257,  -1, 258,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 259,  -1,  -1, 260, 261,
       -1,  -1, 262, 263,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 264,  -1,  -1,  -1, 265,  -1,  -1,  -1, 266,
       -1,  -1, 267,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 268, 269, 270,  -1,  -1, 271,
       -1,  -1,  -1,  -1, 272,  -1,  -1,  -1, 273, 274,
      275, 276, 277,  -1,  -1,  -1, 278,  -1,  -1,  -1,
       -1, 279,  -1,  -1, 280, 281, 282, 283,  -1,  -1,
       -1, 284, 285,  -1,  -1,  -1, 286,  -1,  -1,  -1,
      287, 288, 289,  -1, 290,  -1, 291, 292,  -1,  -1,
       -1,  -1, 293,  -1,  -1,  -1, 294, 295,  -1,  -1,
      296, 297,  -1,  -1,  -1,  -1,  -1, 298, 299, 300,
       -1,  -1, 301,  -1,  -1,  -1, 302,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 303,  -1,  -1,  -1,  -1, 304,
       -1,  -1, 305,  -1, 306, 307,  -1,  -1,  -1, 308,
       -1, 309, 310, 311,  -1,  -1,  -1,  -1,  -1,  -1,
      312,  -1,  -1,  -1,  -1, 313,  -1,  -1,  -1,  -1,
      314,  -1,  -1, 315,  -1, 316,  -1,  -1, 317,  -1,
       -1,  -1,  -1,  -1,  -1, 318,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 319,  -1,  -1, 320,  -1,  -1, 321,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      322,  -1,  -1,  -1, 323,  -1,  -1, 324,  -1, 325,
      326,  -1, 327,  -1,  -1,  -1,  -1, 328, 329,  -1,
       -1,  -1,  -1, 330,  -1, 331, 332, 333, 334,  -1,
       -1, 335,  -1, 336,  -1,  -1,  -1, 337,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 338,
       -1, 339,  -1,  -1, 340, 341,  -1, 342,  -1,  -1,
      343,  -1,  -1,  -1,  -1,  -1, 344,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 345, 346, 347,  -1,  -1,  -1,
      348,  -1, 349,  -1,  -1,  -1,  -1,  -1, 350, 351,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 352,  -1,  -1, 353, 354, 355, 356,  -1,
       -1,  -1,  -1,  -1,  -1, 357,  -1,  -1, 358,  -1,
      359,  -1,  -1,  -1,  -1, 360,  -1,  -1,  -1,  -1,
       -1,  -1, 361,  -1,  -1,  -1,  -1, 362, 363,  -1,
       -1,  -1,  -1, 364,  -1,  -1,  -1, 365,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 366,  -1, 367, 368,  -1,
       -1,  -1,  -1,  -1, 369,  -1,  -1, 370,  -1,  -1,
       -1, 371, 372,  -1,  -1,  -1,  -1,  -1,  -1, 373,
       -1,  -1, 374,  -1, 375,  -1, 376, 377,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      378,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 379, 380,
       -1, 381, 382, 383, 384,  -1, 385, 386,  -1, 387,
       -1,  -1,  -1,  -1, 388,  -1, 389, 390,  -1,  -1,
       -1,  -1, 391,  -1,  -1,  -1, 392,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 393,  -1,  -1,  -1,  -1,  -1,
      394,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 395,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 396,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      397,  -1, 398,  -1,  -1, 399,  -1,  -1,  -1,  -1,
      400,  -1,  -1,  -1, 401, 402,  -1,  -1,  -1, 403,
       -1,  -1,  -1, 404,  -1,  -1,  -1, 405,  -1,  -1,
       -1, 406,  -1,  -1,  -1, 407,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 408,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 409, 410,  -1,
       -1,  -1,  -1, 411,  -1,  -1, 412,  -1,  -1, 413,
       -1,  -1, 414,  -1,  -1, 415,  -1,  -1,  -1, 416,
       -1, 417,  -1, 418, 419,  -1,  -1,  -1, 420,  -1,
      421,  -1,  -1, 422,  -1,  -1,  -1,  -1, 423,  -1,
       -1,  -1,  -1,  -1,  -1, 424,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 425, 426,  -1,  -1,  -1,  -1,  -1, 427,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 428,  -1, 429,
       -1,  -1,  -1,  -1, 430,  -1,  -1,  -1, 431,  -1,
       -1, 432,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 433,  -1,  -1,  -1, 434,  -1,  -1, 435,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 436,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 437, 438,  -1,
       -1,  -1,  -1, 439,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 440,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 441, 442, 443, 444, 445,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 446,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 447,
       -1,  -1,  -1, 448, 449, 450,  -1,  -1,  -1, 451,
      452,  -1,  -1,  -1,  -1,  -1, 453, 454,  -1,  -1,
       -1, 455,  -1,  -1,  -1,  -1,  -1, 456,  -1,  -1,
      457,  -1, 458, 459,  -1, 460,  -1,  -1, 461,  -1,
      462, 463, 464,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 465,  -1,
       -1, 466, 467,  -1, 468,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 469, 470, 471,  -1,  -1,  -1,  -1, 472,
      473,  -1,  -1,  -1,  -1,  -1, 474,  -1,  -1,  -1,
       -1, 475,  -1,  -1, 476,  -1,  -1, 477,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 478, 479,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 480,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 481,  -1,  -1, 482,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 483, 484, 485,  -1,  -1, 486,
      487,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 488,  -1, 489,  -1,
       -1, 490,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 491,
      492,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 493, 494,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 495,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 496, 497,  -1,  -1,  -1,  -1,
       -1,  -1, 498,  -1, 499,  -1,  -1,  -1, 500, 501,
      502,  -1, 503,  -1, 504, 505,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 506,  -1,
       -1, 507,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 508,
      509,  -1,  -1, 510,  -1,  -1, 511,  -1,  -1,  -1,
      512,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 513,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 514,  -1,  -1,  -1, 515,
       -1,  -1,  -1,  -1, 516,  -1,  -1,  -1,  -1,  -1,
       -1, 517,  -1, 518,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 519,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 520,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 521,  -1,  -1, 522,
       -1, 523,  -1, 524,  -1,  -1, 525,  -1,  -1,  -1,
       -1,  -1, 526,  -1,  -1,  -1,  -1,  -1, 527, 528,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      529,  -1, 530, 531,  -1, 532,  -1,  -1, 533,  -1,
      534,  -1,  -1, 535, 536,  -1, 537,  -1,  -1, 538,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 539,  -1,  -1,  -1,  -1,
       -1, 540,  -1,  -1,  -1,  -1,  -1, 541,  -1, 542,
       -1,  -1,  -1,  -1,  -1, 543,  -1,  -1,  -1,  -1,
       -1, 544,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 545,  -1,  -1,  -1,
       -1,  -1, 546, 547,  -1,  -1,  -1, 548,  -1,  -1,
       -1,  -1,  -1, 549,  -1,  -1, 550,  -1,  -1,  -1,
       -1,  -1,  -1, 551,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 552,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 553,  -1, 554, 555, 556,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 557,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 558,  -1, 559, 560,  -1,
       -1, 561,  -1,  -1, 562,  -1,  -1,  -1, 563, 564,
       -1,  -1,  -1, 565, 566,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 567,  -1,  -1, 568,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 569,  -1,  -1, 570,  -1,  -1,  -1,
       -1,  -1, 571,  -1,  -1,  -1, 572,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      573, 574,  -1,  -1,  -1, 575, 576,  -1,  -1,  -1,
       -1,  -1, 577,  -1,  -1,  -1,  -1,  -1,  -1, 578,
       -1,  -1, 579,  -1,  -1, 580,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 581,  -1,
      582,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 583,  -1,  -1,  -1,  -1, 584,  -1, 585,  -1,
       -1,  -1,  -1, 586,  -1,  -1,  -1,  -1,  -1,  -1,
      587,  -1, 588,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 589,  -1,  -1,  -1,  -1,  -1, 590,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 591,  -1,  -1,
       -1,  -1,  -1, 592,  -1,  -1, 593,  -1,  -1,  -1,
      594, 595,  -1,  -1,  -1, 596,  -1,  -1,  -1,  -1,
       -1, 597,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 598,
       -1,  -1,  -1,  -1, 599,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 600,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 601,  -1,  -1, 602,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      603, 604,  -1,  -1,  -1,  -1,  -1,  -1, 605,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 606,  -1, 607, 608,  -1,
       -1,  -1,  -1,  -1,  -1, 609,  -1,  -1, 610,  -1,
      611,  -1, 612,  -1,  -1, 613,  -1, 614,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 615,
       -1,  -1,  -1, 616, 617,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 618,  -1,  -1,  -1,  -1,  -1,
       -1, 619,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 620,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 621,  -1,
       -1,  -1,  -1, 622,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 623,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 624,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 625,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 626, 627,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 628,  -1, 629,  -1, 630,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 631,  -1,  -1,  -1,  -1, 632,  -1,
       -1,  -1, 633, 634,  -1, 635,  -1,  -1, 636,  -1,
      637,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 638,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 639,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 640,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 641,  -1,  -1,  -1,
      642,  -1, 643,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 644,  -1,
       -1,  -1,  -1,  -1,  -1, 645,  -1,  -1,  -1,  -1,
      646,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 647,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 648,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 649,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 650,  -1,
      651, 652,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 653,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 654,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 655,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 656,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 657,
       -1,  -1, 658,  -1,  -1,  -1, 659,  -1,  -1,  -1,
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
       -1, 660,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 661,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      662,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 663,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 664,  -1, 665,  -1,  -1,  -1,  -1,  -1,  -1,
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
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 666,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 667,  -1,  -1,  -1,
       -1, 668,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 669,  -1,  -1,  -1,  -1,  -1, 670,  -1,
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
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 671,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 672,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 673,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
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
       -1, 674,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
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
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 675,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 676,  -1,  -1,  -1,  -1,  -1,
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
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 677
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = attr_hash_func (str, len);

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
#line 686 "attr_lookup"

