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
      cell_vec_id_(UNINIT_OBJECT_ID),
      spef_vec_id_(UNINIT_OBJECT_ID),
      netsparasitics_vec_id_(UNINIT_OBJECT_ID),
      spef_field_(1) {
    setObjectType(ObjectType::kObjectTypeDesignParasitics);
}

DesignParasitics::~DesignParasitics() {
}

DesignParasitics::DesignParasitics(Object* owner, DesignParasitics::IndexType id)
    : DesignParasitics::BaseType(owner, id),
      cell_vec_id_(UNINIT_OBJECT_ID),
      spef_vec_id_(UNINIT_OBJECT_ID),
      netsparasitics_vec_id_(UNINIT_OBJECT_ID),
      spef_field_(1) {
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

void DesignParasitics::addCellId(ObjectId cellId) {
    ArrayObject<ObjectId> *vct = nullptr;
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        if (cell_vec_id_ == UNINIT_OBJECT_ID) {
            vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
            if (vct != nullptr) {
                vct->setPool(timingdb->getPool());
                vct->reserve(10);
                cell_vec_id_ = vct->getId();
            }
        } else
            vct = addr< ArrayObject<ObjectId> >(cell_vec_id_);

        if (vct != nullptr)
            vct->pushBack(cellId);
    }

}

void DesignParasitics::addSpef(SymbolIndex index) {
    ArrayObject<SymbolIndex> *vct = nullptr;
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        if (spef_vec_id_ == UNINIT_OBJECT_ID) {
            vct = timingdb->createObject< ArrayObject<SymbolIndex> >(kObjectTypeArray, timingdb->getId());
            if (vct != nullptr) {
                vct->setPool(timingdb->getPool());
                vct->reserve(10);
                spef_vec_id_ = vct->getId();
            }
        } else
            vct = addr< ArrayObject<SymbolIndex> >(spef_vec_id_);

        if (vct != nullptr)
            vct->pushBack(index);
    }

}

void DesignParasitics::addNetsParasitics(ObjectId netsPara) {
    ArrayObject<ObjectId> *vct = nullptr;
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        if (netsparasitics_vec_id_ == UNINIT_OBJECT_ID) {
            vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
            if (vct != nullptr) {
                vct->setPool(timingdb->getPool());
                vct->reserve(10);
                netsparasitics_vec_id_ = vct->getId();
            }
        } else
            vct = addr< ArrayObject<ObjectId> >(netsparasitics_vec_id_);

        if (vct != nullptr)
            vct->pushBack(netsPara);
    }

}

void DesignParasitics::copy(DesignParasitics const& rhs) {
    this->BaseType::copy(rhs);

    cell_vec_id_ = rhs.cell_vec_id_;
    spef_vec_id_  = rhs.spef_vec_id_;
    netsparasitics_vec_id_ = rhs.netsparasitics_vec_id_;
    spef_field_ = rhs.spef_field_;
}

void DesignParasitics::move(DesignParasitics&& rhs) {
    this->BaseType::move(std::move(rhs));
    cell_vec_id_ = std::move(rhs.cell_vec_id_);
    spef_vec_id_ = std::move(rhs.spef_vec_id_);
    netsparasitics_vec_id_ = std::move(rhs.netsparasitics_vec_id_);
    spef_field_ = std::move(rhs.spef_field_);
}

std::ofstream &operator<<(std::ofstream &os, DesignParasitics const &rhs) {
    if (rhs.netsparasitics_vec_id_ != UNINIT_OBJECT_ID) {
        ArrayObject<ObjectId> *vct =  Object::addr< ArrayObject<ObjectId> >(rhs.netsparasitics_vec_id_);
	if (vct) {
   	    for (size_t i=0; i < vct->getSize(); i++) { 
                NetsParasitics *netsParasitics = Object::addr<NetsParasitics>((*vct)[i]);
		if (netsParasitics)
                    os << *netsParasitics;
                break;         ///Currently only support dump out first spef file	
	    }
	}
    }
    return os;
}

}  // namespace db
}  // namespace open_edi
