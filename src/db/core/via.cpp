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
    col_ = 0;
    row_ = 0;
    space_x_ = 0;
    space_y_ = 0;
    ref_count_ = 0;
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
    col_ = 0;
    row_ = 0;
    space_x_ = 0;
    space_y_ = 0;
    ref_count_ = 0;
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
 * @brief Get the ViaMaster* object
 *
 * @return ObjectId
 */
ViaMaster* Via::getMaster() const {
    return addr<ViaMaster>(master_);
}

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

/**
 * @brief Get the Is Array object
 *
 * @return Bits
 */
Bits Via::getIsArray() const { return is_array_; }

/**
 * @brief Set the Is Array object
 *
 * @param is_array
 */
void Via::setIsArray(Bits is_array) { is_array_ = is_array; }

/**
 * @brief Set the Col object
 *
 * @param col
 */
void Via::setCol(int col) { col_ = col; }
/**
 * @brief Get the Col object
 *
 * @return int
 */
int Via::getCol() const { return col_; }

/**
 * @brief Set the Row object
 *
 * @param Row
 */
void Via::setRow(int row) { row_ = row; }

/**
 * @brief Get the Row object
 *
 * @return int
 */
int Via::getRow() const { return row_; }

/**
 * @brief Set the Space X object
 *
 * @param space_x
 */
void Via::setSpaceX(int space_x) { space_x_ = space_x; }

/**
 * @brief Get the Space X object
 *
 * @return int
 */
int Via::getSpaceX() const { return space_x_; }

/**
 * @brief Set the Space Y object
 *
 * @param space_y
 */
void Via::setSpaceY(int space_y) { space_y_ = space_y; }

/**
 * @brief Get the Space Y object
 *
 * @return int
 */
int Via::getSpaceY() const { return space_y_; }

/**
 * @brief Get the Reference Count object
 *
 * @return int
 */
int Via::getRefCount() const { return ref_count_; }

/**
 * @brief increase the  Reference Count
 *
 */
void Via::RefCountIncrease() { ref_count_++; }

/**
 * @brief decrease the Reference Count
 *
 */
void Via::RefCountDecrease() { ref_count_--; }

}  // namespace db
}  // namespace open_edi
