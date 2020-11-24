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
typedef dbi::Constraint::ConstraintSubType PlConType;
typedef uti::PlaceStatus kPlStatus;
typedef dbi::Constraint::ConstraintSubType kPlConType;
// interface to edi DB 
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
inline PlPort*        getTermPort(PlTerm* term, int idx) { return term->getPort(idx); }
inline PlPoint        getPortLoc(PlPort* port)           { return port->getLocation(); }
inline PlPoint        getTermLoc(PlTerm* term)           { return getPortLoc(getTermPort(term, 0)); }
// pin
inline PlUInt         getNumOfIOPins()                   { return getPlTopCell()->getNumOfIOPins(); }
inline PlArrayObj*    getNetPinArray(PlNet* net)         { return net->getPinArray(); }
inline PlPin*         getPin(PlObjId idx)                { return PlObj::addr<PlPin>(idx); } // need API
inline PlNet*         getPinNet(PlPin* pin)              { return pin->getNet(); }
inline String const&  getPinName(PlPin* pin)             { return pin->getName(); }
inline PlInst*        getPinInst(PlPin* pin)             { return pin->getInst(); }
inline PlTerm*        getPinTerm(PlPin* pin)             { return pin->getTerm(); }
inline bool           isPinSpecial(PlPin* pin)           { return pin->getHasSpecial(); }
inline bool           isConnectNets(PlPin* pin)          { return pin->getIsConnectNets(); }
inline PlArrayObj*    getPinNetArray(PlPin* net)         { return net->getNetArray(); }
inline PlPoint        getPinLoc(PlPin* pin)              { return getTermLoc(getPinTerm(pin)); }
inline PlInt          getPinLocX(PlPin* pin)             { return getPinLoc(pin).getX(); }
inline PlInt          getPinLocY(PlPin* pin)             { return getPinLoc(pin).getY(); }
// instance
inline PlUInt         getNumOfInsts()                    { return getPlTopCell()->getNumOfInsts(); }
inline PlArrayObj*    getInstanceArray()                 { return getPlTopCell()->getInstanceArray(); }
inline PlInst*        getInstance(PlObjId idx)           { return PlObj::addr<PlInst>(idx); }
inline PlCell*        getInstCell(PlInst* inst)          { return inst->getMaster(); } 
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
inline PlUInt         getNumOfCells()                    { return getPlTopCell()->getNumOfCells(); }
inline PlArrayObj*    getCellArray()                     { return getPlTopCell()->getCellArray(); }
inline PlCell*        getCell(PlInt idx)                 { return getPlTopCell()->getCell(idx); }
inline PlUInt         getCellNumOfTerms(PlCell* cell)    { return cell->getNumOfTerms(); }
inline PlArrayObj*    getCellTerms(PlCell* cell)         { return cell->getTermArray(); }
inline String const&  getCellName(PlCell* cell)          { return cell->getName(); }
// group
inline PlUInt         getNumOfGroups()                   { return getPlTopCell()->getNumOfGroups(); }
inline PlArrayObj*    getGroupArray()                    { return getPlTopCell()->getGroupArray(); }
inline PlGroup*       getGroup(size_t idx)               { return getPlTopCell()->getGroup(idx); }
inline PlConstraint*  getRegion(PlGroup* group)          { return group->getRegion(); }
inline PlConType      getConType(PlConstraint* con)      { return con->getConstraintSubType(); }
inline bool           isRegionFence(PlConstraint* con)   { return getConType(con) == kPlConType::kRegionFence; }
inline bool           isRrgionGuide(PlConstraint* con)   { return getConType(con) == kPlConType::kRegionGuide; }
inline PlArrayObj*    getRegionBoxArray(PlConstraint* con) { return PlObj::addr<PlArrayObj>(con->getBoxesId()); }
inline PlBox*         getBox(PlObjId idx)                  { return PlObj::addr<PlBox>(idx); }
// floor plan
inline PlFloorplan*   getFloorplan()                     { return getPlTopCell()->getFloorplan(); }
inline PlBox          getCoreBox()                       { return getFloorplan()->getCoreBox(); }
inline PlSite*        getCoreSite()                      { return getFloorplan()->getCoreSite(); }
inline PlInt          getCoreXOffset()                   { return getFloorplan()->getXOffset(); }
inline PlInt          getCoreYOffset()                   { return getFloorplan()->getYOffset(); }
// tech
inline PlTech*        getTechLib()                       { return getPlTopCell()->getTechLib(); }
inline PlDouble       dbuToMicrons(PlInt i)              { return getTechLib()->dbuToMicrons(i); }
// Box
inline PlInt          getBoxLLX(PlBox& box)              { return box.getLLX(); }
inline PlInt          getBoxLLY(PlBox& box)              { return box.getLLY(); }
inline PlInt          getBoxURX(PlBox& box)              { return box.getURX(); }
inline PlInt          getBoxURY(PlBox& box)              { return box.getURY(); }
inline PlInt          getBoxHeight(PlBox& box)           { return box.getHeight(); }
inline PlInt          getBoxWidth(PlBox& box)            { return box.getWidth(); }

// iterations
#define forEachNet()                                                                     \
        for (auto iter = getNetArray()->begin(); iter != getNetArray()->end(); ++iter) { \
          PlObjId netId = *iter;                                                         \
          PlNet* net = getNet(netId); 
#define endForEachNet }

#define forEachNetPin(net) \
        for (auto iter = getNetPinArray(net)->begin(); iter != getNetPinArray(net)->end(); ++iter) { \
          PlObjId pinId = *iter;                                                                     \
          PlPin* pin = getPin(pinId); 
#define endForEachNetPin }

#define forEachInst()                                                                              \
        for (auto iter = getInstanceArray()->begin(); iter != getInstanceArray()->end(); ++iter) { \
          PlObjId instId = *iter;                                                                  \
          PlInst* inst = getInstance(instId); 
#define endForEachInst }

#define forEachInstPin(inst)                                                                             \
        for (auto iter = getInstPinArray(inst)->begin(); iter != getInstPinArray(inst)->end(); ++iter) { \
          PlObjId pinId = *iter;                                                                         \
          PlPin* pin = getPin(pinId); 
#define endForEachInstPin }

#define forEachCell()                                                                      \
        for (auto iter = getCellArray()->begin(); iter != getCellArray()->end(); ++iter) { \
          PlObjId cellId = *iter;                                                          \
          PlCell* cell = getCell(cellId); 
#define endForEachCell }

#define forEachGruop()                                                                       \
        for (auto iter = getGroupArray()->begin(); iter != getGroupArray()->end(); ++iter) { \
          PlObjId groupId = *iter;                                                           \
          PlGroup* group = getGroup(groupId);   
#define endForEachGroup }

#define forEachRegionBox(con)                                \
        for (auto iter = getRegionBoxArray(con)->begin(); iter != getRegionBoxArray(con)->end(); ++iter) { \
          PlObjId boxId = *iter;                                                          \
          PlBox* box = getBox(boxId);
#define endForEachRegionBox }
                         
DREAMPLACE_END_NAMESPACE 

#endif
