/**
 * @file   inst_term_attr.cpp
 * @date   Apr 2020
 */

#include "db/core/pin_attr.h"

#include "db/core/cell.h"
#include "db/core/inst.h"
#include "db/core/pin.h"
#include "db/core/term.h"

namespace open_edi {
namespace db {

PinAttr::PinAttr() : PinAttr::BaseType(), cell_term_(nullptr) {}

PinAttr::PinAttr(PinAttr const& rhs) { copy(rhs); }

PinAttr::PinAttr(PinAttr&& rhs) noexcept { move(std::move(rhs)); }

PinAttr& PinAttr::operator=(PinAttr const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

PinAttr& PinAttr::operator=(PinAttr&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

PinAttr::IndexType PinAttr::memory() const {
    IndexType ret = sizeof(*this);
    ret += sizeof(cell_term_);
    return ret;
}

PinAttr* PinAttr::clone() const { return new PinAttr(*this); }

void PinAttr::copy(PinAttr const& rhs) { cell_term_ = rhs.cell_term_; }

void PinAttr::move(PinAttr&& rhs) {
    cell_term_ = std::exchange(rhs.cell_term_, nullptr);
}

OStreamBase& operator<<(OStreamBase& os, PinAttr const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    os << DataFieldName("cell_term_");
    if (rhs.cell_term_) {
        os << rhs.cell_term_->getId();
    } else {
        os << UNINIT_OBJECT_ID;
    }

    os << DataEnd(")");
    return os;
}

IStreamBase& operator>>(IStreamBase& is, PinAttr& rhs) {
    is >> DataTypeName(className(rhs)) >> DataBegin("(");

    auto context = is.getContext();
    auto inst_term = static_cast<Pin*>(context);
    // ediAssertMsg(inst_term,
    //             "PinAttr must be read in the context of Pin");

    auto inst = inst_term->getInst();
    auto model = inst->getMaster();
    Object::IndexType cell_term_id;
    is >> DataFieldName("cell_term_") >> cell_term_id;
    auto cell_term = model->getTerm(cell_term_id);
    rhs.cell_term_ = cell_term;

    is >> DataEnd(")");
    return is;
}

}  // namespace db
}  // namespace open_edi
