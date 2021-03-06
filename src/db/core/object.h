/* @file  object.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_OBJECT_H_
#define EDI_DB_OBJECT_H_

#include <vector>

#include "util/util.h"

namespace open_edi {
namespace db {

using namespace open_edi::util;

/// @brief object type enum definition
typedef enum ObjectType {
    kObjectTypeNone = 0,
    kObjectTypeCell,
    kObjecTypeHierData,
    kObjectTypeFloorplan,
    kObjectTypeCellSitePattern,
    kObjectTypeForeign,
    kObjectTypeDensity,
    kObjectTypeDensityLayer,
    //kObjectTypeHierCell,
    kObjectTypeTerm,
    kObjectTypeBus,
    kObjectTypePort,
    kObjectTypeInst,
    kObjectTypePin,
    kObjectTypePinAntennaArea,
    kObjectTypeNet,
    kObjectTypeSpecialNet,
    kObjectTypeWire,
    kObjectTypeSpecialWire,
    kObjectTypeVia,
    kObjectTypeTech,
    kObjectTypeUnits,
    kObjectTypeLayer,
    kObjectTypeLayerMinArea,
    kObjectTypeViaMaster,
    kObjectTypeViaRule,
    kObjectTypeRule,
    kObjectTypeSite,
    kObjectTypeRow,
    kObjectTypeTrack,
    kObjectTypeGcellGrid,
    kObjectTypeFill,
    kObjectTypeScanChain,
    kObjectTypeRegion,
    kObjectTypePhysicalConstraint,
    kObjectTypeGrid,
    kObjectTypeShape,
    kObjectTypeLayerGeometry,
    kObjectTypeGeometry,
    kObjectTypeGeometryVia,
    kObjectTypeMarker,
    kObjectTypeGroup,
    // Timing related
    kObjectTypeTiming,
    kObjectTypeClock,
    kObjectTypeAnalysisView,
    kObjectTypeAnalysisCorner,
    kObjectTypeAnalysisMode,
    kObjectTypeTLib,
    kObjectTypeTCell,
    kObjectTypeTTerm,
    kObjectTypeTPgTerm,
    kObjectTypeArc,
    kObjectTypeDesign,
    kObjectTypeLibSet,
    kObjectTypeOperatingConditions,
    kObjectTypeTUnits,
    kObjectTypeTPvt,
    kObjectTypeWireLoadTable,
    kObjectTypeWireLoad,
    kObjectTypeWireLoadForArea,
    kObjectTypeWireLoadSelection,
    kObjectTypeTableAxis,
    kObjectTypeTableTemplate,
    kObjectTypeTimingTable,
    kObjectTypeTimingTable0,
    kObjectTypeTimingTable1,
    kObjectTypeTimingTable2,
    kObjectTypeTimingTable3,
    kObjectTypeScaleFactors,
    kObjectTypeTFunction,
    kObjectTypeTimingArc,
    kObjectTypeDesignParasitics,
    kObjectTypeNetsParasitics,
    kObjectTypeNetParasitics,
    kObjectTypeDNetParasitics,
    kObjectTypeRNetParasitics,
    kObjectTypeParasiticNode,
    kObjectTypeParasiticIntNode,
    kObjectTypeParasiticPinNode,
    kObjectTypeParasiticExtNode,
    kObjectTypeParasiticDevice,
    kObjectTypeParasiticResistor,
    kObjectTypeParasiticXCap,
    kObjectTypeParasiticCap,
    // Misc
    kObjectTypeVector,
    kObjectTypePropertyDefinition,
    kObjectTypeProperty,
    // geo
    kObjectTypeBox,
    // for ndr rule...
    kObjectTypeNonDefaultRuleLayer,
    kObjectTypeNonDefaultRuleMinCuts,
    // for layer rule
    kObjectTypeCutLayerRule,
    kObjectTypeCutSpacing,
    kObjectTypeSecondLayer,
    kObjectTypeAdjacentCuts,
    kObjectTypeCutSpacingPrlOvlp,
    kObjectTypeEnclosure,
    kObjectTypeEnclosureEol,
    kObjectTypeEnclosureOverhang,
    kObjectTypeArraySpacing,
    kObjectTypeBoundaryEOLBlockage,
    kObjectTypeCornerEOLKeepout,
    kObjectTypeCornerFillSpacing,
    kObjectTypeCornerSpacing,
    kObjectTypeDirSpanLengthSpTbl,
    kObjectTypeSpanLength,
    kObjectTypeExactSLSpacing,
    kObjectTypeEOLKeepout,
    kObjectTypeMinCut,
    kObjectTypeMinEnclArea,
    kObjectTypeMinSize,
    kObjectTypeMinStep,
    kObjectTypeWidthSpTbl,
    kObjectTypeInfluenceSpTbl,
    kObjectTypeParaSpanLenTbl,
    kObjectTypeProtrusionRule,
    kObjectTypeProtrusionWidth,
    kObjectTypeRoutingSpacing,
    kObjectTypeRoutingLayerRule,
    kObjectTypeTrimLayerRule,
    kObjectTypeMEOLLayerRule,
    kObjectTypeAntennaModel,
    kObjectTypeMinArea,
    kObjectTypeCurrentDen,
    kObjectTypeCurrentDenContainer,
    kObjectTypeImplantCoreEdgeLength,
    kObjectTypeImplantSpacing,
    kObjectTypeImplantWidth,
    kObjectTypeImplantLayerRule,
    // for site
    kObjectTypeSitePatternPair,
    kObjectTypeArray,
    kObjectTypeArraySegment,
    kObjectTypeMaxViaStack,
    kObjectTypeAntennaModelTerm,
    kObjectTypeInternalVectorStarts = 4096,
    kObjectTypeMax
} ObjectType;

/// @brief Base class for all objects.

#define NEW_ARRAY_OBJECT
#ifndef NEW_ARRAY_OBJECT
template <typename T>
class ArrayObject : public std::vector<T> {
  public:
    ArrayObject() : std::vector<T>(0) {}
};
#endif

#define UNINIT_OBJECT_ID 0
class Cell;
class StorageUtil;

class Object {
  public:
    using CoordinateType = CoordinateTraits<int32_t>::CoordinateType;
    using IndexType = CoordinateTraits<CoordinateType>::IndexType;

    template <class T>
    static T *createVectorObject();
    template <class T>
    static void deleteVectorObject(T *obj);

    template <class T>
    static T* addr(ObjectId obj_id);

    template <class T>
    static T *createObject(ObjectType type, ObjectId owner_id);
    template <class T>
    static void deleteObject(T *obj);

    /// @brief default constructor
    Object();

    /// @brief constructor
    Object(Object *o, IndexType id);

    /// @brief copy constructor
    Object(Object const &rhs);

    /// @brief move constructor
    Object(Object &&rhs);

    /// @brief copy assignment
    Object &operator=(Object const &rhs);

    /// @brief move assignment
    Object &operator=(Object &&rhs);

    /// @brief destructor
    ~Object();

    /// @brief get the owner of the object
    /// @return the owner id
    ObjectId const getOwnerId() const { return owner_; }

    /// @brief get the owner of the object
    /// @return the owner id
    ObjectId getOwnerId() { return owner_; }

    /// @brief get the owner cell of the object
    /// @return the owner Cell
    Cell *getOwnerCell() const;

    /// @brief get the storage_util by the specified owner_id
    /// @return the StorageUtil ptr
    static StorageUtil *getStorageUtilById(ObjectId owner_id);
    static MemPagePool *getPoolById(ObjectId owner_id);

    /// @brief set the owner of the object
    void setOwner(Object *v) { owner_ = v->getId(); }
    void setOwner(ObjectId id) { owner_ = id; }

    /// @brief get id of the object
    /// @return id
    ObjectId getId() const { return id_; }

    /// @brief set id of the object
    /// @param id
    void setId(IndexType id) { id_ = id; }

    /// @brief get db type of the object
    /// @return type
    ObjectType getObjectType() const { return type_; }

    /// @brief set id of the object
    /// @param type
    void setObjectType(ObjectType type) { type_ = type; }

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    void setIsValid(int v);
    int  getIsValid();
    void setIsSelected(int v);
    int  getIsSelected();
    void setIsHighlight(int v);
    int  getIsHighlight();
    void setIsModified(int v);
    int  getIsModified();
    void setIsMarked(int v);
    int  getIsMarked();

  protected:
    /// @brief copy object
    void copy(Object const &rhs);
    /// @brief move object
    void move(Object &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, Object const &rhs);
    /// @brief overload input stream
    friend IStreamBase &operator>>(IStreamBase &is, Object &rhs);

    template <class T>
    static int __getInternalTypeForVectorObject();

    ObjectId __createObjectIdArray(int64_t size);
    void __deleteObjectIdArray(ObjectId array_id);

    IndexType is_valid_ : 1;
    IndexType is_selected_ : 1;
    IndexType is_highlight_ : 1;
    IndexType is_modified_ : 1;
    IndexType is_marked_ : 1;
    IndexType null_ : 3;
    ObjectId id_ : 56;  ///< object index
    ObjectType type_;

    ObjectId owner_;  ///< parent object of this object
};

template <class T>
int Object::__getInternalTypeForVectorObject() {
    int type = kObjectTypeInternalVectorStarts + (sizeof(T) >> 3);
    return type;
}

template <class T>
T *Object::createVectorObject() {
    T *obj = nullptr;
    ObjectId id;

    int internal_type = __getInternalTypeForVectorObject<T>();
    MemPagePool *pool = MemPool::getCurrentPagePool();

    if (!pool) {
        message->issueMsg(
            kError,
            "Cannot create object for type %d because memory pool is null.\n",
            internal_type);
        return nullptr;
    }

    obj = pool->allocate<T>(internal_type, id);
    obj->setId(id);
    obj->setObjectType(kObjectTypeVector);

    return obj;
}

template <class T>
void Object::deleteVectorObject(T *obj) {
    if (!obj) return;

    int internal_type = __getInternalTypeForVectorObject<T>();
    MemPagePool *pool = MemPool::getPagePoolByObjectId(obj->getId());

    if (!pool) {
        message->issueMsg(
            kError,
            "Cannot delete object for type %d because memory pool is null.\n",
            internal_type);
    }

    pool->free<T>(internal_type, obj);
    obj = nullptr;
}

template <class T>
T* Object::addr(uint64_t obj_id)
{
    if (obj_id == 0) return nullptr;

    MemPagePool *pool = MemPool::getPagePoolByObjectId(obj_id);
    if (pool) {
        return pool->getObjectPtr<T>(obj_id);
    } else {
        return nullptr;
    }
}

/// @brief createObject create object within memory pool
/// @return the object created.
template <class T>
T* Object::createObject(ObjectType type, ObjectId owner_id) {
    assert(type > kObjectTypeNone && type < kObjectTypeMax);
    assert(owner_id != 0);
    MemPagePool *pool = getPoolById(owner_id);

    if (!pool) {
        message->issueMsg(kError,
                          "Cannot create object for type %d because memory "
                          "pool is null.\n",
                          type);
        return nullptr;
    }
    T *obj = nullptr;
    ObjectId id;
    obj = pool->allocate<T>(type, id);
    if (!obj) {
        message->issueMsg(kError, "Pool allocate null object.\n");
        return nullptr;
    }
    obj->setId(id);
    obj->setObjectType(type);
    obj->setIsValid(1);
    obj->setOwner(owner_id);

    return obj;
}

template <class T>
void Object::deleteObject(T *obj) {
    if (!obj) return;

    if (obj->getId()) {
        ObjectType type = obj->getObjectType();
        MemPagePool *pool = MemPool::getPagePoolByObjectId(obj->getId());

        if (!pool) {
            // Internal debug message:
            message->issueMsg(kError,
                              "Cannot delete object for type %d because memory "
                              "pool is null.\n",
                              type);
            return;
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

#endif
