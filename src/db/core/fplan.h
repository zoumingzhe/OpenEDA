/**
 * @file  fplan.hpp
 * @date  08/12/2020 05:13:59 PM CST
 * @brief "Floorplan, Row, Site, Grid & Constraint"
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_FPLAN_HPP
#define EDI_DB_FPLAN_HPP

#include "db/core/object.h"
#include "db/core/inst.h"
#include "db/tech/site.h"
#include "db/util/box.h"
#include "util/enums.h"
#include "util/point.h"
#include "util/util.h"

namespace open_edi {
namespace db {

class Cell;

using CoordinateType = CoordinateTraits<int32_t>::CoordinateType;

class Floorplan;

/// @brief Row class
class Row : public Object {
  public:
    using BaseType = Object;

    Row();
    Row(Row &r);
    Row(Row &&r);
    ~Row();

    Box getBox() const;
    void setBox(Box &b);

    Orient getLegalOrient() const;
    void setLegalOrient(Orient &o);
    Orient getOrient() const;
    void setOrient(Orient &o);

    Site *getSite() const;
    ObjectId getSiteId() const;
    void setSiteId(ObjectId &s);

    int32_t getSiteCount() const;
    void setSiteCount(int32_t &sc);

    void copy(Row const &r);
    void move(Row &&rhs);

    void setFloorplan(ObjectId fp);
    Floorplan *getFloorplan();

    bool setName(const char *name);
    bool setName(std::string &name);
    void setName(SymbolIndex &name);
    std::string &getName();
    SymbolIndex getNameIndex();

    void setSiteName(const char *site_name);
    std::string &getSiteName();

    void setOrigX(int64_t orig_x) { orig_x_ = orig_x; };
    int64_t getOrigX() const { return orig_x_; };

    void setOrigY(int64_t orig_y) { orig_y_ = orig_y; };
    int64_t getOrigY() const { return orig_y_; };

    void setSiteOrient(Orient site_orient) { site_orient_ = site_orient; };
    Orient getSiteOrient() const { return site_orient_; };

    void setHasDo(bool has_do) { has_do_ = has_do; };
    bool getHasDo() const { return has_do_; };

    void setNumX(Bits64 num_x) { num_x_ = num_x; };
    Bits64 getNumX() const { return num_x_; }

    void setNumY(Bits64 num_y) { num_y_ = num_y; };
    Bits64 getNumY() const { return num_y_; }

    void setHasDoStep(bool has_do_step) { has_do_step_ = has_do_step; };
    bool getHasDoStep() const { return has_do_step_; };

    void setStepX(Bits64 step_x) { step_x_ = step_x; };
    Bits64 getStepX() const { return step_x_; };

    void setStepY(Bits64 step_y) { step_y_ = step_y; };
    Bits64 getStepY() const { return step_y_; };

    void setPropertySize(uint64_t v);
    uint64_t getNumProperties() const;
    void addProperty(ObjectId prop_id);
    ObjectId getPropertiesId() const;

    void print();
    void print(FILE *fp);

  private:
    Box bbox_;
    Orient legal_orient_;
    Orient orient_;
    ObjectId site_id_;
    int32_t site_count_;
    ObjectId floorplan_;

    SymbolIndex row_name_index_;

    SymbolIndex site_name_index_;

    int64_t orig_x_;
    int64_t orig_y_;
    Orient site_orient_;

    bool has_do_;
    Bits64 num_x_;
    Bits64 num_y_;

    bool has_do_step_;
    Bits64 step_x_;
    Bits64 step_y_;

    ObjectId properties_id_;
};  // class Row

/// @brief Track class
class Track : public Object {
  public:
    using BaseType = Object;
    Track();
    Track(Track &t);
    Track(Track &&rhs);
    ~Track();

    void setFloorplan(ObjectId fp);
    Floorplan *getFloorplan();

    void setDirectionX(bool direction_x);
    bool getDirectionX();

    void setStart(int32_t start);
    int32_t getStart();

    void setNumTracks(uint32_t num_tracks);
    uint32_t getNumTracks();

    void setSpace(uint32_t space);
    uint32_t getSpace();

    void setHasMask(bool has_mask);
    bool getHasMask();
    void setMask(uint8_t mask);
    uint8_t getMask();
    void setHasSameMask(bool has_same_mask);
    bool getHasSameMask();

    void addLayer(Int32 &layer_id);
    void addLayer(const char *layer_name);
    void addLayer(std::string &layer_name);
    ObjectId getLayers() const;

    void print();
    void print(FILE *fp);
    void copy(Track const &r);
    // void move(Track &&rhs);

  private:
    ObjectId floorplan_;

    bool direction_x_;  // true: X, false: Y
    int32_t start_;
    uint32_t num_tracks_;
    uint32_t space_;

    bool has_mask_;
    uint8_t mask_;
    bool has_same_mask_;
    ObjectId layers_;
};

class Grid : public Object {
  public:
    using BaseType = Object;
    enum GridType { kGridNone, kGridManufactory, kGridGcell, kGridUser };
    Grid();
    Grid(GridType t);
    Grid(Grid &&rhs);
    ~Grid();

    void setGridType(GridType t);
    GridType getGridType();

    void setFloorplan(ObjectId fp);
    Floorplan *getFloorplan();

    void setDirectionX(bool direction_x);
    bool getDirectionX();

    void setStart(int32_t start);
    int32_t getStart();

    void setNumber(uint32_t number);
    uint32_t getNumber();

    void setSpace(uint32_t space);
    uint32_t getSpace();

    void print();
    void print(FILE *fp);
    void copy(Grid const &r);

  private:
    GridType grid_type_;

    ObjectId floorplan_;

    bool direction_x_;  // true: X, false: Y
    int32_t start_;
    uint32_t number_;  // drections is vertical: numColumns, horizontal: numRows
    uint32_t space_;
};

/// @brief Constraint class
class Constraint : public Object {
  public:
    using BaseType = Object;

    enum ConstraintType {
        kConstraintNone,
        kConstraintPBlkg,
        kConstraintRBlkg,
        kConstraintPinBlkg,
        kConstraintRegion
    };
    enum class ConstraintSubType : int8_t {
        kSubTypeNone,
        kPBlkgBufferOnly,
        kPBlkgMacroOnly,
        kPBlkgHalo,
        kRBlkgZeroSpacing,
        kRBlkgEffWidth,    // effective width
        kRBlkgEffSpacing,  // effective spacing
        kRBlkgBoundary,    // outside design boundary blkg
        kPinBlkgSoft,
        kPinBlkgHard,
        kPinBlkgPartial,
        kRegionFence,
        kRegionGuide
    };

    Constraint();
    Constraint(const char *name, ConstraintType t /*, Shape &s*/);
    Constraint(SymbolIndex &name, ConstraintType t /*, Shape &s*/);
    // Constraint(Shape &s); TODO
    Constraint(ConstraintType &t);
    ~Constraint();

    SymbolIndex getNameIndex();
    std::string &getName();
    bool setName(const char *name);
    bool setName(std::string &name);
    void setName(SymbolIndex &name);
    ConstraintType getConstraintType();
    void setConstraintType(ConstraintType t);
    ConstraintSubType getConstraintSubType();
    void setConstraintSubType(ConstraintSubType st);
    int32_t getUtilization();
    void setUtilization(int32_t &u);
    // Shape getShape(); TODO
    // void setShape(Shape &s);

    void setFloorplan(ObjectId fp);
    Floorplan *getFloorplan();

    void setHasLayer(bool has_layer);
    bool setLayer(const char *name);
    void setPlacement(bool has_placement);
    void setHasComponent(bool has_component);
    bool setComponent(const char *name);
    void setSlots(bool has_slots);
    void setFills(bool has_fills);
    void setPushdown(bool has_pushdown);
    void setExceptpgnet(bool has_exceptpgnet);  // 5.7
    void setSoft(bool has_soft);                // 5.7
    void setPartial(bool has_partial);          // 5.7
    void setDensity(double max_density);        // 5.7
    void setSpacing(bool has_spacing);
    void setMinSpacing(Bits64 min_spacing);
    void setDesignRuleWidth(bool has_design_rule_width);
    void setEffectiveWidth(Bits64 effectiveWidth);
    void setMask(bool has_mask);       // 5.8
    void setMaskNum(Bits64 mask_num);  // 5.8

    bool hasLayer() const;
    Int32 layerId() const;
    std::string layerName() const;
    bool hasPlacement() const;
    bool hasComponent() const;
    ObjectId componentId() const;
    Inst *component() const;
    std::string componentName() const;
    bool hasSlots() const;
    bool hasFills() const;
    bool hasPushdown() const;
    bool hasExceptpgnet() const;         // 5.7
    bool hasSoft() const;                // 5.7
    bool hasPartial() const;             // 5.7
    double placementMaxDensity() const;  // 5.7
    bool hasSpacing() const;             // 5.6
    Bits64 minSpacing() const;           // 5.6
    bool hasDesignRuleWidth() const;     // 5.6
    Bits64 effectiveWidth() const;       // 5.6
    bool hasMask() const;                // 5.8
    Bits64 maskNum() const;              // 5.8

    Box *createBox(int64_t xl, int64_t yl, int64_t xh, int64_t yh);
    ObjectId getBoxesId() const;
    void addPolygon(Polygon *polygon);
    ObjectId getPolygonsId() const;

    void printBlockage() const;
    void printBlockage(FILE *fp) const;

    void setPropertySize(uint64_t v);
    uint64_t getNumProperties() const;
    void addProperty(ObjectId prop_id);
    ObjectId getPropertiesId() const;

    void printRegion(FILE *fp);

  private:
    SymbolIndex name_;
    ConstraintType type_;
    ConstraintSubType sub_type_;
    int32_t utilization_;  // region & place blkg
    // Shape shape_; TODO
    // effort
    // exclude_cells_;   region only
    std::vector<ObjectId> insts_;  // region only
    ObjectId floorplan_;
    Int32 layer_index_;
    std::string layer_name_;
    ObjectId component_id_;
    std::string component_name_;
    bool has_layer_       :1;
    bool has_placement_   :1;
    bool has_component_   :1;
    bool has_slots_       :1;
    bool has_fills_       :1;
    bool has_pushdown_    :1;     // 5.7
    bool has_exceptpgnet_ :1;  // 5.7
    bool has_soft_        :1;         // 5.7
    bool has_partial_     :1;
    bool has_spacing_     :1;
    bool has_design_rule_width_ :1;
    bool has_mask_        :1;
    Bits64 mask_num_      :3;  // 5.8
    int  reserved_        :17;
    double max_density_;  // 5.7
    Bits64 min_spacing_;
    Bits64 effective_width_;

    ObjectId boxes_id_;
    ObjectId polygons_id_;

    ObjectId properties_id_;
};  // class Constraint

