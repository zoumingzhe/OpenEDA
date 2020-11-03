/**
 * @file   geometry.h
 * @author Yibo Lin
 * @date   Mar 2020
 */

#ifndef EDI_PYTHON_GEO_H_
#define EDI_PYTHON_GEO_H_

#include "geo/geo.h"
#include "python/util.h"

namespace geo = EDI_NAMESPACE::geo;

using Point2DInt = geo::Point<int32_t, 2>;
using VectorPoint2DInt = std::vector<Point2DInt>;
using Point2DUint = geo::Point<uint32_t, 2>;
using VectorPoint2DUint = std::vector<Point2DUint>;
using Point2DFloat = geo::Point<float, 2>;
using VectorPoint2DFloat = std::vector<Point2DFloat>;
using Point2DDouble = geo::Point<double, 2>;
using VectorPoint2DDouble = std::vector<Point2DDouble>;
using Point3DInt = geo::Point<int32_t, 3>;
using VectorPoint3DInt = std::vector<Point3DInt>;
using Point3DUint = geo::Point<uint32_t, 3>;
using VectorPoint3DUint = std::vector<Point3DUint>;
using Point3DFloat = geo::Point<float, 3>;
using VectorPoint3DFloat = std::vector<Point3DFloat>;
using Point3DDouble = geo::Point<double, 3>;
using VectorPoint3DDouble = std::vector<Point3DDouble>;
using BoxInt = geo::Box<int32_t>;
using VectorBoxInt = std::vector<BoxInt>;
using BoxUint = geo::Box<uint32_t>;
using VectorBoxUint = std::vector<BoxUint>;
using BoxFloat = geo::Box<float>;
using VectorBoxFloat = std::vector<BoxFloat>;
using BoxDouble = geo::Box<double>;
using VectorBoxDouble = std::vector<BoxDouble>;
using Size2DInt = geo::Size<int32_t, 2>;
using VectorSize2DInt = std::vector<Size2DInt>;
using Size2DUint = geo::Size<uint32_t, 2>;
using VectorSize2DUint = std::vector<Size2DUint>;
using Size2DFloat = geo::Size<float, 2>;
using VectorSize2DFloat = std::vector<Size2DFloat>;
using Size2DDouble = geo::Size<double, 2>;
using VectorSize2DDouble = std::vector<Size2DDouble>;
using Size3DInt = geo::Size<int32_t, 3>;
using VectorSize3DInt = std::vector<Size3DInt>;
using Size3DUint = geo::Size<uint32_t, 3>;
using VectorSize3DUint = std::vector<Size3DUint>;
using Size3DFloat = geo::Size<float, 3>;
using VectorSize3DFloat = std::vector<Size3DFloat>;
using Size3DDouble = geo::Size<double, 3>;
using VectorSize3DDouble = std::vector<Size3DDouble>;

#endif
