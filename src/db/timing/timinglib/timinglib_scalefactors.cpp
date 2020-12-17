/**
 * @file timinglib_scalefactors.cpp
 * @date 2020-10-09
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_scalefactors.h"

#include "db/core/db.h"

namespace open_edi {
namespace db {

ScaleFactors::ScaleFactors() : ScaleFactors::BaseType(), name_(0) {
    __initValue();
    setObjectType(ObjectType::kObjectTypeScaleFactors);
}

ScaleFactors::~ScaleFactors() {}

ScaleFactors::ScaleFactors(Object* owner, ScaleFactors::IndexType id)
    : ScaleFactors::BaseType(owner, id), name_(0) {
    __initValue();
    setObjectType(ObjectType::kObjectTypeScaleFactors);
}

ScaleFactors::ScaleFactors(ScaleFactors const& rhs) { copy(rhs); }

ScaleFactors::ScaleFactors(ScaleFactors&& rhs) noexcept {
    move(std::move(rhs));
}

ScaleFactors& ScaleFactors::operator=(ScaleFactors const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

ScaleFactors& ScaleFactors::operator=(ScaleFactors&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void ScaleFactors::copy(ScaleFactors const& rhs) {
    this->BaseType::copy(rhs);
    name_ = rhs.name_;
    for (int i = 0; i < static_cast<int>(ScaleFactorType::kUnknown); ++i) {
        for (int j = 0; j < static_cast<int>(ScaleFactorPvt::kUnknown); ++j) {
            for (int k = 0; k < static_cast<int>(ScaleFactorRiseFall::kUnknown);
                 ++k) {
                scales_[i][j][k] = rhs.scales_[i][j][k];
            }
        }
    }
}

void ScaleFactors::move(ScaleFactors&& rhs) {
    this->BaseType::move(std::move(rhs));

    name_ = std::move(rhs.name_);
    for (int i = 0; i < static_cast<int>(ScaleFactorType::kUnknown); ++i) {
        for (int j = 0; j < static_cast<int>(ScaleFactorPvt::kUnknown); ++j) {
            for (int k = 0; k < static_cast<int>(ScaleFactorRiseFall::kUnknown);
                 ++k) {
                scales_[i][j][k] = std::move(rhs.scales_[i][j][k]);
            }
        }
    }
}

ScaleFactors::IndexType ScaleFactors::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += (static_cast<int>(ScaleFactorType::kUnknown)) *
           (static_cast<int>(ScaleFactorPvt::kUnknown)) *
           (static_cast<int>(ScaleFactorRiseFall::kUnknown)) * sizeof(float);

    return ret;
}

/// set
void ScaleFactors::setName(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void ScaleFactors::addScale(ScaleFactorType t, ScaleFactorPvt p, float f) {
    scales_[static_cast<int>(t)][static_cast<int>(p)][0] = f;
}
void ScaleFactors::addScale(ScaleFactorType t, ScaleFactorPvt p,
                            ScaleFactorRiseFall rf, float f) {
    scales_[static_cast<int>(t)][static_cast<int>(p)][static_cast<int>(rf)] = f;
}

/// get
std::string ScaleFactors::getName(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
float ScaleFactors::getScale(ScaleFactorType t, ScaleFactorPvt p) {
    return scales_[static_cast<int>(t)][static_cast<int>(p)][0];
}
float ScaleFactors::getScale(ScaleFactorType t, ScaleFactorPvt p,
                             ScaleFactorRiseFall rf) {
    return scales_[static_cast<int>(t)][static_cast<int>(p)]
                  [static_cast<int>(rf)];
}

bool ScaleFactors::isRiseFallSuffix(ScaleFactorType type) {
    return type == ScaleFactorType::kCell || type == ScaleFactorType::kHold ||
           type == ScaleFactorType::kSetup ||
           type == ScaleFactorType::kRecovery ||
           type == ScaleFactorType::kRemoval ||
           type == ScaleFactorType::kNochange || type == ScaleFactorType::kSkew;
}

bool ScaleFactors::isRiseFallPrefix(ScaleFactorType type) {
    return type == ScaleFactorType::kTransition;
}

bool ScaleFactors::isHighLowSuffix(ScaleFactorType type) {
    return type == ScaleFactorType::kMin_Pulse_Width;
}

void ScaleFactors::__initValue() {
    for (int i = 0; i < static_cast<int>(ScaleFactorType::kUnknown); ++i) {
        for (int j = 0; j < static_cast<int>(ScaleFactorPvt::kUnknown); ++j) {
            for (int k = 0; k < static_cast<int>(ScaleFactorRiseFall::kUnknown);
                 ++k) {
                scales_[i][j][k] = 0.0;
            }
        }
    }
}

OStreamBase& operator<<(OStreamBase& os, ScaleFactors const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    ScaleFactors::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.getName() << DataDelimiter();
    os << DataFieldName("scales_");

    os << DataBegin("[");
    auto delimiter = DataDelimiter("");
    for (int i = 0; i < static_cast<int>(ScaleFactorType::kUnknown); ++i) {
        for (int j = 0; j < static_cast<int>(ScaleFactorPvt::kUnknown); ++j) {
            for (int k = 0; k < static_cast<int>(ScaleFactorRiseFall::kUnknown);
                 ++k) {
                os << delimiter << rhs.scales_[i][j][k];
                delimiter = DataDelimiter();
            }
        }
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
