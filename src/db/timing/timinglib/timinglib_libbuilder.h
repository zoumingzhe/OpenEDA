/**
 * @file timinglib_libbuilder.h
 * @date 2020-08-26
 * @brief build the data structure
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBBUILDER_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBBUILDER_H_

#include <functional>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/timing/timinglib/timinglib_structs.h"

namespace open_edi {
namespace db {
class LibSet;
class Object;
class TCell;
class TTerm;
class TPgTerm;
class TTermAttr;
class TimingArc;
class TableAxis;
}  // namespace db
}  // namespace open_edi

template <typename T>
using str_omap_t = std::unordered_map<std::string, T>;

namespace Timinglib {

class LibSyn;

#define tb_namespace open_edi::db
#define buildParam const std::string &name, timinglib_attribute_value *v

typedef struct ObjectList {
    tb_namespace::Object *object = nullptr;
    struct ObjectList *owner = nullptr;
    struct ObjectList *next = nullptr;
} ObjectList;

typedef struct TypeGroup {
    int bit_width = 0;
    int bit_from = -1;
    int bit_to = -1;
    bool downto = false;
} TypeGroup;

class LibBuilder {
  public:
    explicit LibBuilder(tb_namespace::LibSet *libset);
    ~LibBuilder();

    // set
    void setLibSyn(LibSyn *s);

    void beginGroup(timinglib_head *);
    void endGroup();

    void buildAttribute(buildParam);

  private:
    void __buildGroupMap();
    void __buildLibrary(timinglib_head *);
    void __buildCell(timinglib_head *);
    void __buildPin(timinglib_head *);
    void __buildPgPin(timinglib_head *);
    void __buildOperatingConditions(timinglib_head *);
    void __buildWireLoad(timinglib_head *);
    void __buildWireLoadTable(timinglib_head *);
    void __buildWireLoadSelection(timinglib_head *);
    void __buildLuTableTemplate(timinglib_head *);
    void __buildFf(timinglib_head *);
    void __buildLatch(timinglib_head *);
    void __buildStatetable(timinglib_head *);
    void __buildBus(timinglib_head *);
    void __buildBundle(timinglib_head *);
    void __buildType(timinglib_head *);
    void __buildTiming(timinglib_head *);
    void __buildCellRise(timinglib_head *);
    void __buildCellFall(timinglib_head *);
    void __buildRiseTransition(timinglib_head *);
    void __buildFallTransition(timinglib_head *);
    void __buildRiseConstraint(timinglib_head *);
    void __buildFallConstraint(timinglib_head *);

    void __buildAttrMap();
    void __buildScalingFactorsAttrs();

    // library
    void __buildDefaultOc(buildParam);
    void __buildTimeUnit(buildParam);
    void __buildPullingResistanceUnit(buildParam);
    void __buildResistanceUnit(buildParam);
    void __buildCapacitiveLoadUnit(buildParam);
    void __buildVoltageUnit(buildParam);
    void __buildCurrentUnit(buildParam);
    void __buildLeakagePowerUnit(buildParam);
    void __buildDistanceUnit(buildParam);
    void __buildNomTemperature(buildParam);
    void __buildNomVoltage(buildParam);
    void __buildNomProcess(buildParam);
    void __buildInputThresholdPctFall(buildParam);
    void __buildInputThresholdPctRise(buildParam);
    void __buildOutputThresholdPctFall(buildParam);
    void __buildOutputThresholdPctRise(buildParam);
    void __buildSlewLowerThresholdPctFall(buildParam);
    void __buildSlewLowerThresholdPctRise(buildParam);
    void __buildSlewUpperThresholdPctFall(buildParam);
    void __buildSlewUpperThresholdPctRise(buildParam);
    void __buildSlewDerateFromLibrary(buildParam);
    void __buildScalingFactorsAttr(buildParam);
    void __buildDefaultWireLoad(buildParam);
    void __buildDefaultWireLoadArea(buildParam);
    void __buildDefaultWireLoadMode(buildParam);
    void __buildDefaultWireLoadCapacitance(buildParam);
    void __buildDefaultWireLoadResistance(buildParam);
    void __buildDefaultWireLoadSelection(buildParam);
    void __buildDefaultInputPinCap(buildParam);
    void __buildDefaultOutputPinCap(buildParam);
    void __buildDefaultInoutPinCap(buildParam);
    void __buildDefaultMaxCapacitance(buildParam);
    void __buildDefaultMaxFanout(buildParam);
    void __buildDefaultMaxTransition(buildParam);
    void __buildDefaultFanoutLoad(buildParam);
    void __buildDefaultCellLeakagePower(buildParam);
    void __buildVoltageMap(buildParam);

    // operating conditions
    void __buildProcess(buildParam);
    void __buildTemperature(buildParam);
    void __buildVoltage(buildParam);
    void __buildTreeType(buildParam);

    // cell
    void __buildArea(buildParam);
    void __buildCellFootprint(buildParam);
    void __buildDontTouch(buildParam);
    void __buildDontUse(buildParam);
    void __buildAlwaysOn(buildParam);
    void __buildIsMacroCell(buildParam);
    void __buildIsPad(buildParam);
    void __buildIsDecapCell(buildParam);
    void __buildIsFillerCell(buildParam);
    void __buildIsTapCell(buildParam);
    void __buildIsClockGatingCell(buildParam);
    void __buildIsClockIsolationCell(buildParam);
    void __buildIsIsolationCell(buildParam);
    void __buildIsNoEnable(buildParam);
    void __buildClockGatingIntegratedCell(buildParam);
    void __buildAntennaDiodeType(buildParam);
    void __buildIsLevelShifter(buildParam);
    void __buildCellLeakagePower(buildParam);
    void __buildSwitchCellType(buildParam);
    void __buildRetentionCell(buildParam);

    // pin
    void __buildRelatedGroundPin(buildParam);
    void __buildRelatedPowerPin(buildParam);
    void __buildMaxTransition(buildParam);
    void __buildFallCapacitance(buildParam);
    void __buildRiseCapacitance(buildParam);
    void __buildThreeState(buildParam);
    void __buildCapacitance(buildParam);
    void __buildMaxCapacitance(buildParam);
    void __buildMinCapacitance(buildParam);
    void __buildDirection(buildParam);
    void __buildMaxFanout(buildParam);
    void __buildMinFanout(buildParam);
    void __buildMinTransition(buildParam);
    void __buildMinPeriod(buildParam);
    void __buildMinPulseWidthHigh(buildParam);
    void __buildMinPulseWidthLow(buildParam);
    void __buildClock(buildParam);
    void __buildClockGateClockPin(buildParam);
    void __buildClockGateEnablePin(buildParam);
    void __buildFunction(buildParam);
    void __buildMembers(buildParam);
    void __buildBusType(buildParam);

    // pg_pin
    void __buildPgType(buildParam);
    void __buildVoltageName(buildParam);

    // type
    void __buildBitWidth(buildParam);
    void __buildBitFrom(buildParam);
    void __buildBitTo(buildParam);
    void __buildDownto(buildParam);

    // timing
    void __buildTimingSense(buildParam);
    void __buildTimingType(buildParam);
    void __buildWhen(buildParam);
    void __buildRelatedPin(buildParam);

    // wire_load
    void __buildResistance(buildParam);
    void __buildSlope(buildParam);
    void __buildFanoutLength(buildParam);

    // wire_load_table
    void __buildFanoutCapacitance(buildParam);
    void __buildFanoutResistance(buildParam);
    void __buildFanoutArea(buildParam);

    // wire_load_selection
    void __buildWireLoadFromArea(buildParam);

    // lu_table_template
    void __buildVariable1(buildParam);
    void __buildVariable2(buildParam);
    void __buildVariable3(buildParam);
    void __buildIndex1(buildParam);
    void __buildIndex2(buildParam);
    void __buildIndex3(buildParam);

    // cell_rise cell_fall rise_transition
    // fall_transition rise_constraint fall_constraint
    void __buildValues(buildParam);

    void __getTermsFromTopStack(std::vector<tb_namespace::TTerm *> *libterms);
    void __getPgTermsFromTopStack(
        std::vector<tb_namespace::TPgTerm *> *libpgterms);
    void __getTimingarcsFromTopStack(
        std::vector<tb_namespace::TimingArc *> *libtimingarcs);
    void __getObjectsFromTopStack(std::vector<tb_namespace::Object *> *objects);
    void __deleteObjectList(ObjectList *objects);
    float __getTimeUnitMultiply(const std::string &u);
    float __getResistanceUnitMultiply(const std::string &u);
    float __getCapacitiveUnitMultiply(const std::string &u);
    float __getVoltageUnitMultiply(const std::string &u);
    float __getCurrentUnitMultiply(const std::string &u);
    float __getPowerUnitMultiply(const std::string &u);
    float __getDistanceUnitMultiply(const std::string &u);
    void __getPinNamesFromBusRange(const char *str,
                                   std::vector<std::string> *pinNames);
    tb_namespace::TCell *__getTcellByTimingarc(tb_namespace::TimingArc *t);

    bool __isIntType(timinglib_attribute_value *);
    bool __isDoubleType(timinglib_attribute_value *);
    bool __isStringType(timinglib_attribute_value *);
    bool __isBoolType(timinglib_attribute_value *);
    float __getDoubleValue(timinglib_attribute_value *);
    void __getStringFloatList(char *str, std::vector<double> *values);
    void __splitStringByDelim(char *str, const char *delim,
                              std::vector<std::string> *rets);

    str_omap_t<std::function<void(timinglib_head *)>> group_builder_map_;
    str_omap_t<
        std::function<void(const std::string &, timinglib_attribute_value *)>>
        attr_builder_map_;

    tb_namespace::LibSet *libset_;
    std::stack<ObjectList *> objects_stack_;
    std::stack<std::string> groupname_stack_;
    bool push_status_;

    str_omap_t<
        std::tuple<tb_namespace::ScaleFactorType, tb_namespace::ScaleFactorPvt,
                   tb_namespace::ScaleFactorRiseFall>>
        scaling_factors_str_map_;

    LibSyn *libsyn_;

    bool setResistanceUnit_;
    str_omap_t<TypeGroup> type_group_lib_map_;
    str_omap_t<TypeGroup> type_group_cell_map_;
    str_omap_t<std::vector<tb_namespace::TTerm *>>
        bus_or_bundle_member_pins_map_;
    std::string cur_type_group_name_;
    std::string cur_bus_or_bundle_name_;

    tb_namespace::TableAxis *axis_[3];
};
}  // namespace Timinglib

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBBUILDER_H_
