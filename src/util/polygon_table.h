/**
 * @file  polygon_table.h
 * @date  Sep 7, 2020
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef SRC_UTIL_POLYGON_TABLE_H_
#define SRC_UTIL_POLYGON_TABLE_H_

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "util/data_traits.h"
#include "util/namespace.h"
#include "util/point.h"

namespace open_edi {
namespace util {

class Polygon {
  public:
    Polygon();
    ~Polygon();

    uint32_t getNumPoints() const { return pts_.size(); }
    void addPoint(Point *value);
    void addPoint(Point value) { pts_.emplace_back(std::move(value)); }
    Point getPoint(int index) const { return pts_[index]; }

    void writeToFile(std::ofstream &outfile, bool debug);
    void readFromFile(std::ifstream &infile, bool debug);

  private:
    std::vector<Point> pts_;
};

class PolygonTable {
  public:
    ObjectIndex addPolygon(Polygon *p) {
        polygons_.push_back(p);
        return polygons_.size() - 1;
    }

    Polygon *getPolygonByIndex(ObjectIndex index) const {
        if (index < 0 || index > polygons_.size()) return nullptr;
        return polygons_.at(index);
    }

    uint32_t getPolygonCount() { return polygons_.size(); }

    PolygonTable();
    ~PolygonTable();

    void writeToFile(std::ofstream &outfile, bool debug);
    void readFromFile(std::ifstream &infile, bool debug);

  private:
    std::vector<Polygon *> polygons_;
};

}  // namespace util
}  // namespace open_edi

#endif  //  SRC_UTIL_POLYGON_TABLE_H_
