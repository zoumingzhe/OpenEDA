/* @file  via.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/via.h"

#include "db/core/db.h"
namespace open_edi {
namespace db {
/**
 * @brief Construct a new Via:: Via object
 *
 * @param via_master
 */
Via::Via(ViaMaster* via_master) {
    master_ = via_master->getId();
    is_array_ = 0;
    status_ = 0;
    orientation_ = 0;
}
/**
 * @brief Construct a new Via:: Via object
 *
 */
Via::Via() {
    master_ = 0;
    is_array_ = 0;
    status_ = 0;
    orientation_ = 0;
}

/**
 * @brief Get the Location object
 *
 * @return Point
 */
Point Via::getLoc() const { return loc_; }

/**
 * @brief Set the Location object
 *
 * @param loc
 */
void Via::setLoc(Point loc) { loc_ = loc; }

/**
 * @brief Set the Location object
 *
 * @param loc
 */
void Via::setLoc(int x, int y) {
    loc_.setX(x);
    loc_.setY(y);
}

/**
 * @brief Get the ViaMaster* object
 *
 * @return ObjectId
 */
ViaMaster* Via::getMaster() const { return addr<ViaMaster>(master_); }

/**
 * @brief Set the Master object
 *
 * @param ViaMaster*
 */
void Via::setMaster(ViaMaster* master) { master_ = master->getId(); }

/**
 * @brief Get the Orien object
 *
 * @return Bits
 */
Bits Via::getOrient() const { return orientation_; }

/**
 * @brief Set the Orient object
 *
 * @param orient
 */
void Via::setOrient(Bits orient) { orientation_ = orient; }

/**
 * @brief judge Via orientation status
 *
 * @return true
 * @return false
 */
bool Via::isR0() const { return getOrient() == kR0; }

/**
 * @brief judge Via orientation status
 *
 * @return true
 * @return false
 */
bool Via::isR90() const { return getOrient() == kR90; }

/**
 * @brief judge Via orientation status
 *
 * @return true
 * @return false
 */
bool Via::isR180() const { return getOrient() == kR180; }

/**
 * @brief judge Via orientation status
 *
 * @return true
 * @return false
 */
bool Via::isR270() const { return getOrient() == kR270; }

/**
 * @brief judge Via orientation status
 *
 * @return true
 * @return false
 */
bool Via::isMX() const { return getOrient() == kMX; }

/**
 * @brief judge Via orientation status
 *
 * @return true
 * @return falsel
 */
bool Via::isMXR90() const { return getOrient() == kMXR90; }

/**
 * @brief judge Via orientation status
 *
 * @return true
 * @return false
 */
bool Via::isMY() const { return getOrient() == kMY; }

/**
 * @brief judge Via orientation status
 *
 * @return true
 * @return false
 */
bool Via::isMYR90() const { return getOrient() == kMYR90; }

/**
 * @brief Get the Route Status object
 *
 * @return Bits
 */
Bits Via::getRouteStatus() const { return status_; }

/**
 * @brief Set the Route Status object
 *
 * @param status
 */
void Via::setRouteStatus(Bits status) { status_ = status; }

void Via::printDEF(FILE* fp) {
    Tech* lib = getTopCell()->getTechLib();
    Layer* layer = nullptr;
    ViaMaster* via_master = getMaster();

    std::string layer_name = via_master->getUperLayerIndex();
    if (layer_name.size() == 0) {
        ObjectId layer_v = via_master->getViaLayerVector();
        ArrayObject<ObjectId>* layer_array =
            Object::addr<ArrayObject<ObjectId>>(layer_v);
        ViaLayer* via_layer =
            Object::addr<ViaLayer>((*layer_array)[layer_array->getSize() - 1]);
        if (via_layer) layer_name = via_layer->getName();
    }

    if (via_master)
        fprintf(fp, "    NEW %s (%d %d) %s\n", layer_name.c_str(), loc_.getX(),
                loc_.getY(), via_master->getName().c_str());
}

}  // namespace db
}  // namespace open_edi
