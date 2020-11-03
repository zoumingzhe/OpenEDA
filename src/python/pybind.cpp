/**
 * @file   pybind.cpp
 * @date   Apr 2020
 */

#include "util/util.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;

void bind_util(py::module&);
void bind_geo(py::module&);
// void bind_ds(py::module&);
void bind_db(py::module&);

PYBIND11_MODULE(openedi, m) {
  m.doc() = R"pbdoc(
        Pybind11 plugin
        -----------------------
        .. currentmodule:: openedi
        .. autosummary::
           :toctree: _generate
           util (utility)
           geo (geometry)
           ds (data structure)
           db (database)
    )pbdoc";

  bind_util(m);

  auto m_geo = m.def_submodule("geo");
  bind_geo(m_geo);

  // auto m_ds = m.def_submodule("ds");
  // bind_ds(m_ds);

  auto m_db = m.def_submodule("db");
  bind_db(m_db);
}
