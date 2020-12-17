/* @file  db.h
 * @date  Nov 2020
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NiiCEDA Inc.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/*************************************************************************/                         

#ifndef _DREAMPLACE_OEM_DB_H
#define _DREAMPLACE_OEM_DB_H

#include <vector>
#include <limits>
#include "utility/src/Msg.h"

#include "db/core/db.h"
#include "db/core/fplan.h"
#include "db/core/group.h"
#include "db/core/inst.h"
#include "db/core/net.h"
#include "db/core/pin.h"
#include "db/core/special_net.h"
#include "db/core/term.h"
#include "db/tech/tech.h"
#include "db/util/box.h"
#include "util/util.h"

DREAMPLACE_BEGIN_NAMESPACE
//using namespace dbi;
// db typedef
namespace dbi = open_edi::db;
namespace uti = open_edi::util;
typedef int32_t PlInt;
typedef uint64_t PlUInt;
typedef double PlDouble;
typedef dbi::Bits PlBits;
typedef std::string String;
typedef dbi::Object PlObj;
typedef dbi::ObjectId PlObjId;
typedef dbi::ArrayObject<PlObjId> PlArrayObj;
typedef dbi::Net PlNet;
typedef dbi::Pin PlPin;
typedef dbi::Term PlTerm;
typedef dbi::Inst PlInst;
typedef dbi::Cell PlCell;
typedef dbi::Group PlGroup;
typedef dbi::Box PlBox;
typedef dbi::Site PlSite;
typedef dbi::Tech PlTech;
typedef dbi::Point PlPoint;
typedef dbi::Orient PlOrient;
typedef dbi::Floorplan PlFloorplan;
typedef dbi::PlaceStatus PlStatus;
typedef dbi::Constraint PlConstraint;
typedef dbi::SignalType PlSignalType;
typedef dbi::Port PlPort;
typedef dbi::Constraint::ConstraintType PlConType;
typedef dbi::Constraint::ConstraintSubType PlConSubType;
typedef dbi::Constraint::ConstraintType kPlConType;
typedef dbi::Constraint::ConstraintSubType kPlConSubType;
typedef dbi::LayerGeometry PlLGeometry;
typedef dbi::Geometry PlGeometry;
typedef dbi::Row PlRow;
typedef dbi::Root PlRoot;
typedef dbi::SignalDirection PlDirection;

