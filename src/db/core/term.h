/* @file  term.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef SRC_DB_CORE_TERM_H_
#define SRC_DB_CORE_TERM_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/util/geometrys.h"

namespace open_edi {
namespace db {

class AntennaArea : public Object {
  public:
    AntennaArea() { layer_name_index_ = -1; }
    AntennaArea(double a) : area_(a), layer_name_index_(-1) {}
    //  AntennaArea(int a, const char * layer):area_(a),layer_name_(layer){}
    ~AntennaArea() {}
    void setArea(double a) { area_ = a; }
    double getArea() const { return area_; }
    void setLayerName(const char* v);
    std::string& getLayerName() const;
    SymbolIndex getLayerNameID() const { return layer_name_index_; }

  private:
    double area_;
    SymbolIndex layer_name_index_;
};

class AntennaModelTerm : public Object {
  public:
    AntennaModelTerm();
    ~AntennaModelTerm() {}

    void addAntennaGateArea(ObjectId aa);
    AntennaArea* getAntennaGateArea(int index) const;
    int getAntennaGateAreaNum() const;
    void addAntennaMaxAreaCar(ObjectId aa);
    AntennaArea* getAntennaMaxAreaCar(int index) const;
    int getAntennaMaxAreaCarNum() const;
    void addAntennaMaxSideAreaCar(ObjectId aa);
    AntennaArea* getAntennaMaxSideAreaCar(int index) const;
    int getAntennaMaxSideAreaCarNum() const;
    void addAntennaMaxCutCar(ObjectId aa);
    AntennaArea* getAntennaMaxCutCar(int index) const;
    int getAntennaMaxCutCarNum() const;
    void setValid(bool v) { valid_ = v; }
    bool getValid() const { return valid_; }
    void print() const;
    void printLEF(std::ofstream& ofs) const;

  private:
    bool valid_;
    ObjectId antenna_gate_areas_;
    ObjectId antenna_max_area_cars_;
    ObjectId antenna_max_side_area_cars_;
    ObjectId antenna_max_cut_cars_;
};

class Port : public Object {
  public:
    Port();
    ~Port();

    void setClass(const char* v);
    std::string& getClass() const;
    void addLayerGeometry(ObjectId v);
    LayerGeometry* getLayerGeometry(int index) const;
    int getLayerGeometryNum() const;
    bool getIsReal() const { return is_real_; }
    void setIsReal(bool flag) { is_real_ = flag; }
    bool getHasPlacement() const;
    void setHasPlacement(bool flag);
    PlaceStatus getStatus() const;
    void setStatus(PlaceStatus s);
    Point getLocation() const;
    void setLocation(Point& p);
    Orient getOrient() const;
    void setOrient(Orient o);

  private:
    // lef information
    SymbolIndex class_index_;
    ObjectId layer_geometries_;
    // def information
    PlaceStatus status_;
    Point location_;
    bool is_real_       :1;
    bool has_placement_ :1;
    Orient orient_      :8;
    int reserved_       :22;
};

/// Yibo: I need this for InstTerm, so this is a temporary implementation.
/// I do not think Term needs to be an AttrObject, as it is frequently accessed.
/// We can directly put all the data members in its data field.

/// @brief Terminal of a cell
class Term : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    Term();

    /// @brief constructor
    Term(Object* owner, IndexType id);

    /// @brief copy constructor
    Term(Term const& rhs);

    /// @brief move constructor
    Term(Term&& rhs) noexcept;

    /// @brief copy assignment
    Term& operator=(Term const& rhs);

    /// @brief move assignment
    Term& operator=(Term&& rhs) noexcept;

    void setName(std::string name);
    std::string& getName() const;

    SymbolIndex getNameIndex() { return name_index_; }

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    void setHasRange(bool has_range) { has_range_ = has_range; }
    bool getHasRange() { return has_range_; }

    void setRangeLow(int32_t range_low) { range_low_ = range_low; }
    int32_t getRangeLow() { return range_low_; }

    void setRangeHigh(int32_t range_high) { range_high_ = range_high; }
    int32_t getRangeHigh() { return range_high_; }

    std::string const& getTaperRule() const;
    void setTaperRule(const char* v);

    //LEF: PIN DIRECTION
    bool hasDirection() const;
    void setDirection(const char* v);
    void setDirection(SignalDirection v);

    //LEF: PIN USE
    bool hasUse() const;
    void setUse(const char* v);

    std::string getDirectionStr() const;
    void setDirectionStr(const char* v);
    
    bool isPGType() const;
    SignalType getPinType() const;
    std::string getPinTypeStr() const;
    void setPinTypeStr(const char* v);
    void setPinType(SignalType v);

    SymbolIndex getNetExprIndex() const;
    std::string const& getNetExpr() const;
    void setNetExpr(const char* v);
    SymbolIndex getSupplySensitivityIndex() const;
    std::string const& getSupplySensitivity() const;
    void setSupplySensitivity(const char* v);
    SymbolIndex getGroundSensitivityIndex() const;
    std::string const& getGroundSensitivity() const;
    void setGroundSensitivity(const char* v);
    std::string const& getMustjoin() const;
    void setMustjoin(const char* v);
    std::string const& getShape() const;
    void setShape(const char* v);
    void addAntennaPartialMetalArea(ObjectId aa);
    AntennaArea* getAntennaPartialMetalArea(int index) const;
    int getAntennaPartialMetalAreaNum() const;
    void addAntennaPartialMetalSideArea(ObjectId aa);
    AntennaArea* getAntennaPartialMetalSideArea(int index) const;
    int getAntennaPartialMetalSideAreaNum() const;
    void addAntennaPartialCutArea(ObjectId aa);
    AntennaArea* getAntennaPartialCutArea(int index) const;
    int getAntennaPartialCutAreaNum() const;
    void addAntennaDiffArea(ObjectId aa);
    AntennaArea* getAntennaDiffArea(int index) const;
    int getAntennaDiffAreaNum() const;
    void addAntennaModelTerm(int index, AntennaModelTerm* am);
    const AntennaModelTerm* GetAntennaModelTerm(int index) const {
        return &antenna_models_[index];
    }
    void addPort(ObjectId p);
    Port* getPort(int index) const;
    int getPortNum() const;
    void print() const;
    void printLEF(std::ofstream& ofs) const;

  protected:
    /// @brief copy object
    void copy(Term const& rhs);
    /// @brief move object
    void move(Term&& rhs);
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, Term const& rhs);
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, Term& rhs);

  private:
    SymbolIndex name_index_;
    
    SignalDirection direction_ :8;
    SignalType type_           :8;
    bool has_range_            :1;
    int32_t reserved_          :15;
    int32_t range_low_;
    int32_t range_high_;

    //  SymbolIndex name_index_; ///< terminal name
    SymbolIndex taper_rule_index_;
    SymbolIndex net_expr_index_;
    SymbolIndex supply_sensitivity_index_;
    SymbolIndex ground_sensitivity_index_;
    SymbolIndex mustjoin_index_;
    SymbolIndex shape_index_;
    ObjectId antenna_partial_metal_areas_;
    ObjectId antenna_partial_metal_side_areas_;
    ObjectId antenna_partial_cut_areas_;
    ObjectId antenna_diff_areas_;
    AntennaModelTerm antenna_models_[6];
    ObjectId ports_;
};

}  // namespace db
}  // namespace open_edi

#endif  //  SRC_DB_CORE_TERM_H_
