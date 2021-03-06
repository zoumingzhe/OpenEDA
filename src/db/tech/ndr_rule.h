/**
 * @file  NonDefaultRule.h
 * @date  08/17/2020 10:18:46 AM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TECH_NDR_RULE_H_
#define EDI_DB_TECH_NDR_RULE_H_

#include "db/core/object.h"
#include "db/tech/property.h"
#include "db/tech/type_def.h"
#include "db/util/symbol_table.h"
#include "util/util.h"

namespace open_edi {
namespace db {

/**
 * @class NonDefaultRule
 * @brief
 * the implementation class of NONDEFAULTRULE
 * [NONDEFAULTRULE ruleName
 *      [HARDSPACING ;]
 *      {LAYER layerName
 *          WIDTH width ;
 *          [DIAGWIDTH diagWidth ;]
 *          [SPACING minSpacing ;]
 *          [WIREEXTENSION value ;]
 *      END layerName} ...
 *      [VIA viaStatement] ...
 *      [USEVIA viaName ;] ...
 *      [USEVIARULE viaRuleName ;] ...
 *      [MINCUTS cutLayerName numCuts ;] ...
 *      [PROPERTY propName propValue ;] ...
 *      [PROPERTY LEF58_USEVIACUTCLASS
 *      "USEVIACUTCLASS cutLayerName className
 *          [ROWCOL numCutRows numCutCols]
 *          ;... “ ;]
 *  END ruleName]
 *
 */
class NonDefaultRuleLayer;
class NonDefaultRuleMinCuts;
#if 0
class NonDefaultRuleUseViaCutClass;
#endif

// Class NonDefaultRuleLayer:
class NonDefaultRuleLayer : public Object {
  public:
    /// @brief default constructor
    NonDefaultRuleLayer();

    /// @brief copy constructor
    NonDefaultRuleLayer(NonDefaultRuleLayer const &rhs);

    /// @brief move constructor
    NonDefaultRuleLayer(NonDefaultRuleLayer &&rhs) noexcept;

    /// @brief copy assignment
    NonDefaultRuleLayer &operator=(NonDefaultRuleLayer const &rhs);

    /// @brief move constructor
    NonDefaultRuleLayer &operator=(NonDefaultRuleLayer &&rhs) noexcept;
    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    NonDefaultRuleLayer *clone() const;

    /// @brief summarize memory usage of the object in bytes
    UInt32 memory() const;

    // ACCESS
    // Get:
    const char *getName() const;
    SymbolIndex getNameIndex() const;
    UInt32 getWidth() const;
    UInt32 getDiagWidth() const;
    UInt32 getSpacing() const;
    UInt32 getWireExt() const;
    float getResistancePerSquare() const;
    float getCapacitancePerSquare() const;
    float getEdgeCapacitance() const;

    bool hasWidth() const;
    bool hasDiagWidth() const;
    bool hasSpacing() const;
    bool hasWireExt() const;
    bool hasResistancePerSquare() const;
    bool hasCapacitancePerSquare() const;
    bool hasEdgeCapacitance() const;

    // Set:
    void setLayerIndex(ObjectIndex v);
    void setWidth(UInt32 v);
    void setDiagWidth(UInt32 v);
    void setSpacing(UInt32 v);
    void setWireExt(UInt32 v);
    void setResistancePerSquare(float v);
    void setCapacitancePerSquare(float v);
    void setEdgeCapacitance(float v);
    void setName(const char *v);
    // Print:
    void printLEF(std::ofstream &ofs) const;
    void printDEF(FILE *fp) const;

  protected:
    /// @brief copy object
    void copy(NonDefaultRuleLayer const &rhs);
    /// @brief move object
    void move(NonDefaultRuleLayer &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os,
                                   NonDefaultRuleLayer const &rhs);
    /// @brief overload input stream
    // friend IStreamBase &operator>>(IStreamBase &is, NonDefaultRuleLayer &rhs)
    // {}

  private:
    // DATA
    SymbolIndex name_index_;
    UInt32 width_;
    UInt32 diag_width_;
    UInt32 spacing_;
    UInt32 wire_ext_;
    float resistance_per_square_;
    float capacitance_per_square_;
    float edge_capacitance_;
    Bits has_width_ : 1;
    Bits has_diag_width_ : 1;
    Bits has_spacing_ : 1;
    Bits has_wire_ext_ : 1;
    Bits has_resistance_per_square_ : 1;
    Bits has_capacitance_per_square_ : 1;
    Bits has_edge_capacitance_ : 1;    
};

// Class NonDefaultRuleMinCuts
class NonDefaultRuleMinCuts : public Object {
  public:
    /// @brief default constructor
    NonDefaultRuleMinCuts() : name_index_(kInvalidSymbolIndex), num_cuts_(0) {}

