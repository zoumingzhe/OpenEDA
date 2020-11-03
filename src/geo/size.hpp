/* @file  size.hpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_GEO_SIZE_HPP_
#define EDI_GEO_SIZE_HPP_

#include <iostream>
#include <numeric>

#include "util/util.h"

EDI_BEGIN_NAMESPACE

namespace geo {

/// @brief Basic class for multiple dimension sizes
template <typename CoordinateType, std::size_t DimensionCount>
class BaseSize : public std::array<CoordinateType, DimensionCount> {
  public:
    using BaseType = std::array<CoordinateType, DimensionCount>;

    /// @brief default constructor
    BaseSize() : BaseType() {
        this->fill(std::numeric_limits<CoordinateType>::max());
    }

    /// @brief constructor
    template <class... Args>
    BaseSize(CoordinateType const& first, Args... args)
        : BaseType({first, args...}) {}

    /// @brief copy constructor
    BaseSize(BaseSize const& rhs) : BaseType(rhs) {}

    /// @brief move constructor
    BaseSize(BaseSize&& rhs) : BaseType(std::move(rhs)) {}

    /// @brief copy assignment
    BaseSize& operator=(BaseSize const& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(rhs);
        }
        return *this;
    }

    /// @brief copy assignment
    BaseSize& operator=(BaseSize&& rhs) {
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

    /// @brief getter for the product
    CoordinateType product() const {
        return std::accumulate(this->begin(), this->end(), 1,
                               std::multiplies<CoordinateType>());
    }

    /// @brief summarize memory usage of the object in bytes
    std::size_t memory() const { return sizeof(*this); }

  protected:
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, BaseSize const& rhs) {
        os << DataTypeName(className(rhs));
        os << DataBegin("(");
        DataDelimiter delimiter("");
        for (std::size_t i = 0; i < DimensionCount; ++i) {
            os << delimiter << rhs[i];
            delimiter = DataDelimiter();
        }
        os << DataEnd(")");
        return os;
    }
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, BaseSize& rhs) {
        is >> DataTypeName(className(rhs));
        is >> DataBegin("(");
        DataDelimiter delimiter("");
        for (std::size_t i = 0; i < DimensionCount; ++i) {
            is >> delimiter >> rhs[i];
            delimiter = DataDelimiter();
        }
        is >> DataEnd(")");
        return is;
    }
};  // namespace geo

/// @brief Size class for specialization
template <typename CoordinateType, std::size_t DimensionCount>
class Size;

/// @brief Specialization for 2D size
template <typename CoordinateType>
class Size<CoordinateType, 2> : public BaseSize<CoordinateType, 2> {
  public:
    using BaseType = BaseSize<CoordinateType, 2>;

    /// @brief default constructor
    Size() : BaseType() {}
    /// @brief constructor
    Size(CoordinateType const& v0, CoordinateType const& v1)
        : BaseType(v0, v1) {}
    /// @brief copy constructor
    Size(Size const& rhs) : BaseType(rhs) {}
    /// @brief move constructor
    Size(Size&& rhs) : BaseType(std::move(rhs)) {}
    /// @brief copy assignment
    Size& operator=(Size const& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(rhs);
        }
        return *this;
    }
    /// @brief move assignment
    Size& operator=(Size&& rhs) {
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
    CoordinateType const& x() const { return getX(); }
    /// @brief getter for width
    CoordinateType const& width() const { return x(); }
    /// @brief getter for y
    CoordinateType const& getY() const {
        return this->BaseType::template get<1>();
    }
    /// @brief getter for y
    CoordinateType const& y() const { return getY(); }
    /// @brief getter for height
    CoordinateType const& height() const { return y(); }
    /// @brief getter general
    template <std::size_t Dimension>
    CoordinateType const& get() const {
        return this->BaseType::template get<Dimension>();
    }
    /// @brief setter for x
    void setX(CoordinateType const& v) { this->BaseType::template set<0>(v); }
    /// @brief setter for width
    void setWidth(CoordinateType const& v) { setX(v); }
    /// @brief setter for y
    void setY(CoordinateType const& v) { this->BaseType::template set<1>(v); }
    /// @brief setter for height
    void setHeight(CoordinateType const& v) { setY(v); }
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
    friend OStreamBase& operator<<(OStreamBase& os, Size const& rhs) {
        os << DataTypeName(className(rhs)) << DataBegin("(");
        Size::BaseType const& base = rhs;
        os << base;
        os << DataEnd(")");
        return os;
    }
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, Size& rhs) {
        is >> DataTypeName(className(rhs)) >> DataBegin("(");
        Size::BaseType& base = rhs;
        is >> base;
        is >> DataEnd(")");
        return is;
    }
};

/// @brief Specialization for 3D size
template <typename CoordinateType>
class Size<CoordinateType, 3> : public BaseSize<CoordinateType, 3> {
  public:
    using BaseType = BaseSize<CoordinateType, 3>;

    /// @brief default constructor
    Size() : BaseType() {}
    /// @brief constructor
    Size(CoordinateType const& v0, CoordinateType const& v1,
         CoordinateType const& v2)
        : BaseType(v0, v1, v2) {}
    /// @brief copy constructor
    Size(Size const& rhs) : BaseType(rhs) {}
    /// @brief move constructor
    Size(Size&& rhs) : BaseType(std::move(rhs)) {}
    /// @brief copy assignment
    Size& operator=(Size const& rhs) {
        if (this != &rhs) {
            this->BaseType::operator=(rhs);
        }
        return *this;
    }
    /// @brief move assignment
    Size& operator=(Size&& rhs) {
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
    CoordinateType const& x() const { return getX(); }
    /// @brief getter for y
    CoordinateType const& getY() const {
        return this->BaseType::template get<1>();
    }
    /// @brief getter for y
    CoordinateType const& y() const { return getY(); }
    /// @brief getter for z
    CoordinateType const& getZ() const {
        return this->BaseType::template get<2>();
    }
    /// @brief getter for z
    CoordinateType const& z() const { return getZ(); }
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
    friend OStreamBase& operator<<(OStreamBase& os, Size const& rhs) {
        os << DataTypeName(className(rhs)) << DataBegin("(");
        Size::BaseType const& base = rhs;
        os << base;
        os << DataEnd(")");
        return os;
    }
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, Size& rhs) {
        is >> DataTypeName(className(rhs)) >> DataBegin("(");
        Size::BaseType& base = rhs;
        is >> base;
        is >> DataEnd(")");
        return is;
    }
};

}  // namespace geo

EDI_END_NAMESPACE

#endif
