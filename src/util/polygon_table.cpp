/** 
 * @file  polygon_table.cpp
 * @date  09/23/2020 05:25:36 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "util/polygon_table.h"
#include <fstream>
#include <iostream>

namespace open_edi {
namespace util {

/// @brief  constructor of Polygon
///
/// @return
Polygon::Polygon() {
}
/// @brief  destructor of Polygon
///
/// @return
Polygon::~Polygon() {
    for (auto pt : pts_) {
        delete pt;
    }
    pts_.clear();
}

void Polygon::addPoint(Point* value) {
    pts_.push_back(value);
}

/// @brief
///
/// @return
void Polygon::writeToFile(IOManager &io_manager, bool debug) {
    //  1. point count
    uint32_t size = getNumPoints();
    io_manager.write((char *) &(size), sizeof(uint32_t));
    if (debug) { std::cout << "RWDBGINFO: points count " << size << std::endl; }

    //  2. write points one by one:
    for (auto pt : pts_) {
        io_manager.write((char *)(pt), sizeof(Point));
        if (debug) { std::cout << "RWDBGINFO: point {" << pt->getX() << " "<< pt->getY() << "}" << std::endl; }
    }
}

/// @brief  readFromFile
///
/// @return
void Polygon::readFromFile(IOManager &io_manager, bool debug) {
    uint32_t num_pts = 0;
    io_manager.read((char *)&(num_pts), sizeof(uint32_t));
    if (debug) { std::cout << "RWDBGINFO: points count " << num_pts << std::endl; }

    for (uint32_t i = 0; i < num_pts; ++i) {
        Point *pt = new Point;
        io_manager.read((char *)pt, sizeof(Point));
        addPoint(pt);
        if (debug) { std::cout << "RWDBGINFO: point {" << pt->getX() << " "<< pt->getY() << "}" << std::endl; }
    }
}

/// @brief  constructor of PolygonTable
///
/// @return
PolygonTable::PolygonTable() {
    polygons_.reserve(10);
}

/// @brief  destructor of PolygonTable
///
/// @return
PolygonTable::~PolygonTable() {
    for (auto &polygon : polygons_) {
        delete polygon;
    }
    polygons_.clear();
}

/// @brief  writeToFile
///
/// @return
void PolygonTable::writeToFile(IOManager &io_manager, bool debug) {
    //  1. polygons count
    uint32_t size = getPolygonCount();
    io_manager.write((char *) &(size), sizeof(uint32_t));
    if (debug) { std::cout << "RWDBGINFO: polygons count " << size << std::endl; }

    //  2. write polygon one by one:
    uint32_t index = 0;
    for (auto &polygon : polygons_) {
        //  io_manager.write((char *) &(index), sizeof(uint32_t));
        polygon->writeToFile(io_manager, debug);
        ++index;
    }
}

/// @brief  readFromFile
///
/// @return
void PolygonTable::readFromFile(IOManager &io_manager, bool debug) {
    //  1. polygons count
    uint32_t size = 0;
    io_manager.read((char *) &(size), sizeof(uint32_t));
    if (debug) { std::cout << "RWDBGINFO: polygons count " << size << std::endl; }

    //  2. read polygon one by one:
    for (int i = 0; i < size; ++i) {
        Polygon *polygon = new Polygon;
        polygon->readFromFile(io_manager, debug);
        addPolygon(polygon);
    }
}

}  // namespace util
}  // namespace open_edi
