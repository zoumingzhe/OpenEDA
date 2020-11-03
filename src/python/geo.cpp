/**
 * @file   geometry.cpp
 * @author Yibo Lin
 * @date   Mar 2020
 */

#include "python/geo.h"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(Point2DInt);
PYBIND11_MAKE_OPAQUE(VectorPoint2DInt);

PYBIND11_MAKE_OPAQUE(Point2DUint);
PYBIND11_MAKE_OPAQUE(VectorPoint2DUint);

PYBIND11_MAKE_OPAQUE(Point2DFloat);
PYBIND11_MAKE_OPAQUE(VectorPoint2DFloat);

PYBIND11_MAKE_OPAQUE(Point2DDouble);
PYBIND11_MAKE_OPAQUE(VectorPoint2DDouble);

PYBIND11_MAKE_OPAQUE(Point3DInt);
PYBIND11_MAKE_OPAQUE(VectorPoint3DInt);

PYBIND11_MAKE_OPAQUE(Point3DUint);
PYBIND11_MAKE_OPAQUE(VectorPoint3DUint);

PYBIND11_MAKE_OPAQUE(Point3DFloat);
PYBIND11_MAKE_OPAQUE(VectorPoint3DFloat);

PYBIND11_MAKE_OPAQUE(Point3DDouble);
PYBIND11_MAKE_OPAQUE(VectorPoint3DDouble);

PYBIND11_MAKE_OPAQUE(BoxInt);
PYBIND11_MAKE_OPAQUE(VectorBoxInt);

PYBIND11_MAKE_OPAQUE(BoxUint);
PYBIND11_MAKE_OPAQUE(VectorBoxUint);

PYBIND11_MAKE_OPAQUE(BoxFloat);
PYBIND11_MAKE_OPAQUE(VectorBoxFloat);

PYBIND11_MAKE_OPAQUE(BoxDouble);
PYBIND11_MAKE_OPAQUE(VectorBoxDouble);

PYBIND11_MAKE_OPAQUE(Size2DInt);
PYBIND11_MAKE_OPAQUE(VectorSize2DInt);

PYBIND11_MAKE_OPAQUE(Size2DUint);
PYBIND11_MAKE_OPAQUE(VectorSize2DUint);

PYBIND11_MAKE_OPAQUE(Size2DFloat);
PYBIND11_MAKE_OPAQUE(VectorSize2DFloat);

PYBIND11_MAKE_OPAQUE(Size2DDouble);
PYBIND11_MAKE_OPAQUE(VectorSize2DDouble);

PYBIND11_MAKE_OPAQUE(Size3DInt);
PYBIND11_MAKE_OPAQUE(VectorSize3DInt);

PYBIND11_MAKE_OPAQUE(Size3DUint);
PYBIND11_MAKE_OPAQUE(VectorSize3DUint);

PYBIND11_MAKE_OPAQUE(Size3DFloat);
PYBIND11_MAKE_OPAQUE(VectorSize3DFloat);

PYBIND11_MAKE_OPAQUE(Size3DDouble);
PYBIND11_MAKE_OPAQUE(VectorSize3DDouble);

