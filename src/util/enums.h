/* @file  enums.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/**
 * @file   enums.h
 * @author Yibo Lin
 * @date   Apr 2020
 */

#ifndef EDI_UTIL_ENUMS_H_
#define EDI_UTIL_ENUMS_H_

#include "util/message.h"
#include "util/stream.h"

namespace open_edi {
namespace util {

inline void toLower(std::string &s) {
    int len = s.size();
    for (int i = 0; i < len; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] += 32;  //+32 to lower case
        }
    }
}

inline void toUpper(std::string &s) {
    int len = s.size();
    for (int i = 0; i < len; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] -= 32;  //-32 to upper case
        }
    }
}

/// @brief Types of models.
enum class CellType : uint8_t {
    kCell,       ///< term model
    kHierCell,   ///< hierarchical cell, such as model in Verilog
    kIOPin,      ///< layout model
    kTerm,       ///< special model for module-term
    kSplitCell,  ///< flatten split cell
    kUnknown
};

inline std::string toString(CellType const &v) {
    switch (v) {
        case CellType::kCell:
            return "kCell";
        case CellType::kIOPin:
            return "kIOPin";
        case CellType::kHierCell:
            return "kHierarchicalCell";
        case CellType::kTerm:
            return "kTerm";
        case CellType::kSplitCell:
            return "kSplitCelt";
        default:
            return "Unknown";
    }
}

/// @brief Signal direction types.
enum class SignalDirection : uint8_t {
    kInput,        ///< input signal
    kOutput,       ///< output signal
    kInout,        ///< inout signal
    kFeedThrough,  ///< feed through signal
    kOutputTristate,///< tristate output
    kUnknown
};

/// @brief convert enum to string
inline std::string toString(SignalDirection const &v) {
    switch (v) {
        case SignalDirection::kInput:
            return "Input";
        case SignalDirection::kOutput:
            return "Output";
        case SignalDirection::kInout:
            return "Inout";
        case SignalDirection::kFeedThrough:
            return "FeedThrough";
        case SignalDirection::kOutputTristate:
            return "Output Tristate";            
        default:
            return "Unknown";
    }
}

/// @brief placement status
enum class PlaceStatus : uint8_t {
    kUnplaced,   ///< not placed or arbitrary placed
    kPlaced,     ///< placed at a location
    kSuggested,  ///< object has a suggested location
    kFixed,      ///< object cannot be moved by a placer
    kLocked,     ///< object cannot be moved by anything
    kCover,      ///< io pin is covered
    kUnknown
};

/// @brief convert enum to string
inline std::string toString(PlaceStatus const &v) {
    switch (v) {
        case PlaceStatus::kUnplaced:
            return "Unplaced";
        case PlaceStatus::kPlaced:
            return "Placed";
        case PlaceStatus::kSuggested:
            return "Suggested";
        case PlaceStatus::kFixed:
            return "Fixed";
        case PlaceStatus::kLocked:
            return "Locked";
        case PlaceStatus::kCover:
            return "Cover";
        default:
            return "Unknown";
    }
}

/// @brief orientation
enum class Orient : uint8_t {
    kN,   ///< North, R0 in OpenAccess
    kS,   ///< South, R180 in OpenAccess
    kW,   ///< East, R270 in OpenAccess
    kE,   ///< West, R90 in OpenAccess
    kFN,  ///< Flipped north, MY in OpenAccess
    kFS,  ///< Flipped South, MX in OpenAccess
    kFW,  ///< Flipped West, MX90 in OpenAccess
    kFE,  ///< Flipped East, MY90 in OpenAccess
    kUnknown
};

/// @brief orientation
enum class Direction : uint8_t {
    kHorizontal,
    kVertical,
    kDiagonal,
    kUnknown
};

/// @brief convert enum to string
inline std::string toString(Orient const &v) {
    switch (v) {
        case Orient::kN:
            return "N";
        case Orient::kS:
            return "S";
        case Orient::kW:
            return "W";
        case Orient::kE:
            return "E";
        case Orient::kFN:
            return "FN";
        case Orient::kFS:
            return "FS";
        case Orient::kFW:
            return "FW";
        case Orient::kFE:
            return "FE";
        default:
            return "Unknown";
    }
}

