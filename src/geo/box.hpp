/* @file  box.hpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/**
 * @file   box.hpp
 * @date   Apr 2020
 */

#ifndef EDI_GEO_BOX_HPP_
#define EDI_GEO_BOX_HPP_

#include "geo/point.hpp"

EDI_BEGIN_NAMESPACE

namespace geo {

namespace bg = boost::geometry;

/// @brief A rectangle box.
template <typename CoordinateType,
          typename CoreType = bg::model::box<Point<CoordinateType, 2>>>
class Box : public CoreType {
  public:
    using BaseType = CoreType;
    using PointType = Point<CoordinateType, 2>;
    /// @brief default constructor
    Box() : CoreType() { reset(); }

    /// @brief constructor
    Box(CoordinateType xxl, CoordinateType yyl, CoordinateType xxh,
        CoordinateType yyh)
        : CoreType({xxl, yyl}, {xxh, yyh}) {}

    /// @brief getter for xl
    CoordinateType const& getXL() const { return this->min_corner().x(); }
    /// @brief getter for xl
    CoordinateType const& xl() const { return this->getXL(); }
    /// @brief getter for yl
    CoordinateType const& getYL() const { return this->min_corner().y(); }
    /// @brief getter for yl
    CoordinateType const& yl() const { return this->getYL(); }
    /// @brief getter for xh
    CoordinateType const& getXH() const { return this->max_corner().x(); }
    /// @brief getter for xh
    CoordinateType const& xh() const { return this->getXH(); }
    /// @brief getter for yh
    CoordinateType const& getYH() const { return this->max_corner().y(); }
    /// @brief getter for yh
    CoordinateType const& yh() const { return this->getYH(); }

    /// @brief reset to invalid box
    void reset() {
        this->min_corner().set(std::numeric_limits<CoordinateType>::max(),
                               std::numeric_limits<CoordinateType>::max());
        this->max_corner().set(std::numeric_limits<CoordinateType>::lowest(),
                               std::numeric_limits<CoordinateType>::lowest());
    }

    /// @brief set box
    void set(CoordinateType xxl, CoordinateType yyl, CoordinateType xxh,
             CoordinateType yyh) {
        this->min_corner().set(xxl, yyl);
        this->max_corner().set(xxh, yyh);
    }

    /// @brief summarize memory usage of the object in bytes
    std::size_t memory() const { return sizeof(*this); }

  protected:
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, Box const& rhs) {
        os << DataTypeName(className(rhs)) << DataBegin("(") << rhs.xl()
           << DataDelimiter() << rhs.yl() << DataDelimiter() << rhs.xh()
           << DataDelimiter() << rhs.yh() << DataEnd(")");
        return os;
    }
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, Box& rhs) {
        CoordinateType xl, yl, xh, yh;
        is >> DataTypeName(className(rhs)) >> DataBegin("(") >> xl >>
            DataDelimiter() >> yl >> DataDelimiter() >> xh >> DataDelimiter() >>
            yh >> DataEnd(")");
        rhs.set(xl, yl, xh, yh);
        return is;
    }
};

}  // namespace geo

EDI_END_NAMESPACE

namespace boost {
namespace geometry {
namespace traits {

//////// for box ////////
template <typename CoordinateType, typename CoreType>
struct tag<EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>>
    : public tag<typename EDI_NAMESPACE::geo::Box<CoordinateType,
                                                  CoreType>::BaseType> {};

template <typename CoordinateType, typename CoreType>
struct point_type<EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>> {
    typedef
        typename EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>::PointType
            type;
};

template <typename CoordinateType, typename CoreType>
struct indexed_access<EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>,
                      min_corner, 0>
    : public indexed_access<
          typename EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>::BaseType,
          min_corner, 0> {};

template <typename CoordinateType, typename CoreType>
struct indexed_access<EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>,
                      min_corner, 1>
    : public indexed_access<
          typename EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>::BaseType,
          min_corner, 1> {};

template <typename CoordinateType, typename CoreType>
struct indexed_access<EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>,
                      max_corner, 0>
    : public indexed_access<
          typename EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>::BaseType,
          max_corner, 0> {};

template <typename CoordinateType, typename CoreType>
struct indexed_access<EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>,
                      max_corner, 1>
    : public indexed_access<
          typename EDI_NAMESPACE::geo::Box<CoordinateType, CoreType>::BaseType,
          max_corner, 1> {};

}  // namespace traits
}  // namespace geometry
}  // namespace boost

#endif
