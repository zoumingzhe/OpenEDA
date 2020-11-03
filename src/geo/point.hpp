/* @file  point.hpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_GEO_POINT_HPP_
#define EDI_GEO_POINT_HPP_

#include <boost/geometry.hpp>

#include "util/util.h"

EDI_BEGIN_NAMESPACE

namespace geo {

/// @brief Point in cartesian coordinate system
template <typename CoordinateType, std::size_t DimensionCount>
class BasePoint : public std::array<CoordinateType, DimensionCount> {
  public:
    using BaseType = std::array<CoordinateType, DimensionCount>;

    /// @brief default constructor
    BasePoint() : BaseType() {
        this->fill(std::numeric_limits<CoordinateType>::max());
    }

    /// @brief constructor
    template <class... Args>
    BasePoint(CoordinateType const& first, Args... args)
        : BaseType({first, args...}) {}

    /// @brief copy constructor
    BasePoint(BasePoint const& rhs) : BaseType(rhs) {}

    /// @brief move constructor
    BasePoint(BasePoint&& rhs) : BaseType(std::move(rhs)) {}

    /// @brief copy assignment
    BasePoint& operator=(BasePoint const& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(rhs);
        }
        return *this;
    }

    /// @brief copy assignment
    BasePoint& operator=(BasePoint&& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(std::move(BaseType(rhs)));
        }
        return *this;
    }

    /// @brief getter for data
    template <std::size_t Dimension>
    CoordinateType const& get() const {
        static_assert(Dimension < DimensionCount,
                      "Dimension < DimensionCount failed");
        return (*this)[Dimension];
    }
    /// @brief setter for data
    template <std::size_t Dimension>
    void set(CoordinateType const& v) {
        static_assert(Dimension < DimensionCount,
                      "Dimension < DimensionCount failed");
        (*this)[Dimension] = v;
    }
    /// @brief setter for data
    template <class... Args>
    void set(CoordinateType const& first, Args... args) {
        *this = {first, args...};
    }

    /// @brief summarize memory usage of the object in bytes
    std::size_t memory() const { return sizeof(*this); }

  protected:
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, BasePoint const& rhs) {
        os << DataTypeName(className(rhs));
        os << DataBegin("(");
        auto delimiter = DataDelimiter("");
        for (std::size_t i = 0; i < DimensionCount; ++i) {
            os << delimiter << rhs[i];
            delimiter = DataDelimiter();
        }
        os << DataEnd(")");
        return os;
    }
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, BasePoint& rhs) {
        is >> DataTypeName(className(rhs));
        is >> DataBegin("(");
        auto delimiter = DataDelimiter("");
        for (std::size_t i = 0; i < DimensionCount; ++i) {
            is >> delimiter >> rhs[i];
            delimiter = DataDelimiter();
        }
        is >> DataEnd(")");
        return is;
    }
};

/// @brief Point class for specialization
template <typename CoordinateType, std::size_t DimensionCount>
class Point;

/// @brief Specialization for 2D point
template <typename CoordinateType>
class Point<CoordinateType, 2> : public BasePoint<CoordinateType, 2> {
  public:
    using BaseType = BasePoint<CoordinateType, 2>;

    /// @brief default constructor
    Point() : BaseType() {}
    /// @brief constructor
    Point(CoordinateType const& v0, CoordinateType const& v1)
        : BaseType(v0, v1) {}
    /// @brief copy constructor
    Point(Point const& rhs) : BaseType(rhs) {}
    /// @brief move constructor
    Point(Point&& rhs) : BaseType(std::move(rhs)) {}
    /// @brief copy assignment
    Point& operator=(Point const& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(rhs);
        }
        return *this;
    }
    /// @brief move assignment
    Point& operator=(Point&& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(std::move(rhs));
        }
        return *this;
    }

    /// @brief getter for x
    CoordinateType const& getX() const {
        return this->BaseType::template get<0>();
    }
    /// @brief getter for x
    CoordinateType const& x() const { return this->getX(); }
    /// @brief getter for y
    CoordinateType const& getY() const {
        return this->BaseType::template get<1>();
    }
    /// @brief getter for y
    CoordinateType const& y() const { return this->getY(); }
    /// @brief getter general
    template <std::size_t Dimension>
    CoordinateType const& get() const {
        return this->BaseType::template get<Dimension>();
    }
    /// @brief setter for x
    void setX(CoordinateType const& v) { this->BaseType::template set<0>(v); }
    /// @brief setter for y
    void setY(CoordinateType const& v) { this->BaseType::template set<1>(v); }
    /// @brief setter general
    template <std::size_t Dimension>
    void set(CoordinateType const& v) {
        this->BaseType::template set<Dimension>(v);
    }
    /// @brief setter for x and y
    void set(CoordinateType const& v0, CoordinateType const& v1) {
        this->BaseType::template set(v0, v1);
    }

    /// @brief summarize memory usage of the object in bytes
    std::size_t memory() const { return this->BaseType::memory(); }

  protected:
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, Point const& rhs) {
        os << DataTypeName(className(rhs)) << DataBegin("(");
        Point::BaseType const& base = rhs;
        os << base;
        os << DataEnd(")");
        return os;
    }
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, Point& rhs) {
        is >> DataTypeName(className(rhs)) >> DataBegin("(");
        Point::BaseType& base = rhs;
        is >> base;
        is >> DataEnd(")");
        return is;
    }
};

/// @brief Specialization for 3D point
template <typename CoordinateType>
class Point<CoordinateType, 3> : public BasePoint<CoordinateType, 3> {
  public:
    using BaseType = BasePoint<CoordinateType, 3>;

    /// @brief default constructor
    Point() : BaseType() {}
    /// @brief constructor
    Point(CoordinateType const& v0, CoordinateType const& v1,
          CoordinateType const& v2)
        : BaseType(v0, v1, v2) {}
    /// @brief copy constructor
    Point(Point const& rhs) : BaseType(rhs) {}
    /// @brief move constructor
    Point(Point&& rhs) : BaseType(std::move(rhs)) {}
    /// @brief copy assignment
    Point& operator=(Point const& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(rhs);
        }
        return *this;
    }
    /// @brief move assignment
    Point& operator=(Point&& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(std::move(rhs));
        }
        return *this;
    }

    /// @brief getter for x
    CoordinateType const& getX() const {
        return this->BaseType::template get<0>();
    }
    /// @brief getter for x
    CoordinateType const& x() const { return this->getX(); }
    /// @brief getter for y
    CoordinateType const& getY() const {
        return this->BaseType::template get<1>();
    }
    /// @brief getter for y
    CoordinateType const& y() const { return this->getY(); }
    /// @brief getter for z
    CoordinateType const& getZ() const {
        return this->BaseType::template get<2>();
    }
    /// @brief getter for z
    CoordinateType const& z() const { return this->getZ(); }
    /// @brief getter general
    template <std::size_t Dimension>
    CoordinateType const& get() const {
        return this->BaseType::template get<Dimension>();
    }
    /// @brief setter for x
    void setX(CoordinateType const& v) { this->BaseType::template set<0>(v); }
    /// @brief setter for y
    void setY(CoordinateType const& v) { this->BaseType::template set<1>(v); }
    /// @brief setter for z
    void setZ(CoordinateType const& v) { this->BaseType::template set<2>(v); }
    /// @brief setter general
    template <std::size_t Dimension>
    void set(CoordinateType const& v) {
        this->BaseType::template set<Dimension>(v);
    }
    /// @brief setter for x, y, z
    void set(CoordinateType const& v0, CoordinateType const& v1,
             CoordinateType const& v2) {
        this->BaseType::template set(v0, v1, v2);
    }

    /// @brief summarize memory usage of the object in bytes
    std::size_t memory() const { return this->BaseType::memory(); }

  protected:
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, Point const& rhs) {
        os << DataTypeName(className(rhs)) << DataBegin("(");
        Point::BaseType const& base = rhs;
        os << base;
        os << DataEnd(")");
        return os;
    }
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, Point& rhs) {
        is >> DataTypeName(className(rhs)) >> DataBegin("(");
        Point::BaseType& base = rhs;
        is >> base;
        is >> DataEnd(")");
        return is;
    }
};  // namespace geo

}  // namespace geo

EDI_END_NAMESPACE

namespace boost {
namespace geometry {
namespace traits {

//////// for point ////////
template <typename CoordinateType, std::size_t DimensionCount>
struct tag<EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount>> {
    typedef point_tag type;
};

template <typename CoordinateType, std::size_t DimensionCount>
struct point_type<EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount>> {
    typedef typename EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount>
        type;
};

template <typename CoordinateType, std::size_t DimensionCount>
struct coordinate_type<
    EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount>> {
    typedef CoordinateType type;
};

template <typename CoordinateType, std::size_t DimensionCount>
struct coordinate_system<
    EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount>> {
    typedef boost::geometry::cs::cartesian type;
};

template <typename CoordinateType, std::size_t DimensionCount>
struct dimension<EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount>>
    : boost::mpl::int_<DimensionCount> {};

template <typename CoordinateType, std::size_t DimensionCount,
          std::size_t Dimension>
struct access<EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount>,
              Dimension> {
    static inline CoordinateType get(
        EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount> const& p) {
        return p.template get<Dimension>();
    }

    static inline void set(
        EDI_NAMESPACE::geo::Point<CoordinateType, DimensionCount>& p,
        CoordinateType const& value) {
        p.template set<Dimension>(value);
    }
};

}  // namespace traits
}  // namespace geometry
}  // namespace boost

#endif