/// @brief Signal type of a net
enum class SignalType : uint8_t {
    kSignal,  ///< signal net
    kPower,   ///< power net
    kGround,  ///< ground net
    kClock,   ///< clock net
    kAnalog,  ///< analog signal
    kReset,   ///< reset net
    kScan,    ///< scan net
    kTieoff,  ///< tie off net
    kUnknown
};

/// @brief convert enum to string
inline std::string toString(SignalType const &v) {
    switch (v) {
        case SignalType::kSignal:
            return "Signal";
        case SignalType::kPower:
            return "Power";
        case SignalType::kGround:
            return "Ground";
        case SignalType::kClock:
            return "Clock";
        case SignalType::kAnalog:
            return "Analog";
        case SignalType::kReset:
            return "Reset";
        case SignalType::kScan:
            return "Scan";
        case SignalType::kTieoff:
            return "Tieoff";
        default:
            return "Unknown";
    }
}

/// @brief Wire type of a net
enum class WireType : uint8_t {
    kNone,      ///< no wire type
    kCover,     ///< cover
    kFixed,     ///< fixed net
    kRouted,    ///< routed
    kShield,    ///< shield
    kNoShield,  ///< no shield
    kUnknown
};

/// @brief convert enum to string
inline std::string toString(WireType const &v) {
    switch (v) {
        case WireType::kNone:
            return "None";
        case WireType::kCover:
            return "Cover";
        case WireType::kFixed:
            return "Fixed";
        case WireType::kRouted:
            return "Routed";
        case WireType::kShield:
            return "Shield";
        case WireType::kNoShield:
            return "NoShield";
        default:
            return "Unknown";
    }
}

/// @brief Types of spacing mode
enum class SpacingMode : uint8_t {
    kEuclideanMode = 0,  ///< eudlidean
    kMaxXYMode,          ///< max XY
    kUnknown
};

inline std::string toString(SpacingMode const &v) {
    switch (v) {
        case SpacingMode::kEuclideanMode:
            return "EuclideanMode";
        case SpacingMode::kMaxXYMode:
            return "MaxXYMode";
        default:
            return "Unknown";
    }
}

/// @brief Types of tech node
enum class TechNode : uint8_t {
    kNode32_28 = 0,  ///<
    kNode16_14,      ///<
    kNode10,
    kNode7,
    kNode5,
    kNodeUnknown
};

inline std::string toString(TechNode const &v) {
    switch (v) {
        case TechNode::kNode32_28:
            return "Node 32/28";
        case TechNode::kNode16_14:
            return "Node 16/14";
        case TechNode::kNode10:
            return "Node 10";
        case TechNode::kNode7:
            return "Node 7";
        case TechNode::kNode5:
            return "Node 5";
        default:
            return "Unknown";
    }
}

/// @brief Types of site class
enum class SiteClass : uint8_t {
    kCore = 0,  ///<
    kPad = 1,   ///<
    kVirtual = 2,
    kUnknown = 3
};

inline std::string toString(SiteClass const &v) {
    switch (v) {
        case SiteClass::kCore:
            return "CORE";
        case SiteClass::kPad:
            return "PAD";
        case SiteClass::kVirtual:
            return "VIRTUAL";
        default:
            return "Unknown";
    }
}

/// @brief Types of symmetry
enum class Symmetry : uint8_t {
    kR90 = 0,  ///<
    kX = 1,
    kY = 2,
    kUnknown = 3
};

inline std::string toString(Symmetry const &v) {
    switch (v) {
        case Symmetry::kR90:
            return "R90";
        case Symmetry::kX:
            return "X";
        case Symmetry::kY:
            return "Y";
        default:
            return "Unknown";
    }
}

/// @brief Types of PropDataType
enum class PropDataType : uint8_t {
    kString = 0,  ///<
    kInt = 1,
    kReal = 2,
    kUnknown = 3
};

inline std::string toString(PropDataType const &v) {
    switch (v) {
        case PropDataType::kString:
            return "STRING";
        case PropDataType::kInt:
            return "INTEGER";
        case PropDataType::kReal:
            return "REAL";
        default:
            return "Unknown";
    }
}

