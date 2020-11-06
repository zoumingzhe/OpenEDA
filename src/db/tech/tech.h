/* @file  tech.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TECH_TECH_H_
#define EDI_DB_TECH_TECH_H_

#include <vector>

#include "db/core/object.h"
#include "db/tech/layer.h"
#include "db/tech/max_via_stack.h"
#include "db/tech/ndr_rule.h"
#include "db/tech/site.h"
#include "db/tech/units.h"
#include "db/tech/via_rule.h"
#include "db/util/array.h"
#include "util/util.h"

namespace open_edi {
namespace db {

class Tech : public Object {
  public:
    Tech();
    ~Tech();

    bool getHasClearanceMeasure() const;
    void setHasClearanceMeasure(bool flag);
    bool getClearanceMeasureUseMaxxy() const;
    void setClearanceMeasureUseMaxxy(bool flag);
    bool getHasOBSUseMinSpacing() const;
    void setHasOBSUseMinSpacing(bool flag);
    bool getOBSUseMinSpacing() const;
    void setOBSUseMinSpacing(bool flag);
    bool getHasPinUseMinSpacing() const;
    void setHasPinUseMinSpacing(bool flag);
    bool getPinUseMinSpacing() const;
    void setPinUseMinSpacing(bool flag);
    bool getHasFixedMask() const;
    void setHasFixedMask(bool flag);
    bool getCaseSensitive() const;
    void setCaseSensitive(bool flag);

    SymbolIndex getBusBitsId() const;
    SymbolIndex getDividerId() const;
    SymbolIndex getExtensionsId() const;
    const char *getBusBitsName() const;
    void setBusBitsName(const char *s);
    const char *getDividerName() const;
    void setDividerName(const char *s);
    const char *getExtensionsName() const;
    void setExtensionsName(const char *s);

    UInt32 getManuGrids() const;
    void setManuGrids(UInt32 manu_grids);
    double getVersion() const;
    void setVersion(double v);
    Units *getUnits() const;
    void setUnits(Units *const vobj);
    bool addLayer(Layer *layer);
    Int32 getLayerLEFIndexByName(const char *layer_name);
    Layer *getLayer(Int32 layer_id);
    Layer *getLayerByName(const char *layer_name);
    UInt32 getNumLayers() const;
    ObjectId createPropertyDefinitionVector(PropType type);
    void addPropertyDefinition(ObjectId pobj_id);
    ObjectId getPropertyDefinitionVectorId(PropType type);
    ObjectId getPropertyDefinitionId(PropType type, const char *prop_name);
    MaxViaStack *getMaxViaStack() const;
    void setMaxViaStack(MaxViaStack *mvs);
    ViaRule *getViaRule() const;
    ViaRule *getViaRule(const char *name) const;
    ObjectId getNonDefaultRuleVectorId() const;
    ObjectId getViaMasterVectorId() const;
    ObjectId getViaRuleVectorId() const;
    ObjectId getSiteVectorId() const;
    ObjectId getNonDefaultRuleIdByName(const char *ndr_rule_name) const;
    NonDefaultRule *getNonDefaultRule(const char *ndr_rule_name) const;
    void addNonDefaultRule(ObjectId ndr_rule_id);

    ArrayObject<ObjectId> *getNonDefaultRuleArray() const;
    ArrayObject<ObjectId> *getViaMasterArray() const;
    ArrayObject<ObjectId> *getViaRuleArray() const;
    ArrayObject<ObjectId> *getSiteArray() const;

    ViaMaster *createAndAddViaMaster(std::string &name);
    ViaRule *createViaRule(std::string &name);

    ViaMaster *getViaMaster(const std::string &name) const;
    ViaRule *getViaRule(const std::string &name) const;

    void addViaMaster(ViaMaster *via_master);
    void addViaRule(ViaRule *via_rule);

    void addSite(Site *site);
    Site *getSiteByName(const char *site_name) const;

    // transfer between user unit and db unit
    Int32 micronsToDBU(double microns);
    double areaMicronsToDBU(double microns);
    double dbuToMicrons(Int32 dbu);
    double areaDBUToMicrons(Long dbu);

  private:
    // first 32 bits
    Bits has_clearance_measure_ : 1;
    Bits clearance_measure_use_maxxy_ : 1;
    Bits has_obs_use_min_spacing_ : 1;
    Bits obs_use_min_spacing_ : 1;
    Bits has_pin_use_min_spacing_ : 1;
    Bits pin_use_min_spacing_ : 1;
    Bits has_fixed_mask_ : 1;
    Bits case_sensitive_ : 1;
    Bits unused_ : 24;

    SymbolIndex bus_bits_index_;
    SymbolIndex divider_name_index_;
    SymbolIndex extensions_name_index_;

    double version_;
    UInt32 manu_grids_;
    ObjectId db_units_;
    ObjectId layer_ids_;
    ObjectId sites_;
    ObjectId property_definitions_array_[static_cast<int>(PropType::kUnknown)];
    MaxViaStack *max_via_stack_;
    ObjectId via_rules_;
    ObjectId ndr_rules_;
    ObjectId via_masters_;
    ObjectId cell_;  // top cell
};

}  // namespace db
}  // namespace open_edi

#endif /* ----- #ifndef EDI_DB_TECH_TECH_H_  ----- */
