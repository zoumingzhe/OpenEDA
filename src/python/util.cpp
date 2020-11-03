/**
 * @file   util.cpp
 * @author Yibo Lin
 * @date   Mar 2020
 */

#include "python/util.h"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"

namespace py = pybind11;

void bind_util(py::module &m) {
  m.doc() = R"pbdoc(
        Pybind11 plugin
        -----------------------
        .. currentmodule:: util
        .. autosummary::
           :toctree: _generate
           MessageType 
           SignalDirection
           PlaceStatus
           Orient 
           SignalType
    )pbdoc";

  py::enum_<MessageType>(m, "MessageType")
      .value("kNone", MessageType::kNone)
      .value("kInfo", MessageType::kInfo)
      .value("kWarn", MessageType::kWarn)
      .value("kError", MessageType::kError)
      .value("kDebug", MessageType::kDebug)
      .value("kAssert", MessageType::kAssert);

  py::enum_<SignalDirection>(m, "SignalDirection")
      .value("kInput", SignalDirection::kInput)
      .value("kOutput", SignalDirection::kOutput)
      .value("kInout", SignalDirection::kInout)
      .value("kUnknown", SignalDirection::kUnknown);

  py::enum_<PlaceStatus>(m, "PlaceStatus")
      .value("kUnplaced", PlaceStatus::kUnplaced)
      .value("kPlaced", PlaceStatus::kPlaced)
      .value("kSuggested", PlaceStatus::kSuggested)
      .value("kFixed", PlaceStatus::kFixed)
      .value("kLocked", PlaceStatus::kLocked)
      .value("kUnknown", PlaceStatus::kUnknown);

  py::enum_<Orient>(m, "Orient")
      .value("kN", Orient::kN)
      .value("kS", Orient::kS)
      .value("kW", Orient::kW)
      .value("kE", Orient::kE)
      .value("kFN", Orient::kFN)
      .value("kFS", Orient::kFS)
      .value("kFW", Orient::kFW)
      .value("kFE", Orient::kFE)
      .value("kUnknown", Orient::kUnknown);

  py::enum_<SignalType>(m, "SignalType")
      .value("kSignal", SignalType::kSignal)
      .value("kPower", SignalType::kPower)
      .value("kGround", SignalType::kGround)
      .value("kClock", SignalType::kClock)
      .value("kAnalog", SignalType::kAnalog)
      .value("kReset", SignalType::kReset)
      .value("kScan", SignalType::kScan)
      .value("kTieoff", SignalType::kTieoff)
      .value("kUnknown", SignalType::kUnknown);

  py::enum_<WireType>(m, "WireType")
      .value("kNone", WireType::kNone)
      .value("kCover", WireType::kCover)
      .value("kFixed", WireType::kFixed)
      .value("kRouted", WireType::kRouted)
      .value("kShield", WireType::kShield)
      .value("kNoShield", WireType::kNoShield)
      .value("kUnknown", WireType::kUnknown);

  py::enum_<CellType>(m, "CellType")
      .value("kCell", CellType::kCell)
      .value("kIOPin", CellType::kIOPin)
      .value("kModule", CellType::kModule)
      .value("kTerm", CellType::kTerm)
      .value("kSplitCell", CellType::kSplitCell)
      .value("kUnknown", CellType::kUnknown);

  /// @brief bind print function
  m.def(
      "ediPrint",
      [](MessageType m, std::string const &msg) {
        EDI_NAMESPACE::ediPrint(m, "%s", msg.c_str());
      },
      "Print message");
  // testing message function
  m.def(
      "messageTest",
      []() {
        EDI_NAMESPACE::utilTestMsg();
      },
      "test message");


#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "develop";
#endif
}