/// @brief Types of PropType
enum class PropType : uint8_t {
    kLayer = 0,  ///<
    kLibrary = 1,
    kMacro = 2,
    kNonDefaultRule = 3,
    kPin = 4,
    kVia = 5,
    kViaRule = 6,
    // for DEF:
    kComponent = 7,
    kComponentPin = 8,
    kDesign = 9,
    kGroup = 10,
    kNet = 11,
    kRegion = 12,
    kRow = 13,
    kSpecialNet = 14,
    kUnknown = 15
};

inline std::string toString(PropType const &v) {
    switch (v) {
        case PropType::kLayer:
            return "LAYER";
        case PropType::kLibrary:
            return "LIBRARY";
        case PropType::kMacro:
            return "MACRO";
        case PropType::kNonDefaultRule:
            return "NONDEFAULTRULE";
        case PropType::kPin:
            return "PIN";
        case PropType::kVia:
            return "VIA";
        case PropType::kViaRule:
            return "VIARULE";
        case PropType::kComponent:
            return "COMPONENT";
        case PropType::kComponentPin:
            return "COMPONENTPIN";
        case PropType::kDesign:
            return "DESIGN";
        case PropType::kGroup:
            return "GROUP";
        case PropType::kNet:
            return "NET";
        case PropType::kRegion:
            return "REGION";
        case PropType::kRow:
            return "ROW";
        case PropType::kSpecialNet:
            return "SPECIALNET";
        default:
            return "UNKNOWN";
    }
}

/*enum class ObjectType : uint8_t {
  kRect,
  kCell,
  kTerm,
  kInst,
  kNet,
  kPin,
  kDesign,
  kPG,
  kSignal,
  kInt
};

inline std::string toString(ObjectType const& v) {
  switch (v) {
    case ObjectType::kRect:
      return "kRect";
    case ObjectType::kCell:
      return "kCell";
    case ObjectType::kTerm:
      return "kTerm";
    case ObjectType::kInst:
      return "kInst";
    case ObjectType::kNet:
      return "kNet";
    case ObjectType::kPin:
      return "kPin";
    case ObjectType::kDesign:
      return "kDesign";
    case ObjectType::kPG:
      return "kPG";
    case ObjectType::kSignal:
      return "kSignal";
    case ObjectType::kInt:
      return "kInt";
    default:
      return "kUnknown";
  }
}
*/
/// @brief Explicitly convert enum type to underlying values
template <typename E>
constexpr inline
    typename std::enable_if<std::is_enum<E>::value,
                            typename std::underlying_type<E>::type>::type
    toInteger(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

/// @brief Explicitly convert an integer value to enum
template <typename E, typename T>
constexpr inline typename std::enable_if<
    std::is_enum<E>::value && std::is_integral<T>::value, E>::type
toEnum(T value) noexcept {
    return static_cast<E>(value);
}

/// @brief Explicitly convert const char* to enum (enum should has kUnknown.)
template <typename E>
constexpr inline typename std::enable_if<std::is_enum<E>::value, E>::type
toEnumByString(const char *v) noexcept {
    for (int i = 0; i < toInteger(E::kUnknown); ++i) {
        if (strcasecmp(v, toString(toEnum<E, int>(i)).c_str()) == 0) {
            // if (toString(toEnum<E, int>(i)).compare(v) == 0) {
            return toEnum<E, int>(i);
        }
    }
    return E::kUnknown;
}

/// @brief Overload output stream for enum type
template <typename E>
inline typename std::enable_if<std::is_enum<E>::value, std::ostream &>::type
operator<<(std::ostream &os, E e) {
    os << toString(e);
    return os;
}

/// @brief Overload output stream for enum type
template <typename E>
inline typename std::enable_if<std::is_enum<E>::value, OStreamBase &>::type
operator<<(OStreamBase &os, E e) {
    os << toString(e);
    return os;
}

template <typename E>
inline typename std::enable_if<std::is_enum<E>::value, IStreamBase &>::type
operator>>(IStreamBase &is, E &e) {
    std::string str;
    is >> str;
    e = toEnumByString<E>(str.c_str());
    return is;
}

}  // namespace util
}  // namespace open_edi

#endif
