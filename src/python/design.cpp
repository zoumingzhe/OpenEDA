/**
 * File              : design.cpp
 * Author            : Yibo Lin <yibolin@pku.edu.cn>
 * Date              : 04.24.2020
 * Last Modified Date: 04.24.2020
 * Last Modified By  : Yibo Lin <yibolin@pku.edu.cn>
 */

#include "python/db.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(InstAttr);
PYBIND11_MAKE_OPAQUE(NetAttr);
PYBIND11_MAKE_OPAQUE(PinAttr);

PYBIND11_MAKE_OPAQUE(Inst);
PYBIND11_MAKE_OPAQUE(Net);
PYBIND11_MAKE_OPAQUE(Pin);

void bind_design(py::module &m) {
  using IndexType = Object::IndexType;
  using CoordinateType = Object::CoordinateType;

  py::class_<InstAttr, ObjectAttr>(m, "InstAttr")
      .def(py::init<>())
      .def("getName", &InstAttr::getName)
      .def("setName", (void (InstAttr::*)(std::string const &)) & InstAttr::setName)
      .def("getLoc", &InstAttr::getLoc)
      .def("setLoc",
           (void (InstAttr::*)(InstAttr::PointType const &)) & InstAttr::setLoc)
      .def("memory", &InstAttr::memory)
      .def("clone", &InstAttr::clone, py::return_value_policy::move)
      .def("__repr__", [](InstAttr const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<NetAttr, ObjectAttr>(m, "NetAttr")
      .def(py::init<>())
      .def("getName", &NetAttr::getName)
      .def("setName", (void (NetAttr::*)(std::string const &)) & NetAttr::setName)
      .def("getWeight", &NetAttr::getWeight)
      .def("setWeight", (void (NetAttr::*)(NetAttr::WeightType const &)) &
                            NetAttr::setWeight)
      .def("clone", &NetAttr::clone, py::return_value_policy::move)
      .def("memory", &NetAttr::memory)
      .def("__repr__", [](NetAttr const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<PinAttr, ObjectAttr>(m, "PinAttr")
      .def(py::init<>())
      .def("getTerm",
           (Term const *(PinAttr::*)()) & PinAttr::getTerm,
           py::return_value_policy::reference_internal)
      .def("setTerm",
           (void (PinAttr::*)(Term const *)) & PinAttr::setTerm)
      .def("clone", &PinAttr::clone, py::return_value_policy::move)
      .def("memory", &PinAttr::memory)
      .def("__repr__", [](PinAttr const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Inst>(m, "Inst")
      .def(py::init<>())
      .def(py::init<Object *, IndexType>())
      .def("getAttr", (InstAttr * (Inst::*)()) & Inst::getAttr,
           py::return_value_policy::reference_internal)
      .def("setAttr", (void (Inst::*)(InstAttr *)) & Inst::setAttr)
      .def("numPins", &Inst::numPins)
      .def("getPin", (Pin * (Inst::*)(IndexType)) & Inst::getPin,
           py::return_value_policy::reference_internal)
      .def("addPin", (void (Inst::*)(Pin *)) & Inst::addPin)
      .def("getCell", (Cell * (Inst::*)()) & Inst::getCell, py::return_value_policy::reference_internal)
      .def("addCell", (void (Inst::*)(Cell *)) & Inst::addCell)
      .def("getDesign", (Design * (Inst::*)()) & Inst::getDesign, py::return_value_policy::reference_internal)
      .def("addDesign", (void (Inst::*)(Design *)) & Inst::addDesign)
      .def("getPlaceStatus", &Inst::getPlaceStatus)
      .def("setPlaceStatus",
           (void (Inst::*)(PlaceStatus const &)) & Inst::setPlaceStatus)
      .def("getOrient", &Inst::getOrient)
      .def("setOrient", (void (Inst::*)(Orient const &)) & Inst::setOrient)
      .def("memory", &Inst::memory)
      .def("__repr__", [](Inst const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Net>(m, "Net")
      .def(py::init<>())
      .def(py::init<Object *, IndexType>())
      .def("getAttr", (NetAttr * (Net::*)()) & Net::getAttr,
           py::return_value_policy::reference_internal)
      .def("setAttr", (void (Net::*)(NetAttr *)) & Net::setAttr)
      .def("numPins", &Net::numPins)
      .def("getPin", (Pin * (Net::*)(IndexType)) & Net::getPin,
           py::return_value_policy::reference_internal)
      .def("addPin", (void (Net::*)(Pin *)) & Net::addPin)
      .def("getSignalType", &Net::getSignalType)
      .def("setSignalType",
           (void (Net::*)(SignalType const &)) & Net::setSignalType)
      .def("getWireType", &Net::getWireType)
      .def("setWireType", (void (Net::*)(WireType const &)) & Net::setWireType)
      .def("memory", &Net::memory)
      .def("__repr__", [](Net const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  py::class_<Pin>(m, "Pin")
      .def(py::init<>())
      .def(py::init<Object *, IndexType>())
      .def("getAttr", (PinAttr * (Pin::*)()) & Pin::getAttr,
           py::return_value_policy::reference_internal)
      .def("setAttr", (void (Pin::*)(PinAttr *)) & Pin::setAttr)
      .def("getInst", (Inst * (Pin::*)()) & Pin::getInst,
           py::return_value_policy::reference_internal)
      .def("setInst", (void (Pin::*)(Inst *)) & Pin::setInst)
      .def("getNet", (Net * (Pin::*)()) & Pin::getNet,
           py::return_value_policy::reference_internal)
      .def("setNet", (void (Pin::*)(Net *)) & Pin::setNet)
      .def("memory", &Pin::memory)
      .def("__repr__", [](Pin const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });
  
  py::class_<Design, Object>(m, "Design")
      .def(py::init<>())
      .def(py::init<Object *, IndexType>())
      .def("getName", &Design::getName)
      .def("setNanme", (void (Design::*)(std::string const &)) & Design::setName)
      .def("getUniquified", &Design::getUniquified)
      .def("setUniquified", (void (Design::*)(bool)) & Design::setUniquified)
      .def("getFlattened", &Design::getFlattened)
      .def("setUniquified", (void (Design::*)(bool)) & Design::setFlattened)
      .def("numInsts", &Design::numInsts)
      .def("getInst", (Inst * (Design::*)(IndexType)) & Design::getInst, py::return_value_policy::reference_internal)
      .def("numNets", &Design::numNets)
      .def("getNet", (Net * (Design::*)(IndexType)) & Design::getNet, py::return_value_policy::reference_internal)
      .def("numPins", &Design::numPins)
      .def("getPin", (Pin * (Design::*)(IndexType)) & Design::getPin, py::return_value_policy::reference_internal)
      .def("addInst", (Inst * (Design::*)()) & Design::addInst, py::return_value_policy::reference_internal)
      .def("addInst", (Inst * (Design::*)(InstAttr const &)) & Design::addInst, py::return_value_policy::reference_internal)
      .def("addNet", (Net * (Design::*)()) & Design::addNet, py::return_value_policy::reference_internal)
      .def("addNet", (Net * (Design::*)(NetAttr const &)) & Design::addNet, py::return_value_policy::reference_internal)
      .def("addPin", (Pin * (Design::*)()) & Design::addPin, py::return_value_policy::reference_internal)
      .def("addPin", (Pin * (Design::*)(PinAttr const &)) & Design::addPin, py::return_value_policy::reference_internal)
      .def("uniquify", (void (Design::*)()) & Design::uniquify)
      .def("flatten", (void (Design::*)(Cell *)) & Design::uniquify)
      .def("numTerms", &Design::numTerms)
      .def("initInstsOfTerms", (void (Design::*)(int)) & Design::initInstsOfTerms)
      .def("setInstOfTerm", (void (Design::*)(IndexType, IndexType)) & Design::setInstOfTerm)
      .def("getInstOfTerm", (Inst* (Design::*)(IndexType)) &Design::getInstOfTerm, py::return_value_policy::reference_internal)
      .def("getInstIdOfTerm", (IndexType (Design::*)(IndexType)) &Design::getInstIdOfTerm)
      .def("clear", (void (Design::*)()) & Design::clear)
      .def("memory", &Design::memory)
      .def("__repr__", [](Design const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });
}
