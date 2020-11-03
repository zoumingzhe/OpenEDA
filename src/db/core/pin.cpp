
#include "db/core/pin.h"

#include "db/core/db.h"
#include "db/core/inst.h"
#include "db/core/net.h"

namespace open_edi {
namespace db {

Pin::Pin() {
    has_special_ = false;
    is_connect_nets_ = false;
    name_index_ = 0;
    term_ = 0;
    inst_ = 0;
    net_ = 0;
}

Pin::Pin(Pin const& rhs) { copy(rhs); }

Pin::Pin(Pin&& rhs) noexcept { move(std::move(std::move(rhs))); }

void Pin::setName(std::string name) {
    Cell* top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when set pin name %s \n",
                          name.c_str());
        return;
    }
    name_index_ = top_cell->getOrCreateSymbol(name);
    top_cell->addSymbolReference(name_index_, this->getId());
}

std::string& Pin::getName() const {
    Cell* top_cell = getTopCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(name_index_);
}

Pin& Pin::operator=(Pin const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

Pin& Pin::operator=(Pin&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(std::move(rhs)));
    }
    return *this;
}

Term* Pin::getTerm() const {
    if (!term_) {
        return nullptr;
    }
    return addr<Term>(term_);
}

void Pin::setTerm(Term* v) {
    if (v == nullptr) {
        term_ = 0;
    } else {
        term_ = v->getId();
    }  
}

Inst* Pin::getInst() const {
    if (!inst_) {
        return nullptr;
    }
    return addr<Inst>(inst_); 
}

void Pin::setInst(Inst* v) {
    if (v == nullptr) {
        inst_ = 0;
    } else {
        inst_ = v->getId();
    }
}

Net* Pin::getNet() const {
    if (!net_) {
        return nullptr;
    }
    return addr<Net>(net_); 
}

void Pin::setNet(Net* net) {
    if (net == nullptr) {
        net_ = 0;
    } else {
        net_ = net->getId();
    }
}

void Pin::addNet(Net *net) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (nets_ == 0) {
        vct = getTopCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getTopCell()->getPool());
        vct->reserve(8);
        nets_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(nets_);
    }

    if (vct) {
        vct->pushBack(net->getId());
        is_connect_nets_ = true;
    }
}

ArrayObject<ObjectId> *Pin::getNetArray() const {
    if (is_connect_nets_) {
        return addr< ArrayObject<ObjectId> >(nets_);
    } else {
        return nullptr;
    }
}

void Pin::copy(Pin const& rhs) {
    has_special_ = rhs.has_special_;
    name_index_ = rhs.name_index_;
    term_ = rhs.term_;
    inst_ = rhs.inst_;
    net_ = rhs.net_;
}

void Pin::move(Pin&& rhs) {
    has_special_ = rhs.has_special_;
    rhs.has_special_ = false;
    name_index_ = std::exchange(rhs.name_index_, 0);
    term_ = std::exchange(rhs.term_, 0);
    inst_ = std::exchange(rhs.inst_, 0);
    net_ = std::exchange(rhs.net_, 0);
}

OStreamBase& operator<<(OStreamBase& os, Pin const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    os << DataFieldName("inst_");
    if (rhs.inst_) {
        os << rhs.inst_;
    } else {
        os << UNINIT_OBJECT_ID;
    }
    os << DataDelimiter();

    os << DataFieldName("term_");
    if (rhs.term_) {
        os << rhs.term_;
    } else {
        os << UNINIT_OBJECT_ID;
    }

    os << DataDelimiter();
    os << DataFieldName("net_");
    if (rhs.net_) {
        os << rhs.net_;
    } else {
        os << UNINIT_OBJECT_ID;
    }
    os << DataDelimiter();

    os << DataEnd(")");
    return os;
}

IStreamBase& operator>>(IStreamBase& is, Pin& rhs) {
    is >> DataTypeName(className(rhs)) >> DataBegin("(");

    auto context = is.getContext();

    // add current context to Pin
    // is.addContext(&rhs);

    Pin::IndexType inst_id;
    is >> DataFieldName("inst_") >> inst_id >> DataDelimiter();
    if (inst_id == UNINIT_OBJECT_ID) {
        rhs.inst_ = 0;
    } 
    Pin::IndexType net_id;
    is >> DataFieldName("net_") >> net_id >> DataDelimiter();
    if (net_id == UNINIT_OBJECT_ID) {
        rhs.net_ = 0;
    }

    is >> DataEnd(")");
    return is;
}

bool Pin::getHasSpecial() const { return has_special_; }

void Pin::setHasSpecial(bool flag) { has_special_ = flag; }

bool Pin::getIsConnectNets() const { return is_connect_nets_; }
}  // namespace db
}  // namespace open_edi