typedef uti::PlaceStatus kPlStatus;
typedef uti::Polygon PlPolygon;
// interface to edi DB 
// root
inline PlRoot*        getRoot()                          { return dbi::getRoot(); }
// tech
inline PlTech*        getTechLib()                       { return getRoot()->getTechLib(); }
inline PlDouble       dbuToMicrons(PlInt i)              { return getTechLib()->dbuToMicrons(i); }
inline PlArrayObj*    getSiteArray()                     { return getTechLib()->getSiteArray(); }
inline PlSite*        getCoreSite()                      { return PlObj::addr<PlSite>(*(getSiteArray()->begin())); }    // frist is core site
inline PlInt          getSiteH(PlSite* site)             { return site->getHeight(); }
inline PlInt          getSiteW(PlSite* site)             { return site->getWidth(); }
// top cell
inline PlCell*        getPlTopCell()                     { return dbi::getTopCell(); }
// net
inline PlUInt         getNumOfNets()                     { return getPlTopCell()->getNumOfNets(); }
inline PlObjId        getNetsId()                        { return getPlTopCell()->getNets(); }
inline PlArrayObj*    getNetArray()                      { return getPlTopCell()->getNetArray(); }
inline PlUInt         getNumOfSpecialNets()              { return getPlTopCell()->getNumOfSpecialNets(); }
inline PlArrayObj*    getSpecialNetArray()               { return getPlTopCell()->getSpecialNetArray(); }
inline PlNet*         getNet(PlObjId idx)                { return PlObj::addr<PlNet>(idx); } // need API
inline String const&  getNetName(PlNet* net)             { return net->getName(); }
inline PlBits         getNetType(PlNet* net)             { return net->getType(); }
inline bool           isNetClock(PlNet* net)             { return (getNetType(net) == dbi::kNetTypeClock); }
inline PlDouble       getNetWeight(PlNet* net)           { return net->getWeight(); }
// term
inline PlUInt         getNumOfTerms()                    { return getPlTopCell()->getNumOfTerms(); }
inline String const   getTermName(PlTerm* term)          { return term->getName(); }
inline bool           isTermInput(PlTerm* term)          { return term->isInput(); }
inline bool           isTermOutput(PlTerm* term)         { return term->isOutput(); }
inline bool           isTermInOut(PlTerm* term)          { return term->isInOut(); }
inline bool           isTermUsed(PlTerm* term)           { return term->hasUse(); }
inline bool           isTermPG(PlTerm* term)             { return term->isPGType(); }
inline PlSignalType   getTermPinType(PlTerm* term)       { return term->getPinType(); }
inline int            getTermPortNum(PlTerm* term)       { return term->getPortNum(); }
inline bool           isTermHasPort(PlTerm* term)        { return getTermPortNum(term) > 0 ? true : false; }     
inline PlPort*        getTermPortById(PlTerm* term, int idx)      { return term->getPort(idx); }
inline PlPoint        getPortLoc(PlPort* port)                    { return port->getLocation(); }
inline PlPoint        getTermLoc(PlTerm* term)                    { return getPortLoc(getTermPortById(term, 0)); }
inline PlInt          getPortLayerGeometryNum(PlPort* port)       { return port->getLayerGeometryNum(); }
inline bool           isPortHasLGeometry(PlPort* port)            { return getPortLayerGeometryNum(port) > 0 ? true : false; }     
inline PlLGeometry*   getPortLGeometryById(PlPort* port, int idx) { return port->getLayerGeometry(idx); }
inline PlGeometry*    getGeometry(PlLGeometry* lGeo, int idx)     { return lGeo->getGeometry(idx); }
inline PlBox          getGeometryBox(PlGeometry* geo)             { return geo->getBox(); }
// pin
inline PlUInt         getNumOfIOPins()                   { return getPlTopCell()->getNumOfIOPins(); }
inline PlArrayObj*    getNetPinArray(PlNet* net)         { return net->getPinArray(); }
inline PlPin*         getPin(PlObjId idx)                { return PlObj::addr<PlPin>(idx); } // need API
inline PlPin*         getIOPin(PlObjId idx)              { return getPlTopCell()->getIOPin(idx); } // need API
inline PlDirection    getPinDirection(PlPin* pin)        { return pin->getDirection(); }
inline PlNet*         getPinNet(PlPin* pin)              { return pin->getNet(); }
inline String const&  getPinName(PlPin* pin)             { return pin->getName(); }
inline PlInst*        getPinInst(PlPin* pin)             { return pin->getInst(); }
inline PlTerm*        getPinTerm(PlPin* pin)             { return pin->getTerm(); }
inline bool           isPinSpecial(PlPin* pin)           { return pin->getIsSpecial(); }
inline bool           isConnectNets(PlPin* pin)          { return pin->getIsConnectNets(); }
inline PlArrayObj*    getPinNetArray(PlPin* pin)         { return pin->getNetArray(); }
inline PlPoint        getPinLoc(PlPin* pin)              { return getTermLoc(getPinTerm(pin)); }
inline PlInt          getPinLocX(PlPin* pin)             { return getPinLoc(pin).getX(); }
inline PlInt          getPinLocY(PlPin* pin)             { return getPinLoc(pin).getY(); }
// instance
inline PlUInt         getNumOfInsts()                    { return getPlTopCell()->getNumOfInsts(); }
inline PlArrayObj*    getInstanceArray()                 { return getPlTopCell()->getInstanceArray(); }
inline PlInst*        getInstance(PlObjId idx)           { return PlObj::addr<PlInst>(idx); }
inline PlCell*        getInstCell(PlInst* inst)          { return inst->getParent(); } 
inline String         getInstName(PlInst* inst)          { return inst->getName(); }
inline PlBox          getInstBox(PlInst* inst)           { return inst->getBox(); }
inline PlBox          getInstHaloBox(PlInst* inst)       { return inst->getHalo(); }
inline PlPoint        getInstLoc(PlInst* inst)           { return inst->getLocation(); }
inline PlOrient       getInstOri(PlInst* inst)           { return inst->getOrient(); }
inline PlStatus       getInstStatus(PlInst* inst)        { return inst->getStatus(); }
inline bool           isInstHasWeight(PlInst* inst)      { return inst->getHasWeight(); }
inline PlInt          getInstWeigth(PlInst* inst)        { return inst->getWeight(); }
inline PlConstraint*  getInstRegion(PlInst* inst)        { return inst->getRegion(); }
inline PlInt          getInstNumPins(PlInst* inst)       { return inst->numPins(); }
inline PlArrayObj*    getInstPinArray(PlInst* inst)      { return inst->getPinArray(); }
inline bool           isInstUnplaced(PlInst* inst)       { return getInstStatus(inst) == kPlStatus::kUnplaced; }
inline bool           isInstPlaced(PlInst* inst)         { return getInstStatus(inst) == kPlStatus::kPlaced; }
inline bool           isInstSuggested(PlInst* inst)      { return getInstStatus(inst) == kPlStatus::kSuggested;}
inline bool           isInstFixed(PlInst* inst)          { return getInstStatus(inst) == kPlStatus::kFixed; }
inline bool           isInstLocked(PlInst* inst)         { return getInstStatus(inst) == kPlStatus::kLocked; }
inline bool           isInstCover(PlInst* inst)          { return getInstStatus(inst) == kPlStatus::kCover; }
inline bool           isInstMoveable(PlInst* inst)       { return (isInstUnplaced(inst) || isInstPlaced(inst) || isInstSuggested(inst)); }
// cell
inline PlUInt         getNumOfCells()                    { return getRoot()->getTechLib()->getNumOfCells(); }
inline PlArrayObj*    getCellArray()                     { return getRoot()->getTechLib()->getCellArray(); }
inline PlCell*        getCell(PlInt idx)                 { return PlObj::addr<PlCell>(idx); }
inline PlUInt         getCellNumOfTerms(PlCell* cell)    { return cell->getNumOfTerms(); }
inline PlArrayObj*    getCellTerms(PlCell* cell)         { return cell->getTermArray(); }
inline String const&  getCellName(PlCell* cell)          { return cell->getName(); }
inline bool           isCellBlock(PlCell* cell)          { return cell->isClassBlock(); }
inline PlLGeometry*   getCellOBSById(PlCell* cell, int idx)   { return cell->getOBS(idx); }
// group
inline PlUInt         getNumOfGroups()                   { return getPlTopCell()->getNumOfGroups(); }
inline PlArrayObj*    getGroupArray()                    { return getPlTopCell()->getGroupArray(); }
inline PlGroup*       getGroup(size_t idx)               { return getPlTopCell()->getGroup(idx); }
inline PlConstraint*  getRegion(PlGroup* group)          { return group->getRegion(); }
inline PlConType      getConType(PlConstraint* con)      { return con->getConstraintType(); }
inline PlConSubType   getSubConType(PlConstraint* con)   { return con->getConstraintSubType(); }
inline bool           isRegionFence(PlConstraint* con)   { return getSubConType(con) == kPlConSubType::kRegionFence; }
inline bool           isRrgionGuide(PlConstraint* con)   { return getSubConType(con) == kPlConSubType::kRegionGuide; }
inline bool           isRegionPlaceBlk(PlConstraint* con)  { return getConType(con) == kPlConType::kConstraintPBlkg; }
inline PlArrayObj*    getRegionBoxArray(PlConstraint* con) { return PlObj::addr<PlArrayObj>(con->getBoxesId()); }
inline PlBox*         getBox(PlObjId idx)                  { return PlObj::addr<PlBox>(idx); }
// floor plan
inline PlFloorplan*   getFloorplan()                     { return getPlTopCell()->getFloorplan(); }
inline PlPolygon*     getCorePolygon()                   { return getFloorplan()->getDieAreaPolygon(); }
inline PlInt          getPolygonNumPoint(PlPolygon* pl)  { return pl->getNumPoints(); } 
inline PlBox          getCoreBox()                       { return getFloorplan()->getCoreBox(); }
inline PlInt          getCoreXOffset()                   { return getFloorplan()->getXOffset(); }
inline PlInt          getCoreYOffset()                   { return getFloorplan()->getYOffset(); }
inline PlUInt         getFPNumOfRow(PlFloorplan* fp)     { return fp->getNumOfRows(); }
inline PlUInt         getNumOfRows()                     { return getFloorplan()->getNumOfRows(); }
inline PlObjId        getRowsId(PlFloorplan* fp)         { return fp->getRows(); }
inline PlArrayObj*    getRowArray()                      { return PlObj::addr<PlArrayObj>(getFloorplan()->getRows()); }
inline PlRow*         getRow(PlObjId idx)                { return PlObj::addr<PlRow>(idx); } // need API
inline PlBox          getRowBox(PlRow* row)              { return row->getBox(); }
// place blockage
inline PlInt          getNumPlaceBlk()                   { return getFloorplan()->getNumOfPlaceBlockages(); }
inline PlObjId        getPlaceBlksId(PlFloorplan* fp)    { return fp->getPlaceBlockages(); }
inline PlArrayObj*    getPlaceBlkArray()                 { return PlObj::addr<PlArrayObj>(getFloorplan()->getPlaceBlockages()); }
inline PlConstraint*  getPlaceBlk(PlObjId idx)           { return PlObj::addr<PlConstraint>(idx); }
// route blockage
inline PlInt          getNumRouteBlk()                   { return getFloorplan()->getNumOfRouteBlockages(); }
inline PlObjId        getRouteBlksId(PlFloorplan* fp)    { return fp->getRouteBlockages(); }
inline PlArrayObj*    getRouteBlkArray()                 { return PlObj::addr<PlArrayObj>(getFloorplan()->getRouteBlockages()); }
inline PlConstraint*  getRouteBlk(PlObjId idx)           { return PlObj::addr<PlConstraint>(idx); }
// Box
inline PlInt          getBoxLLX(PlBox& box)              { return box.getLLX(); }
inline PlInt          getBoxLLY(PlBox& box)              { return box.getLLY(); }
inline PlInt          getBoxURX(PlBox& box)              { return box.getURX(); }
inline PlInt          getBoxURY(PlBox& box)              { return box.getURY(); }
inline PlInt          getBoxHeight(PlBox& box)           { return box.getHeight(); }
inline PlInt          getBoxWidth(PlBox& box)            { return box.getWidth(); }
inline PlInt          getBoxArea(PlBox& box)             { return (box.getHeight() * box.getWidth());}
inline void           setBoxLLX(PlBox& box, PlInt lx)    { box.setLLX(lx); }
inline void           setBoxLLY(PlBox& box, PlInt ly)    { box.setLLY(ly); }
inline void           setBoxURX(PlBox& box, PlInt ux)    { box.setURX(ux); }
inline void           setBoxURY(PlBox& box, PlInt uy)    { box.setURY(uy); }
inline void           setBox(PlBox& box, PlInt lx, PlInt ly, PlInt ux, PlInt uy)
                      { box.setBox(lx, ly, ux, uy); }