void bind_geo(py::module& m) {
  m.doc() = R"pbdoc(
        Pybind11 plugin
        -----------------------
        .. currentmodule:: geo
        .. autosummary::
           :toctree: _generate
           Point
           Box
    )pbdoc";

  py::class_<Point2DInt>(m, "Point2DInt")
      .def(py::init<>())
      .def(py::init<int32_t const&, int32_t const&>())
      .def("getX", &Point2DInt::getX)
      .def("getY", &Point2DInt::getY)
      .def("x", &Point2DInt::x)
      .def("y", &Point2DInt::y)
      .def("setX", (void (Point2DInt::*)(int32_t const&)) & Point2DInt::setX)
      .def("setY", (void (Point2DInt::*)(int32_t const&)) & Point2DInt::setY)
      .def("set", (void (Point2DInt::*)(int32_t const&, int32_t const&)) &
                      Point2DInt::set)
      .def("__copy__", [](Point2DInt const& rhs) { return Point2DInt(rhs); })
      .def("__repr__", [](Point2DInt const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Point2DUint>(m, "Point2DUint")
      .def(py::init<>())
      .def(py::init<uint32_t const&, uint32_t const&>())
      .def("getX", &Point2DUint::getX)
      .def("getY", &Point2DUint::getY)
      .def("x", &Point2DUint::x)
      .def("y", &Point2DUint::y)
      .def("setX", (void (Point2DUint::*)(uint32_t const&)) & Point2DUint::setX)
      .def("setY", (void (Point2DUint::*)(uint32_t const&)) & Point2DUint::setY)
      .def("set", (void (Point2DUint::*)(uint32_t const&, uint32_t const&)) &
                      Point2DUint::set)
      .def("__copy__", [](Point2DUint const& rhs) { return Point2DUint(rhs); })
      .def("__repr__", [](Point2DUint const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Point2DFloat>(m, "Point2DFloat")
      .def(py::init<>())
      .def(py::init<float const&, float const&>())
      .def("getX", &Point2DFloat::getX)
      .def("getY", &Point2DFloat::getY)
      .def("x", &Point2DFloat::x)
      .def("y", &Point2DFloat::y)
      .def("setX", (void (Point2DFloat::*)(float const&)) & Point2DFloat::setX)
      .def("setY", (void (Point2DFloat::*)(float const&)) & Point2DFloat::setY)
      .def("set", (void (Point2DFloat::*)(float const&, float const&)) &
                      Point2DFloat::set)
      .def("__copy__",
           [](Point2DFloat const& rhs) { return Point2DFloat(rhs); })
      .def("__repr__", [](Point2DFloat const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Point2DDouble>(m, "Point2DDouble")
      .def(py::init<>())
      .def(py::init<double const&, double const&>())
      .def("getX", &Point2DDouble::getX)
      .def("getY", &Point2DDouble::getY)
      .def("x", &Point2DDouble::x)
      .def("y", &Point2DDouble::y)
      .def("setX",
           (void (Point2DDouble::*)(double const&)) & Point2DDouble::setX)
      .def("setY",
           (void (Point2DDouble::*)(double const&)) & Point2DDouble::setY)
      .def("set", (void (Point2DDouble::*)(double const&, double const&)) &
                      Point2DDouble::set)
      .def("__copy__",
           [](Point2DDouble const& rhs) { return Point2DDouble(rhs); })
      .def("__repr__", [](Point2DDouble const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Point3DInt>(m, "Point3DInt")
      .def(py::init<>())
      .def(py::init<int32_t const&, int32_t const&, int32_t const&>())
      .def("getX", &Point3DInt::getX)
      .def("getY", &Point3DInt::getY)
      .def("getZ", &Point3DInt::getZ)
      .def("x", &Point3DInt::x)
      .def("y", &Point3DInt::y)
      .def("z", &Point3DInt::z)
      .def("setX", (void (Point3DInt::*)(int32_t const&)) & Point3DInt::setX)
      .def("setY", (void (Point3DInt::*)(int32_t const&)) & Point3DInt::setY)
      .def("setZ", (void (Point3DInt::*)(int32_t const&)) & Point3DInt::setZ)
      .def("set", (void (Point3DInt::*)(int32_t const&, int32_t const&,
                                        int32_t const&)) &
                      Point3DInt::set)
      .def("__copy__", [](Point3DInt const& rhs) { return Point3DInt(rhs); })
      .def("__repr__", [](Point3DInt const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Point3DUint>(m, "Point3DUint")
      .def(py::init<>())
      .def(py::init<uint32_t const&, uint32_t const&, uint32_t const&>())
      .def("getX", &Point3DUint::getX)
      .def("getY", &Point3DUint::getY)
      .def("getZ", &Point3DUint::getZ)
      .def("x", &Point3DUint::x)
      .def("y", &Point3DUint::y)
      .def("z", &Point3DUint::z)
      .def("setX", (void (Point3DUint::*)(uint32_t const&)) & Point3DUint::setX)
      .def("setY", (void (Point3DUint::*)(uint32_t const&)) & Point3DUint::setY)
      .def("setZ", (void (Point3DUint::*)(uint32_t const&)) & Point3DUint::setZ)
      .def("set", (void (Point3DUint::*)(uint32_t const&, uint32_t const&,
                                         uint32_t const&)) &
                      Point3DUint::set)
      .def("__copy__", [](Point3DUint const& rhs) { return Point3DUint(rhs); })
      .def("__repr__", [](Point3DUint const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Point3DFloat>(m, "Point3DFloat")
      .def(py::init<>())
      .def(py::init<float const&, float const&, float const&>())
      .def("getX", &Point3DFloat::getX)
      .def("getY", &Point3DFloat::getY)
      .def("getZ", &Point3DFloat::getZ)
      .def("x", &Point3DFloat::x)
      .def("y", &Point3DFloat::y)
      .def("z", &Point3DFloat::z)
      .def("setX", (void (Point3DFloat::*)(float const&)) & Point3DFloat::setX)
      .def("setY", (void (Point3DFloat::*)(float const&)) & Point3DFloat::setY)
      .def("setZ", (void (Point3DFloat::*)(float const&)) & Point3DFloat::setZ)
      .def("set",
           (void (Point3DFloat::*)(float const&, float const&, float const&)) &
               Point3DFloat::set)
      .def("__copy__",
           [](Point3DFloat const& rhs) { return Point3DFloat(rhs); })
      .def("__repr__", [](Point3DFloat const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Point3DDouble>(m, "Point3DDouble")
      .def(py::init<>())
      .def(py::init<double const&, double const&, double const&>())
      .def("getX", &Point3DDouble::getX)
      .def("getY", &Point3DDouble::getY)
      .def("getZ", &Point3DDouble::getZ)
      .def("x", &Point3DDouble::x)
      .def("y", &Point3DDouble::y)
      .def("z", &Point3DDouble::z)
      .def("setX",
           (void (Point3DDouble::*)(double const&)) & Point3DDouble::setX)
      .def("setY",
           (void (Point3DDouble::*)(double const&)) & Point3DDouble::setY)
      .def("setZ",
           (void (Point3DDouble::*)(double const&)) & Point3DDouble::setZ)
      .def("set", (void (Point3DDouble::*)(double const&, double const&,
                                           double const&)) &
                      Point3DDouble::set)
      .def("__copy__",
           [](Point3DDouble const& rhs) { return Point3DDouble(rhs); })
      .def("__repr__", [](Point3DDouble const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<BoxInt>(m, "BoxInt")
      .def(py::init<>())
      .def(py::init<int32_t, int32_t, int32_t, int32_t>())
      .def("getXL", &BoxInt::getXL)
      .def("getYL", &BoxInt::getYL)
      .def("getXH", &BoxInt::getXH)
      .def("getYH", &BoxInt::getYH)
      .def("xl", &BoxInt::xl)
      .def("yl", &BoxInt::yl)
      .def("xh", &BoxInt::xh)
      .def("yh", &BoxInt::yh)
      .def("set",
           (void (BoxInt::*)(int32_t, int32_t, int32_t, int32_t)) & BoxInt::set)
      .def("__copy__", [](BoxInt const& rhs) { return BoxInt(rhs); })
      .def("__repr__", [](BoxInt const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<BoxUint>(m, "BoxUint")
      .def(py::init<>())
      .def(py::init<uint32_t, uint32_t, uint32_t, uint32_t>())
      .def("getXL", &BoxUint::getXL)
      .def("getYL", &BoxUint::getYL)
      .def("getXH", &BoxUint::getXH)
      .def("getYH", &BoxUint::getYH)
      .def("xl", &BoxUint::xl)
      .def("yl", &BoxUint::yl)
      .def("xh", &BoxUint::xh)
      .def("yh", &BoxUint::yh)
      .def("set", (void (BoxUint::*)(uint32_t, uint32_t, uint32_t, uint32_t)) &
                      BoxUint::set)
      .def("__copy__", [](BoxUint const& rhs) { return BoxUint(rhs); })
      .def("__repr__", [](BoxUint const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<BoxFloat>(m, "BoxFloat")
      .def(py::init<>())
      .def(py::init<float, float, float, float>())
      .def("getXL", &BoxFloat::getXL)
      .def("getYL", &BoxFloat::getYL)
      .def("getXH", &BoxFloat::getXH)
      .def("getYH", &BoxFloat::getYH)
      .def("xl", &BoxFloat::xl)
      .def("yl", &BoxFloat::yl)
      .def("xh", &BoxFloat::xh)
      .def("yh", &BoxFloat::yh)
      .def("set",
           (void (BoxFloat::*)(float, float, float, float)) & BoxFloat::set)
      .def("__copy__", [](BoxFloat const& rhs) { return BoxFloat(rhs); })
      .def("__repr__", [](BoxFloat const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<BoxDouble>(m, "BoxDouble")
      .def(py::init<>())
      .def(py::init<double, double, double, double>())
      .def("getXL", &BoxDouble::getXL)
      .def("getYL", &BoxDouble::getYL)
      .def("getXH", &BoxDouble::getXH)
      .def("getYH", &BoxDouble::getYH)
      .def("xl", &BoxDouble::xl)
      .def("yl", &BoxDouble::yl)
      .def("xh", &BoxDouble::xh)
      .def("yh", &BoxDouble::yh)
      .def("set", (void (BoxDouble::*)(double, double, double, double)) &
                      BoxDouble::set)
      .def("__copy__", [](BoxDouble const& rhs) { return BoxDouble(rhs); })
      .def("__repr__", [](BoxDouble const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Size2DInt>(m, "Size2DInt")
      .def(py::init<>())
      .def(py::init<int32_t const&, int32_t const&>())
      .def("getX", &Size2DInt::getX)
      .def("getY", &Size2DInt::getY)
      .def("x", &Size2DInt::x)
      .def("y", &Size2DInt::y)
      .def("width", &Size2DInt::width)
      .def("height", &Size2DInt::height)
      .def("setX", (void (Size2DInt::*)(int32_t const&)) & Size2DInt::setX)
      .def("setWidth",
           (void (Size2DInt::*)(int32_t const&)) & Size2DInt::setWidth)
      .def("setY", (void (Size2DInt::*)(int32_t const&)) & Size2DInt::setY)
      .def("setHeight",
           (void (Size2DInt::*)(int32_t const&)) & Size2DInt::setHeight)
      .def("set", (void (Size2DInt::*)(int32_t const&, int32_t const&)) &
                      Size2DInt::set)
      .def("__copy__", [](Size2DInt const& rhs) { return Size2DInt(rhs); })
      .def("__repr__", [](Size2DInt const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Size2DUint>(m, "Size2DUint")
      .def(py::init<>())
      .def(py::init<uint32_t const&, uint32_t const&>())
      .def("getX", &Size2DUint::getX)
      .def("getY", &Size2DUint::getY)
      .def("x", &Size2DUint::x)
      .def("y", &Size2DUint::y)
      .def("width", &Size2DUint::width)
      .def("height", &Size2DUint::height)
      .def("setX", (void (Size2DUint::*)(uint32_t const&)) & Size2DUint::setX)
      .def("setWidth",
           (void (Size2DUint::*)(uint32_t const&)) & Size2DUint::setWidth)
      .def("setY", (void (Size2DUint::*)(uint32_t const&)) & Size2DUint::setY)
      .def("setHeight",
           (void (Size2DUint::*)(uint32_t const&)) & Size2DUint::setHeight)
      .def("set", (void (Size2DUint::*)(uint32_t const&, uint32_t const&)) &
                      Size2DUint::set)
      .def("__copy__", [](Size2DUint const& rhs) { return Size2DUint(rhs); })
      .def("__repr__", [](Size2DUint const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Size2DFloat>(m, "Size2DFloat")
      .def(py::init<>())
      .def(py::init<float const&, float const&>())
      .def("getX", &Size2DFloat::getX)
      .def("getY", &Size2DFloat::getY)
      .def("x", &Size2DFloat::x)
      .def("y", &Size2DFloat::y)
      .def("width", &Size2DFloat::width)
      .def("height", &Size2DFloat::height)
      .def("setX", (void (Size2DFloat::*)(float const&)) & Size2DFloat::setX)
      .def("setWidth",
           (void (Size2DFloat::*)(float const&)) & Size2DFloat::setWidth)
      .def("setY", (void (Size2DFloat::*)(float const&)) & Size2DFloat::setY)
      .def("setHeight",
           (void (Size2DFloat::*)(float const&)) & Size2DFloat::setHeight)
      .def("set", (void (Size2DFloat::*)(float const&, float const&)) &
                      Size2DFloat::set)
      .def("__copy__", [](Size2DFloat const& rhs) { return Size2DFloat(rhs); })
      .def("__repr__", [](Size2DFloat const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Size2DDouble>(m, "Size2DDouble")
      .def(py::init<>())
      .def(py::init<double const&, double const&>())
      .def("getX", &Size2DDouble::getX)
      .def("getY", &Size2DDouble::getY)
      .def("x", &Size2DDouble::x)
      .def("y", &Size2DDouble::y)
      .def("width", &Size2DDouble::width)
      .def("height", &Size2DDouble::height)
      .def("setX", (void (Size2DDouble::*)(double const&)) & Size2DDouble::setX)
      .def("setWidth",
           (void (Size2DDouble::*)(double const&)) & Size2DDouble::setWidth)
      .def("setY", (void (Size2DDouble::*)(double const&)) & Size2DDouble::setY)
      .def("setHeight",
           (void (Size2DDouble::*)(double const&)) & Size2DDouble::setHeight)
      .def("set", (void (Size2DDouble::*)(double const&, double const&)) &
                      Size2DDouble::set)
      .def("__copy__",
           [](Size2DDouble const& rhs) { return Size2DDouble(rhs); })
      .def("__repr__", [](Size2DDouble const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Size3DInt>(m, "Size3DInt")
      .def(py::init<>())
      .def(py::init<int32_t const&, int32_t const&, int32_t const&>())
      .def("getX", &Size3DInt::getX)
      .def("getY", &Size3DInt::getY)
      .def("getZ", &Size3DInt::getZ)
      .def("x", &Size3DInt::x)
      .def("y", &Size3DInt::y)
      .def("z", &Size3DInt::z)
      .def("setX", (void (Size3DInt::*)(int32_t const&)) & Size3DInt::setX)
      .def("setY", (void (Size3DInt::*)(int32_t const&)) & Size3DInt::setY)
      .def("setZ", (void (Size3DInt::*)(int32_t const&)) & Size3DInt::setZ)
      .def("set", (void (Size3DInt::*)(int32_t const&, int32_t const&,
                                       int32_t const&)) &
                      Size3DInt::set)
      .def("__copy__", [](Size3DInt const& rhs) { return Size3DInt(rhs); })
      .def("__repr__", [](Size3DInt const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Size3DUint>(m, "Size3DUint")
      .def(py::init<>())
      .def(py::init<uint32_t const&, uint32_t const&, uint32_t const&>())
      .def("getX", &Size3DUint::getX)
      .def("getY", &Size3DUint::getY)
      .def("getZ", &Size3DUint::getZ)
      .def("x", &Size3DUint::x)
      .def("y", &Size3DUint::y)
      .def("z", &Size3DUint::z)
      .def("setX", (void (Size3DUint::*)(uint32_t const&)) & Size3DUint::setX)
      .def("setY", (void (Size3DUint::*)(uint32_t const&)) & Size3DUint::setY)
      .def("setZ", (void (Size3DUint::*)(uint32_t const&)) & Size3DUint::setZ)
      .def("set", (void (Size3DUint::*)(uint32_t const&, uint32_t const&,
                                        uint32_t const&)) &
                      Size3DUint::set)
      .def("__copy__", [](Size3DUint const& rhs) { return Size3DUint(rhs); })
      .def("__repr__", [](Size3DUint const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Size3DFloat>(m, "Size3DFloat")
      .def(py::init<>())
      .def(py::init<float const&, float const&, float const&>())
      .def("getX", &Size3DFloat::getX)
      .def("getY", &Size3DFloat::getY)
      .def("getZ", &Size3DFloat::getZ)
      .def("x", &Size3DFloat::x)
      .def("y", &Size3DFloat::y)
      .def("z", &Size3DFloat::z)
      .def("setX", (void (Size3DFloat::*)(float const&)) & Size3DFloat::setX)
      .def("setY", (void (Size3DFloat::*)(float const&)) & Size3DFloat::setY)
      .def("setZ", (void (Size3DFloat::*)(float const&)) & Size3DFloat::setZ)
      .def("set",
           (void (Size3DFloat::*)(float const&, float const&, float const&)) &
               Size3DFloat::set)
      .def("__copy__", [](Size3DFloat const& rhs) { return Size3DFloat(rhs); })
      .def("__repr__", [](Size3DFloat const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Size3DDouble>(m, "Size3DDouble")
      .def(py::init<>())
      .def(py::init<double const&, double const&, double const&>())
      .def("getX", &Size3DDouble::getX)
      .def("getY", &Size3DDouble::getY)
      .def("getZ", &Size3DDouble::getZ)
      .def("x", &Size3DDouble::x)
      .def("y", &Size3DDouble::y)
      .def("z", &Size3DDouble::z)
      .def("setX", (void (Size3DDouble::*)(double const&)) & Size3DDouble::setX)
      .def("setY", (void (Size3DDouble::*)(double const&)) & Size3DDouble::setY)
      .def("setZ", (void (Size3DDouble::*)(double const&)) & Size3DDouble::setZ)
      .def("set", (void (Size3DDouble::*)(double const&, double const&,
                                          double const&)) &
                      Size3DDouble::set)
      .def("__copy__",
           [](Size3DDouble const& rhs) { return Size3DDouble(rhs); })
      .def("__repr__", [](Size3DDouble const& rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::bind_vector<VectorPoint2DInt>(m, "VectorPoint2DInt");
  py::bind_vector<VectorPoint2DUint>(m, "VectorPoint2DUint");
  py::bind_vector<VectorPoint2DFloat>(m, "VectorPoint2DFloat");
  py::bind_vector<VectorPoint2DDouble>(m, "VectorPoint2DDouble");
  py::bind_vector<VectorPoint3DInt>(m, "VectorPoint3DInt");
  py::bind_vector<VectorPoint3DUint>(m, "VectorPoint3DUint");
  py::bind_vector<VectorPoint3DFloat>(m, "VectorPoint3DFloat");
  py::bind_vector<VectorPoint3DDouble>(m, "VectorPoint3DDouble");
  py::bind_vector<VectorBoxInt>(m, "VectorBoxInt");
  py::bind_vector<VectorBoxUint>(m, "VectorBoxUint");
  py::bind_vector<VectorBoxFloat>(m, "VectorBoxFloat");
  py::bind_vector<VectorBoxDouble>(m, "VectorBoxDouble");
  py::bind_vector<VectorSize2DInt>(m, "VectorSize2DInt");
  py::bind_vector<VectorSize2DUint>(m, "VectorSize2DUint");
  py::bind_vector<VectorSize2DFloat>(m, "VectorSize2DFloat");
  py::bind_vector<VectorSize2DDouble>(m, "VectorSize2DDouble");
  py::bind_vector<VectorSize3DInt>(m, "VectorSize3DInt");
  py::bind_vector<VectorSize3DUint>(m, "VectorSize3DUint");
  py::bind_vector<VectorSize3DFloat>(m, "VectorSize3DFloat");
  py::bind_vector<VectorSize3DDouble>(m, "VectorSize3DDouble");

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "develop";
#endif
}