/// @brief Floorplan class
class Floorplan : public Object {
  public:
    using BaseType = Object;

    Floorplan();
    Floorplan(Floorplan &fp);
    Floorplan(Floorplan &&fp);
    ~Floorplan();

    Box getCoreBox() const;
    void setCoreBox(Box &box);

    int32_t getXOffset() const;
    void setXOffset(int32_t &offset);
    int32_t getYOffset() const;
    void setYOffset(int32_t &offset);

    Site *getCoreSite() const;
    ObjectId getCoreSiteId() const;
    void setCoreSiteId(ObjectId &id);

    void copy(Floorplan const &fp);
    void move(Floorplan &&rhs);

    Cell *getCell();

    Constraint *createPlaceBlockage();
    uint64_t getNumOfPlaceBlockages() const;
    ObjectId getPlaceBlockages() const;

    Constraint *createRouteBlockage();
    uint64_t getNumOfRouteBlockages() const;
    ObjectId getRouteBlockages() const;

    Constraint *createRegion(const char *name);
    uint64_t getNumOfRegions() const;
    ObjectId getRegions() const;
    Constraint *getRegion(std::string &name) const;

    Row *createRow();
    ObjectId getRows() const;
    uint64_t getNumOfRows() const;

    Track *createTrack();
    ObjectId getTracks() const;
    uint64_t getNumOfTracks() const;

    Grid *createGcellGrid();
    ObjectId getGcellGrids() const;
    uint64_t getNumOfGrids() const;

    void setDieArea(ObjectIndex die_area);
    ObjectIndex getDieArea();
    Polygon *getDieAreaPolygon();

  private:
    Box core_box_;
    ObjectIndex die_area_;  // ObjectIndex 0 is valid.
    int32_t x_offset_;
    int32_t y_offset_;
    ObjectId core_site_id_;    // ObjectId: 0 is invalid

    ObjectId tracks_;       // TODO: we might need a tree for this.
    ObjectId gcell_grids_;  // TODO: we might need a tree for this.
    ObjectId rows_;
    ObjectId place_blockages_;
    ObjectId route_blockages_;
    ObjectId regions_;
};  // class Floorplan

}  // namespace db
}  // namespace open_edi
#endif
