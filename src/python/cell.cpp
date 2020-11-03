/**
 * File              : cell.cpp
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

PYBIND11_MAKE_OPAQUE(Cell);
PYBIND11_MAKE_OPAQUE(Term);

void bind_cell(py::module &m) {
  using IndexType = Object::IndexType;
  using CoordinateType = Object::CoordinateType;
  // using CellType = Object::CellType;
  using CellType = edi::CellType;

  py::class_<Cell, Object>(m, "Cell")
      .def(py::init<>())
      .def(py::init<Object *, IndexType>())
      .def("memory", &Cell::memory)
      .def("getCellType", &Cell::getCellType)
      .def("setCellType", (void (Cell::*)(CellType const &)) & Cell::setCellType)
      .def("getName", &Cell::getName)
      .def("setName", (void (Cell::*)(std::string const &)) & Cell::setName)
      .def("numTerms", &Cell::numTerms)
      .def("getTerm", (Term * (Cell::*)(IndexType)) & Cell::getTerm, py::return_value_policy::reference_internal)
      //.def("getTermId", &Cell::getTermId)
      .def("getDesign", (Design * (Cell::*)()) & Cell::getDesign, py::return_value_policy::reference_internal)
      .def("addTerm", &Cell::addTerm, py::return_value_policy::reference_internal)
      .def("addDesign", &Cell::addDesign, py::return_value_policy::reference_internal)
      .def("clear", (void (Cell::*)()) & Cell::clear)
      .def("__repr__", [](Cell const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });


  py::class_<Term, Object>(m, "Term")
      .def(py::init<>())
      .def(py::init<Object *, IndexType>())
      .def("getName", &Term::getName)
      .def("setName", (void (Term::*)(std::string const &)) & Term::setName)
      .def("getDirection", &Term::getDirection)
      .def("setDirection",
           (void (Term::*)(Directionion const &)) & Term::setDirection)
      .def("memory", &Term::memory)
      .def("__repr__", [](Term const &rhs) {
        OStream<std::ostringstream> os;
        os << rhs;
        return os.getStream().str();
      });
}
