/**
 * File              : db.cpp
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

void bind_cell(py::module &);
void bind_design(py::module &);

PYBIND11_MAKE_OPAQUE(Object);
PYBIND11_MAKE_OPAQUE(ObjectAttr);
// PYBIND11_MAKE_OPAQUE(AttrObject);

void bind_db(py::module &m) {
  m.doc() = R"pbdoc(
        Pybind11 plugin
        -----------------------
        .. currentmodule:: util
        .. autosummary::
           :toctree: _generate
           Object
           ObjectAttr
           AttrObject
           InstAttr
           InstTermAttr
           NetAttr  
           Inst 
           Net 
           InstTerm 
    )pbdoc";

  using IndexType = Object::IndexType;
  using CoordinateType = Object::CoordinateType;

  // bind base Object
  py::class_<Object>(m, "Object")
      .def(py::init<>())
      .def(py::init<Object *, IndexType>())
      .def("getOwner", (Object * (Object::*)()) & Object::getOwner,
           py::return_value_policy::reference_internal)
      .def("setOwner", (void (Object::*)(Object *)) & Object::setOwner)
      .def("getId", &Object::getId)
      .def("setId", (void (Object::*)(IndexType)) & Object::setId)
      .def("memory", &Object::memory)
      .def("__repr__", [](Object const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  // cannot be abstract class
  // I am not able to figure out how to work with abstract class
  // and using raw pointers.
  // There are examples to use std::shared_ptr
  py::class_<ObjectAttr>(m, "ObjectAttr")
      .def(py::init<>())
      //.def("clone", &ObjectAttr::clone, py::return_value_policy::move)
      .def("memory", &ObjectAttr::memory);

  py::class_<Database, Object>(m, "Database")
      .def(py::init<>())
      .def(py::init<Object *, IndexType>())
      .def("numCells", &Database::numCells)
      .def("getCell", (Cell * (Database::*)(IndexType)) & Database::getCell, py::return_value_policy::reference_internal)
      .def("addCell", (Cell * (Database::*)(std::string const &)) & Database::addCell, py::return_value_policy::reference_internal)
      .def("getDesign", (Design * (Database::*)()) & Database::getDesign, py::return_value_policy::reference_internal)
      .def("uniquify", (void (Database::*)()) & Database::uniquify)
      .def("flatten", (void (Database::*)()) & Database::flatten)
      .def("flatten", (void (Database::*)(Design *)) & Database::flatten)
      .def("createSplitCell", (void (Database::*)()) & Database::createSplitCell)
      .def("writeVerilog", (bool (Database::*)(std::string const&)) & Database::writeVerilog)
      .def("drawGraphviz", (bool (Database::*)(std::string const&)) & Database::drawGraphviz)
      .def("memory", &Database::memory)
      .def("__repr__", [](Database const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });

  // bind reading database file  
  m.def("read", [](Database& rhs, std::string const& filename, bool binary){
        auto mode = std::ios::in; 
        if (binary) {
          mode |= std::ios::binary; 
        }
        EDI_NAMESPACE::IStream<std::ifstream> is(rhs, filename.c_str(), mode);
        ediAssertMsg(is.isOpen(), "failed to open %s for read", filename.c_str());
        is >> rhs; 
        is.close(); 
      });

  // bind writing database file  
  m.def("write", [](Database const& rhs, std::string const& filename, bool binary){
        auto mode = std::ios::out; 
        if (binary) {
          mode |= std::ios::binary; 
        }
        EDI_NAMESPACE::OStream<std::ofstream> os(filename.c_str(), mode);
        ediAssertMsg(os.isOpen(), "failed to open %s for write", filename.c_str());
        os << rhs; 
        os.close(); 
      });

  // As AttrObject is a template class now, I directly move the APIs
  // to the instantiated classes
  // py::class_<AttrObject, Object>(m, "AttrObject")
  //    .def(py::init<>())
  //    .def(py::init<Object *, IndexType>())
  //    .def("getAttr", (ObjectAttr * (AttrObject::*)()) & AttrObject::getAttr,
  //         py::return_value_policy::reference_internal)
  //    .def("setAttr",
  //         (void (AttrObject::*)(ObjectAttr *)) & AttrObject::setAttr)
  //    .def("memory", &AttrObject::memory)
  //    .def("__repr__", [](AttrObject const &rhs) {
  //      OStream<std::ostringstream> os;
  //      os << rhs;
  //      return os.getStream().str();
  //    });

  bind_cell(m);
  bind_design(m);
}