inline bool           isBoxInvalid(PlBox& box)           { return box.isInvalid(); }
// db set API
inline void           setInstLoc(PlInst* inst, PlPoint loc)         { inst->setLocation(loc); }
inline void           setInstCell(PlInst* inst, PlCell* cell)       { inst->setParent(cell); } 
inline void           setInstOri(PlInst* inst, PlOrient ori)        { inst->setOrient(ori); }
inline void           setInstPStatus(PlInst* inst, PlStatus status) { inst->setStatus(status); } 

inline bool
getPin1Box(PlPin* pin, PlBox& box)
{
    PlTerm* term = getPinTerm(pin); 
    if (term && isTermHasPort(term)) {
        // get first port
        PlPort* port = getTermPortById(term, 0);
        if (port && isPortHasLGeometry(port)) {
            // get first geometry
            PlLGeometry* lGeo = getPortLGeometryById(port, 0);
            PlGeometry* geo = getGeometry(lGeo, 0);
            box = getGeometryBox(geo);
            return true;
        }
    }
    return false;
}

#define setMin2(a, b) if (a < b) b = a;
#define setMax2(a, b) if (a > b) b = a;
// iterations
#define forEachNets(net)                                                                 \
        for (auto iter = getNetArray()->begin(); iter != getNetArray()->end(); ++iter) { \
          PlObjId netId = *iter;                                                         \
          net = getNet(netId);                                                           \
          if (nullptr == net) continue;
#define endForEachNets }

#define forEachNetPins(net) \
        for (auto iter = getNetPinArray(net)->begin(); iter != getNetPinArray(net)->end(); ++iter) { \
          PlObjId pinId = *iter;                                                                     \
          PlPin* pin = getPin(pinId);                                                                \
          if (nullptr == pin) continue;
#define endForEachNetPins }

#define forEachInsts(inst)                                                                             \
        for (auto iter = getInstanceArray()->begin(); iter != getInstanceArray()->end(); ++iter) { \
          PlObjId instId = *iter;                                                                  \
          inst = getInstance(instId);                                                      \
          if (nullptr == inst) continue;
#define endForEachInsts }

#define forEachInstPins(inst)                                                                            \
        for (auto iter = getInstPinArray(inst)->begin(); iter != getInstPinArray(inst)->end(); ++iter) { \
          PlObjId pinId = *iter;                                                                         \
          PlPin* pin = getPin(pinId);                                                                    \
          if (nullptr == pin) continue;
#define endForEachInstPins }

#define forEachCells(cell)                                                                     \
        for (auto iter = getCellArray()->begin(); iter != getCellArray()->end(); ++iter) { \
          PlObjId cellId = *iter;                                                          \
          cell = getCell(cellId);                                                      \
          if (nullptr == cell) continue;
#define endForEachCells }

#define forEachGruops(group)                                                                      \
        for (auto iter = getGroupArray()->begin(); iter != getGroupArray()->end(); ++iter) { \
          PlObjId groupId = *iter;                                                           \
          group = getGroup(groupId);                                                \
          if (nullptr == group) continue;
#define endForEachGroups }

#define forEachPlaceBlockages(con)                                                               \
        for (auto iter = getGroupArray()->begin(); iter != getGroupArray()->end(); ++iter) { \
          PlGroup* group = getGroup(*iter);                                                  \
          con = getRegion(group);                                              \
          if (nullptr == con || !isRegionPlaceBlk(con)) continue;
#define endForEachPlaceBlockages }

#define forEachRegionBoxs(con)                                                                              \
        for (auto iter = getRegionBoxArray(con)->begin(); iter != getRegionBoxArray(con)->end(); ++iter) { \
          PlObjId boxId = *iter;                                                                           \
          PlBox* box = getBox(boxId);                                                                      \
          if (nullptr == box) continue;
#define endForEachRegionBoxs }

#define forEachIOPins(pin)                                             \
        for (int pinId = 0; pinId < getNumOfIOPins(); ++pinId) {      \
          pin = getIOPin(pinId);                                      \
          if (nullptr == pin) continue;
#define endForEachIOPins }

#define forEachRows(row)                                                                  \
        for (auto iter = getRowArray()->begin(); iter != getRowArray()->end(); ++iter) { \
          PlObjId rowId = *iter;                                                         \
          row = getRow(rowId);                                                           \
          if (nullptr == row) continue;
#define endForEachRows }

#define forEachPlaceBlks(blk)                                                                       \
        for (auto iter = getPlaceBlkArray()->begin(); iter != getPlaceBlkArray()->end(); ++iter) { \
          PlObjId blkId = *iter;                                                                   \
          blk = getPlaceBlk(blkId);                                                                \
          if (nullptr == blk) continue;
#define endForEachPlaceBlks }
                         
#define forEachRouteBlks(blk)                                                                       \
        for (auto iter = getRouteBlkArray()->begin(); iter != getRouteBlkArray()->end(); ++iter) { \
          PlObjId blkId = *iter;                                                                   \
          blk = getRouteBlk(blkId);                                                                \
          if (nullptr == blk) continue;
#define endForEachRouteBlks }

DREAMPLACE_END_NAMESPACE 

#endif
