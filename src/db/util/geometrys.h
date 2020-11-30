/* @file  geometry.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_UTIL_GEOMETRYS_H_
#define SRC_UTIL_GEOMETRYS_H_
#include  <string>
#include "util/util.h"
#include "util/point.h"
#include "db/util/box.h"
#include "util/polygon_table.h"
#include "db/util/symbol_table.h"
#include "db/core/object.h"
// #include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {

enum GeometryType{
    kNoGeomType = 0,
    kPath = 1,
    kRect = 2,
    kPolygon = 3,
    kVia = 4,
};

class Geometry: public Object{
 public:
    Geometry() {color_mask_ = 0;}
    ~Geometry() {}
    GeometryType getType() const { return type_; }
    void setType(GeometryType t) { type_ = t; }
    bool isPath() { return getType() == kPath; }
    bool isRect() { return getType() == kRect; }
    bool isPolygon() { return getType() == kPolygon; }
    int getNumMask() const { return color_mask_; }
    void setNumMask(int c) { color_mask_ = c; }
    void setPathID(int id) { path_id = id;}
    int getPathID() const {return path_id;}
    void setBox(Box* b) { box.setBox(b);}
    const Box& getBox() const { return box;}
    void setPolygonID(int id) { polygon_id = id;}
    int getPolygonID() const {return polygon_id;}

 private:
    GeometryType type_;
    int color_mask_;
    union{
        uint64_t path_id;
        Box box;
        uint64_t polygon_id;
    };
};

class GeometryVia: public Object{
 public:
    GeometryVia() {}
    ~GeometryVia() {}
    SymbolTable* getSymbolTable();
    SymbolIndex getOrCreateSymbol(const char *name);
    void setName(const char *value) { name_index_ = getOrCreateSymbol(value); }
    std::string getName();
    void setPoint(Point value) { p_ = value; }
    Point getPoint() const { return p_; }
    void setTopMaskNum(int value) { top_mask_num_ = value; }
    int getTopMaskNum() const { return top_mask_num_; }
    void setCutMaskNum(int value) { cut_mask_num_ = value; }
    int getCutMaskNum() const { return cut_mask_num_; }
    void setBottomMaskNum(int value) { bottom_mask_num_ = value; }
    int getBottomMaskNum() const { return bottom_mask_num_; }

 private:
    int64_t name_index_;
    Point p_;
    int top_mask_num_;
    int cut_mask_num_;
    int bottom_mask_num_;
};

class LayerGeometry: public Object{
 public:
     LayerGeometry();
     ~LayerGeometry();
     SymbolTable* getSymbolTable();
    SymbolIndex getOrCreateSymbol(const char *name);
    GeometryType getType() const { return type_; }
    void setType(GeometryType t) { type_ = t; }
    void addGeometry(ObjectId id);
    void addGeometryVia(ObjectId id);
    int getVecNum() const;
    // int getGeometrySize(){ return vct->totalSize()? type_ != kVia : 0;}
    Geometry* getGeometry(int i) const;
    // int getGeometryViaSize(){ return vct->totalSize()? type_ != kVia : 0;}
    GeometryVia* getGeometryVia(int i) const;
    void setName(const char *value) { name_index_ = getOrCreateSymbol(value); }
    std::string getName();
    void sethasEXCEPTPGNET(bool value) { has_EXCEPTPGNET_ = value; }
    bool hasEXCEPTPGNET() const { return has_EXCEPTPGNET_; }
    void setMinSpacing(int value) { min_spacing_ = value; }
    int getMinSpacing() const { return min_spacing_; }
    void setdesignRuleWidth(int value) { design_rule_width_ = value; }
    int getdesignRuleWidth() const { return design_rule_width_; }
    void setWidth(int value) { width_ = value; }
    int getWidth() const { return width_; }
    PolygonTable* getPolygonTable();
    int64_t CreatePolygon(Polygon* p);
    void print();
    void printLEF(std::ofstream & ofs, bool from_port = false);
    bool hasSpacing() const { return has_spacing_; }
    void setHasSpacing(bool value) { has_spacing_ = value; }
    bool hasDRW() const { return has_drw_; }
    void setHasDRW(bool value) { has_drw_ = value; }

 private:
    ObjectId geometrys_;
    GeometryType type_;
    SymbolIndex name_index_;
    bool has_EXCEPTPGNET_;
    bool has_spacing_;
    int min_spacing_;
    bool has_drw_;
    int design_rule_width_;
    int width_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_UTIL_GEOMETRYS_H_
