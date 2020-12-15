/**
 * @file  cell.h
 * @date  09/18/2020 01:38:36 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_CORE_CELL_H_
#define SRC_DB_CORE_CELL_H_

#include <string>
#include <vector>
#include "db/core/root.h"

#include "db/core/bus.h"
#include "db/core/fill.h"
#include "db/core/fplan.h"
#include "db/core/group.h"
#include "db/core/inst.h"
#include "db/core/net.h"
#include "db/core/pin.h"
#include "db/core/scan_chain.h"
#include "db/core/special_net.h"
#include "db/core/term.h"
#include "db/tech/tech.h"
#include "db/util/box.h"
#include "db/util/geometrys.h"
#include "db/util/symbol_table.h"
#include "util/polygon_table.h"
#include "util/util.h"

namespace open_edi {
namespace db {

class SpecialNet;
class StorageUtil;

enum CellClassType {
    kNoClass = 0,
    kCover = 1,
    kRing = 2,
    kBlock = 3,
    kPad = 4,
    kCore = 5,
    kEndcap = 6, 
    kUnknownClass
};

enum CellSubClassType {
    kNoSubClass = 0,
    kCoverBump = 1,
    kBlockBox = 2,
    kBlockSoft = 3,
    kPadInput = 4,
    kPadOutput = 5,
    kPadInout = 6,
    kPadPower = 7,
    kPadSpacer = 8,
    kPadAreaIO = 9,
    kCoreFeedthru = 10,
    kCoreTiehigh = 11,
    kCoreTielow = 12,
    kCoreSpacer = 13,
    kCoreAntennaCell = 14,
    kCoreWelltap = 15,
    kEndcapPre = 16,
    kEndcapPost = 17,
    kEndcapTopLeft = 18,
    kEndcapTopRight = 19,
    kEndcapBottomLeft = 20,
    kEndcapBottomRight = 21, 
    kUnknownSubClass
};

class Foreign : public Object {
  public:
    Foreign() {}
    ~Foreign() {}
    int hasForeignPoint() const { return hasForeignPoint_; }
    void sethasForeignPoint(int hasForeignPoint_value) {
        hasForeignPoint_ = hasForeignPoint_value;
    }
    int getforeignX() const { return foreignX_; }
    int getforeignY() const { return foreignY_; }
    void setforeignX(int value) { foreignX_ = value; }
    void setforeignY(int value) { foreignY_ = value; }

    int hasForeignOrient() const {
        return (foreignOrient_ == Orient::kUnknown) ? 0 : 1;
    }
    const char *foreignOrientStr() const {
        return toString(foreignOrient_).c_str();
    }
    void setforeignOrient(Orient value) { foreignOrient_ = value; }
    Orient getforeignOrient() const { return foreignOrient_; }
    std::string getName() const;
    void setName(const char *v);

  private:
    // int foreignAllocated_;  // not used outside
    // int  hasForeignOrigin_; // not used outside
    int hasForeignPoint_;
    Orient foreignOrient_;
    int foreignX_;
    int foreignY_;
    SymbolIndex name_index_;
};

class DensityLayer : public Object {
  public:
    DensityLayer() {}
    ~DensityLayer() {}
    void setRect(Box rect) { rect_ = rect; }
    Box getRect() const { return rect_; }
    void setDensity(int value) { density_ = value; }
    int getDensity() const { return density_; }

  private:
    Box rect_;
    int density_;
};

class Density : public Object {
  public:
    Density() {}
    ~Density() {}
    void setName(const char *value);
    std::string getName() const;
    void addDensityLayer(ObjectId id);
    DensityLayer *getDensityLayer(int i) const;
    int getDensityLayerNum() const;

  private:
    ObjectId density_layers_;
    SymbolIndex name_index_;
};

class StepPattern {
  public:
    StepPattern() : xCount_(0), yCount_(0), xStep_(0), yStep_(0) {}
    StepPattern(int xst, int yst, int xsp, int ysp)
        : xCount_(xst), yCount_(yst), xStep_(xsp), yStep_(ysp) {}
    ~StepPattern() {}
    void setxCount(int value) { xCount_ = value; }
    int getxCount() const { return xCount_; }
    void setyCount(int value) { yCount_ = value; }
    int getyCount() const { return yCount_; }
    void setxStep(int value) { xStep_ = value; }
    int getxStep() const { return xStep_; }
    void setyStep(int value) { yStep_ = value; }
    int getyStep() const { return yStep_; }

  private:
    int xCount_;
    int yCount_;
    int xStep_;
    int yStep_;
};

class SitePattern : public Object {
  public:
    SitePattern() {}
    ~SitePattern() {}
    std::string getName() const;
    Site *getSite();
    void setSiteID(ObjectId v);
    void setxOrigin(int value) { x_origin_ = value; }
    int getxOrigin() const { return x_origin_; }
    void setyOrigin(int value) { y_origin_ = value; }
    int getyOrigin() const { return y_origin_; }
    void setOrient(Orient v) { site_orient_ = v; }
    Orient getOrient() const { return site_orient_; }
    void setStepPattern(StepPattern *v);
    StepPattern getStepPattern() const { return sp_; }

  private:
    ObjectId site_;
    int x_origin_;
    int y_origin_;
    Orient site_orient_;
    StepPattern sp_;
};

class HierData : public Object {
  public:
    // Utils: symbol table, polygon table, pool, etc.
    void setSymbolTable(SymbolTable *stb);
    SymbolTable *getSymbolTable();
    void setPolygonTable(PolygonTable *pt);
    PolygonTable *getPolygonTable();
    std::string &getSymbolByIndex(SymbolIndex index);
    bool addSymbolReference(SymbolIndex index, ObjectId owner);
    void setPool(MemPagePool *p);
    MemPagePool *getPool();
    void setStorageUtil(StorageUtil *v);
    StorageUtil* getStorageUtil() const;

    // void setTechLibId(ObjectId v);
    // ObjectId getTechLibId() const;
    void setFloorplanId(ObjectId v);
    ObjectId getFloorplanId() const;

    // Get object vector:
    ObjectId getCells() const;
    ObjectId getInstances() const;
    ObjectId getBuses() const;
    ObjectId getNets() const;
    ObjectId getSpecialNets() const;
    ObjectId getIOPins() const;
    ObjectId getGroups() const;
    ObjectId getFills() const;
    ObjectId getScanChains() const;
    ObjectId getRegions() const;

    // Set object vector:
    void setCells(ObjectId v);
    void setInstances(ObjectId v);
    void setBuses(ObjectId v);
    void setNets(ObjectId v);
    void setSpecialNets(ObjectId v);
    void setIOPins(ObjectId v);
    void setGroups(ObjectId v);
    void setFills(ObjectId v);
    void setScanChains(ObjectId v);
    void setRegions(ObjectId v);

  private:
    void __init();

    StorageUtil *storage_util_; // runtime object.
    ObjectId floor_plan_;
    // ObjectId tech_lib_;  ///< tech LEF information
    ObjectId cells_;  ///< Macro in LEF, module in Verilog,full implemented sub
                      ///< blocks. Only used by hierarchical cell
    ObjectId instances_;  ///< Instantiation of a cell
    ObjectId buses_;
    ObjectId nets_;  ///< connection between devices
    ObjectId special_nets_;
    ObjectId io_pins_;  ///< IO pins in top cell
    ObjectId groups_;
    ObjectId fills_;        /// FILLS defined in DEF
    ObjectId scan_chains_;  /// SCANCHAINS defined in DEF
    ObjectId regions_;      /// REGIONS defined in DEF
};

/// @brief cell class for cells/stencils in the library
class Cell : public Object {
  public:
    using BaseType = Object;

    template <class T>
    T *createObject(ObjectType type);
    template <class T>
    void deleteObject(T *obj);

    template <class T>
    T *getObjectByName(const std::string &);

    Cell();
    Cell(Object *owner, ObjectId id);
    ~Cell();

    // Name and CellType:
    CellType getCellType() const { return cell_type_; }
    void setCellType(CellType const &v);
    bool isHierCell() const;
    SymbolIndex getNameIndex();
    std::string const &getName();
    void setName(std::string &v);

    // Utils: symbol table, polygon table, pool, etc.
    void setSymbolTable(SymbolTable *stb);
    SymbolTable *getSymbolTable();
    SymbolTable *getParentOrTopSymbolTable();
    void setPolygonTable(PolygonTable *pt);
    PolygonTable *getPolygonTable();
    std::string &getSymbolByIndex(SymbolIndex index);
    SymbolIndex getOrCreateSymbol(const char *name);
    SymbolIndex getOrCreateSymbol(std::string &name);
    bool addSymbolReference(SymbolIndex index, ObjectId owner);
    void setPool(MemPagePool *p);
    MemPagePool *getPool();
    StorageUtil *getStorageUtil();
    void setStorageUtil(StorageUtil *v);
    void initHierData(StorageUtil *v);
    // void initHierData();

    // Get object vector size:
    uint64_t getNumOfCells() const;
    uint64_t getNumOfTerms() const;
    uint64_t getNumOfBuses() const;
    uint64_t getNumOfInsts() const;
    uint64_t getNumOfNets() const;
    uint64_t getNumOfSpecialNets() const;
    uint64_t getNumOfIOPins() const;
    uint64_t getNumOfGroups() const;
    uint64_t getNumOfFills() const;
    uint64_t getNumOfScanChains() const;
    uint64_t getNumOfForeigns() const;
    uint64_t getNumOfSitePatterns() const;

    // Get object by name:
    Cell *getCell(std::string name);
    Cell *getCellFromTechLib(std::string name);

    Term *getTerm(std::string name);

    Bus *getBus(std::string name);
    Net *getNet(std::string name);
    Inst *getInstance(std::string name);
    SpecialNet *getSpecialNet(std::string name);
    Pin *getPin(std::string name);
    Pin *getIOPin(const std::string &name);
    Pin *getVPin(const std::string &name);
    Group *getGroup(std::string &name);

    // Get object vector:
    ObjectId getInstances() const;
    ObjectId getCells() const;
    ObjectId getTerms() const;
    ObjectId getBuses() const;
    ObjectId getNets() const;
    ObjectId getSpecialNets() const;
    ObjectId getGroups() const;
    ObjectId getIOPins() const;
    ObjectId getFills() const;
    ObjectId getScanChains() const;
    ObjectId getForeigns() const;

    ArrayObject<ObjectId> *getCellArray() const;
    ArrayObject<ObjectId> *getInstanceArray() const;
    ArrayObject<ObjectId> *getTermArray() const;
    ArrayObject<ObjectId> *getBusArray() const;
    ArrayObject<ObjectId> *getNetArray() const;
    ArrayObject<ObjectId> *getSpecialNetArray() const;
    ArrayObject<ObjectId> *getGroupArray() const;

    // Get object by index/ID:
    Pin *getIOPinById(ObjectId obj_id);        // TODO: to be removed.
    Inst *getInstance(ObjectId obj_id) const;  // TODO: to be removed.
    Pin *getIOPin(size_t idx);
    Term *getTerm(size_t idx) const;
    Group *getGroup(size_t idx) const;
    Fill *getFill(size_t idx) const;
    ScanChain *getScanChain(size_t idx) const;
    Cell *getCell(int i) const;
    Foreign *getForeign(int i) const;
    Density *getDensity(int index);
    LayerGeometry *getOBS(int index) const;
    ArrayObject<ObjectId> *getOBSes() const;


    // add object (by ID) into vector
    void addCell(ObjectId id);
    void addTerm(ObjectId id);
    void addBus(ObjectId id);
    void addNet(ObjectId id);
    void addSpecialNet(ObjectId id);
    void addInstance(ObjectId id);
    void addIOPin(ObjectId id);
    void addVPin(ObjectId id);
    void addGroup(ObjectId sp);
    void addFill(ObjectId id);
    void addScanChain(ObjectId id);
    void addSitePattern(ObjectId sp);
    void addForeign(ObjectId f);
    void addDensity(ObjectId density);
    void addOBS(ObjectId id);

    void setForeigns(ObjectId id);

    // the following functions only allocate an object in the pool, the content
    // should be filled by the caller.
    Floorplan *createFloorplan();
    Cell *createCell(std::string &name, bool isHier = false);
    Term *createTerm(std::string &name);
    Bus *createBus(std::string &name);
    Inst *createInstance(std::string &name);
    Net *createNet(std::string &name);
    SpecialNet *createSpecialNet(std::string &name);
    Pin *createIOPin(std::string &name);
    Pin *createVPin(std::string &name);
    Group *createGroup(std::string &name);
    Fill *createFill();
    ScanChain *createScanChain(std::string &name);
    //?
    void deleteCell(Cell *cell);

    // timinglib
    void resetTerms(const std::vector<Term *> &terms);
    // timinglib

    // Container: tech, floorplan, etc.
    // void setTechLib(Tech *t);
    Tech *getTechLib();
    Layer *getLayerByLayerId(Int32 id);
    void setFloorplan(Floorplan *fp);
    Floorplan *getFloorplan();

    // Physical attribute access:
    void setHasSize(int value) { has_size_ = value; }
    int hasSize() { return has_size_; }
    void setSizeX(int value) { sizeX_ = value; }
    int getSizeX() { return sizeX_; }
    int getSizeY() { return sizeY_; }
    void setSizeY(int value) { sizeY_ = value; }
    void setHasOrigin(int value) { has_origin_ = value; }
    int hasOrigin() { return has_origin_; }
    void setOriginX(int value) { originX_ = value; }
    int getOriginX() { return originX_; }
    void setOriginY(int value) { originY_ = value; }
    int getOriginY() { return originY_; }
    void setHas90Symmetry(bool v) { has_90_symmetry_ = v; }
    bool has90Symmetry() const { return has_90_symmetry_; }
    void setHasYSymmetry(bool v) { has_y_symmetry_ = v; }
    bool hasYSymmetry() const { return has_y_symmetry_; }
    void setHasXSymmetry(bool v) { has_x_symmetry_ = v; }
    bool hasXSymmetry() const { return has_x_symmetry_; }
    void setHasSiteName(int value) { has_site_name_ = value; }
    int hasSiteName() { return has_site_name_; }
    void setSiteID(ObjectId v) { site_ = v; }
    ObjectId getSiteID() { return site_; }
    std::string getSiteName();
    Site *getSite();
    // void setNumSites(int value) { site_num_ = value; }
    // int getNumSites() { return site_num_; }
    SitePattern *getSitePattern(int i) const;
    // int numForeigns() const { return foreign_num_; }
    // void setNumForeigns(int value) { foreign_num_ = value; }

    void setClass(const char *cls);
    std::string const getClassString();
    bool isClassCover() { return class_type_ == kCover? true : false;}
    bool isClassRing() { return class_type_ == kRing? true : false;}
    bool isClassBlock() { return class_type_ == kBlock? true : false;}
    bool isClassPad() { return class_type_ == kPad? true : false;}
    bool isClassCore() { return class_type_ == kCore? true : false;}
    bool isClassEndCap() { return class_type_ == kEndcap? true : false;}
    bool isSubClassCoverBump() { return sub_class_type_ == kCoverBump? true : false;}
    bool isSubClassBlockBox() { return sub_class_type_ == kBlockBox? true : false;}
    bool isSubClassBlockSoft() { return sub_class_type_ == kBlockSoft? true : false;}
    bool isSubClassPadInput() { return sub_class_type_ == kPadInput? true : false;}
    bool isSubClassPadOutput() { return sub_class_type_ == kPadOutput? true : false;}
    bool isSubClassPadInout() { return sub_class_type_ == kPadInout? true : false;}
    bool isSubClassPadPower() { return sub_class_type_ == kPadPower? true : false;}
    bool isSubClassPadSpacer() { return sub_class_type_ == kPadSpacer? true : false;}
    bool isSubClassPadAreaIO() { return sub_class_type_ == kPadAreaIO? true : false;}
    bool isSubClassCoreFeedthru() { return sub_class_type_ == kCoreFeedthru? true : false;}
    bool isSubClassCoreTiehigh() { return sub_class_type_ == kCoreTiehigh? true : false;}
    bool isSubClassCoreTielow() { return sub_class_type_ == kCoreTielow? true : false;}
    bool isSubClassCoreSpacer() { return sub_class_type_ == kCoreSpacer? true : false;}
    bool isSubClassCoreAntennaCell() { return sub_class_type_ == kCoreAntennaCell? true : false;}
    bool isSubClassCoreWelltap() { return sub_class_type_ == kCoreWelltap? true : false;}
    bool isSubClassEndcapPre() { return sub_class_type_ == kEndcapPre? true : false;}
    bool isSubClassEndcapPost() { return sub_class_type_ == kEndcapPost? true : false;}
    bool isSubClassEndcapTopLeft() { return sub_class_type_ == kEndcapTopLeft? true : false;}
    bool isSubClassEndcapTopRight() { return sub_class_type_ == kEndcapTopRight? true : false;}
    bool isSubClassEndcapBottomLeft() { return sub_class_type_ == kEndcapBottomLeft? true : false;}
    bool isSubClassEndcapBottomRight() { return sub_class_type_ == kEndcapBottomRight? true : false;}

    void setIsFixedMask(int mask) { is_fixed_mask_ = mask; }
    int getIsFixedMask() { return is_fixed_mask_; }
    void setHasEEQ(int value) { has_eeq_ = value; }
    int hasEEQ() { return has_eeq_; }
    void setEEQ(const char *v);
    std::string const &getEEQ();

    void setNumMaskShiftLayers(uint8_t num);
    uint8_t getNumMaskShiftLayers();
    bool addMaskShiftLayer(ObjectId layer_id);
    ObjectId getMaskShiftLayer(uint8_t index);

    // Print:
    // void print();
    void printLEF(std::ofstream &ofs);

  private:
    void __init();
    const HierData *__getConstHierData() const;
    HierData *__getHierData();
    //void __initHierData();

    SymbolIndex name_index_;  ///< cell name
    CellType cell_type_;      ///< cell type
    CellClassType class_type_;
    CellSubClassType sub_class_type_;
    ObjectId hier_data_id_;   ///< hier only data: check with is_hierarchical_;

    int originX_;
    int originY_;
    int sizeX_;
    int sizeY_;

    ObjectId terms_;  ///< corresponding to Verilog module port definition or
                      ///< LEF macro pin definitions.

    SymbolIndex eeq_index_;
    ObjectId site_;
    ObjectId site_patterns_;
    ObjectId foreigns_;
    ObjectId densities_;
    ObjectId obses_;
   
    ///< Component mask shift in DEF
    ObjectId mask_shift_layers_[max_layer_num];
    uint8_t num_mask_shift_layers_;
    
    // Bits:
    Bits has_origin_ : 1;
    Bits has_eeq_ : 1;
    Bits has_size_ : 1;
    Bits has_x_symmetry_ : 1;
    Bits has_y_symmetry_ : 1;
    Bits has_90_symmetry_ : 1;
    Bits has_site_name_ : 1;
    Bits is_fixed_mask_ : 1;
};

/// @brief createObject create object within memory pool
/// @return the object created.
template <class T>
T *Cell::createObject(ObjectType type) {
    assert(type > kObjectTypeNone && type < kObjectTypeMax);

    T *obj = nullptr;
    ObjectId id;
    MemPagePool *pool = getPool();

    if (!pool) {
        message->issueMsg(kError,
                          "Cannot create object for type %d because memory "
                          "pool is null.\n",
                          type);
        return nullptr;
    }

    obj = pool->allocate<T>(type, id);
    if (!obj) {
        message->issueMsg(kError, "Pool allocate null object.\n");
        return nullptr;
    }
    obj->setId(id);
    obj->setObjectType(type);
    obj->setIsValid(1);
    obj->setOwner(this->getId());

    return obj;
}

template <class T>
void Cell::deleteObject(T *obj) {
    if (!obj) return;

    if (obj->getId()) {
        ObjectType type = obj->getObjectType();
        MemPagePool *pool = getPool();

        if (!pool) {
            message->issueMsg(kError,
                              "Cannot delete object for type %d because memory "
                              "pool is null.\n",
                              type);
        }

        obj->setIsValid(0);
        pool->free<T>(type, obj);
        obj = nullptr;
    } else {
        delete obj;
    }
}

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_CORE_CELL_H_
