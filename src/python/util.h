/**
 * @file   util.h
 * @author Yibo Lin
 * @date   Mar 2020
 */

#ifndef EDI_PYTHON_UTIL_H_
#define EDI_PYTHON_UTIL_H_

#include <cstdint>

#include "util/util.h"

using MessageType = EDI_NAMESPACE::MessageType;
using SignalDirection = EDI_NAMESPACE::SignalDirection;
using PlaceStatus = EDI_NAMESPACE::PlaceStatus;
using Orient = EDI_NAMESPACE::Orient;
using SignalType = EDI_NAMESPACE::SignalType;
using WireType = EDI_NAMESPACE::WireType;
using CellType = EDI_NAMESPACE::CellType;

template <typename T>
using OStream = EDI_NAMESPACE::OStream<T>;

#endif
