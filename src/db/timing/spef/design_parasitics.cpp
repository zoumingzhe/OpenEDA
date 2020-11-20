/** 
 * @file design_parasitics.cpp
 * @date 2020-11-02
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

#include "db/timing/spef/design_parasitics.h"
#include "db/timing/spef/nets_parasitics.h"

#include <map>

#include "db/core/db.h"
#include "util/stream.h"

namespace open_edi {
namespace db {

DesignParasitics::DesignParasitics()
    : DesignParasitics::BaseType(),
      spef_map_(),
      parasitics_map_(),
      spefField_(1) {
    setObjectType(ObjectType::kObjectTypeDesignParasitics);
}

DesignParasitics::~DesignParasitics() {
    spef_map_.clear();
    parasitics_map_.clear();
}

DesignParasitics::DesignParasitics(Object* owner, DesignParasitics::IndexType id)
    : DesignParasitics::BaseType(owner, id),
      spef_map_(),
      parasitics_map_(),
      spefField_(1) {
    setObjectType(ObjectType::kObjectTypeDesignParasitics);
}

DesignParasitics::DesignParasitics(DesignParasitics const& rhs) { copy(rhs); }

DesignParasitics::DesignParasitics(DesignParasitics&& rhs) noexcept { move(std::move(rhs)); }

DesignParasitics& DesignParasitics::operator=(DesignParasitics const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

DesignParasitics& DesignParasitics::operator=(DesignParasitics&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void DesignParasitics::copy(DesignParasitics const& rhs) {
    this->BaseType::copy(rhs);

    spef_map_ = rhs.spef_map_;
    parasitics_map_ = rhs.parasitics_map_;
    spefField_ = rhs.spefField_;
}

void DesignParasitics::move(DesignParasitics&& rhs) {
    this->BaseType::move(std::move(rhs));
    spef_map_ = std::move(rhs.spef_map_);
    parasitics_map_ = std::move(rhs.parasitics_map_);
    spefField_ = std::move(rhs.spefField_);
    rhs.spef_map_.clear();
    rhs.parasitics_map_.clear();
}

OStreamBase &operator<<(OStreamBase &os, DesignParasitics const &rhs) {
   for (auto obj : rhs.getParasiticsMap()) {
       NetsParasitics *netsParasitics = Object::addr<NetsParasitics>(obj.second);
       os << *netsParasitics;
       break;         ///Currently only support dump out first spef file	
   }
}

}  // namespace db
}  // namespace open_edi