    /// @brief copy constructor
    NonDefaultRuleMinCuts(NonDefaultRuleMinCuts const &rhs);

    /// @brief move constructor
    NonDefaultRuleMinCuts(NonDefaultRuleMinCuts &&rhs) noexcept;

    /// @brief copy assignment
    NonDefaultRuleMinCuts &operator=(NonDefaultRuleMinCuts const &rhs);

    /// @brief move constructor
    NonDefaultRuleMinCuts &operator=(NonDefaultRuleMinCuts &&rhs) noexcept;
    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    NonDefaultRuleMinCuts *clone() const;

    /// @brief summarize memory usage of the object in bytes
    UInt32 memory() const;

    void printLEF(std::ofstream &ofs) const;
    void printDEF(FILE *fp) const;

    // Get:
    const char *getName() const;
    SymbolIndex getNameIndex() const;
    uint32_t getNumCuts() const;

    // Set:
    void setName(const char *v);
    void setNumCuts(uint32_t v);

  protected:
    /// @brief copy object
    void copy(NonDefaultRuleMinCuts const &rhs);
    /// @brief move object
    void move(NonDefaultRuleMinCuts &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os,
                                   NonDefaultRuleMinCuts const &rhs);
    /// @brief overload input stream
    // friend IStreamBase &operator>>(IStreamBase &is, NonDefaultRuleMinCuts
    // &rhs);

  private:
    // DATA
    SymbolIndex name_index_;
    uint32_t num_cuts_;
};

class NonDefaultRule : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    NonDefaultRule();

    /// @brief constructor
    NonDefaultRule(Object *owner, UInt32 id);

    /// @brief copy constructor
    NonDefaultRule(NonDefaultRule const &rhs);
    /// @brief move constructor
    NonDefaultRule(NonDefaultRule &&rhs) noexcept;

    /// @brief destructor
    ~NonDefaultRule();

    /// @brief copy assignment
    NonDefaultRule &operator=(NonDefaultRule const &rhs);

    /// @brief move constructor
    NonDefaultRule &operator=(NonDefaultRule &&rhs) noexcept;
    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    NonDefaultRule *clone() const {return nullptr;}

    /// @brief summarize memory usage of the object in bytes
    UInt32 memory() const;

    // Get:
    const char *getName() const;
    SymbolIndex getNameIndex() const;
    bool getHardSpacing() const;
    bool getFromDEF() const;
    uint64_t numLayers() const;
    uint64_t numMinCuts() const;
    uint64_t numVias() const;
    uint64_t numUseVias() const;
    uint64_t numUseViaRules() const;
    uint64_t numProperties() const;
    // UInt32 numLEF58UseViaCutClass() const ;
    ObjectId getLayersId() const;
    ObjectId getMinCutsId() const;
    ObjectId getViasId() const;
    ObjectId getUseViasId() const;
    ObjectId getUseViaRulesId() const;
    ObjectId getPropertiesId() const;

    // Set:
    void setName(const char *v);
    void setLayerSize(uint64_t v);
    void setMinCutsSize(uint64_t v);
    void setViaSize(uint64_t v);
    void setUseViaSize(uint64_t v);
    void setUseViaRuleSize(uint64_t v);
    void setPropertySize(uint64_t v);
    void addLayer(ObjectId obj_id);
    void addMinCuts(ObjectId obj_id);
    void addVia(ObjectId obj_id);
    void addUseVia(ObjectId obj_id);
    void addUseViaRule(ObjectId obj_id);
    void addProperty(ObjectId obj_id);
    void setHardSpacing(bool v);
    void setFromDEF(bool v);
    // print:
    void printLEF(std::ofstream &ofs) const;
    void printDEF(FILE *fp) const;

  protected:
    /// @brief copy object
    void copy(NonDefaultRule const &rhs);
    /// @brief move object
    void move(NonDefaultRule &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, NonDefaultRule const &rhs);
    /// @brief overload input stream
    // friend IStreamBase &operator>>(IStreamBase &is, NonDefaultRule &rhs) {};

  private:
    void __init();
    ObjectId __createObjectArray(int64_t size);
    void __deleteObjectArray(ObjectId array_id);

    // DATA
    SymbolIndex name_index_; /**< name */
    ObjectId layers_;
    ObjectId min_cuts_;
    ObjectId properties_;
    ObjectId vias_;
    ObjectId use_vias_;
    ObjectId use_via_rules_;
    Bits hard_spacing_ : 1;
    Bits from_def_ : 1;    
};

}  // namespace db
}  // namespace open_edi

#endif
