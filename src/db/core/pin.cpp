
#include "db/core/pin.h"

#include "db/core/db.h"
#include "db/core/inst.h"
#include "db/core/net.h"
#include "db/util/transform.h"

namespace open_edi {
namespace db {

Pin::Pin() {
    is_special_ = false;
    is_connect_nets_ = false;
    term_ = 0;
    inst_ = 0;
    net_ = 0;
}

Pin::Pin(Pin const& rhs) { copy(rhs); }

Pin::Pin(Pin&& rhs) noexcept { move(std::move(std::move(rhs))); }

bool Pin::setName(std::string name) {
    // Only do it for primary pin, since no duplicate.
    if (getIsPrimary()){
        SymbolIndex symbol_index = 
            getOwnerCell()->getOrCreateSymbol(name.c_str());
        if (symbol_index == kInvalidSymbolIndex) return false;

        getOwnerCell()->addSymbolReference(symbol_index, getId());
    }
    return true;
}

std::string& Pin::getName() const {
    Term* master_term = getTerm();
    ediAssert(nullptr != master_term);
    return master_term->getName();
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

SignalDirection Pin::getDirection() const {
    Term* term = nullptr;
    term = getTerm();
    if (nullptr != term) return term->getDirection();

    return SignalDirection::kUnknown;
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

void Pin::getBoxVector(std::vector <Box> & box_vector) const {
    Inst *inst = getInst();
    Term *term = getTerm();
    ediAssert(term != nullptr);
    Box box(0, 0, 0, 0);
    for (int index = 0; index < term->getPortNum(); ++index) {
        Port *port = term->getPort(index);
        for (int layergeom_index = 0;
            layergeom_index < port->getLayerGeometryNum(); ++layergeom_index) {
            LayerGeometry *layer_geom = port->getLayerGeometry(layergeom_index);
            for (int geom_i = 0; geom_i < layer_geom->getVecNum(); ++geom_i) {
            Geometry *geo = layer_geom->getGeometry(geom_i);
            if (geo == nullptr ||
                geo->getType() != GeometryType::kRect) continue;
            Box box = geo->getBox();
            if (inst != nullptr) {
                transformByInst(inst, box);
            } else {
                transformByIOPin(this, box);
            }
            box_vector.push_back(box);
            }
        }
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
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getOwnerCell()->getPool());
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
    is_special_ = rhs.is_special_;
    term_ = rhs.term_;
    inst_ = rhs.inst_;
    net_ = rhs.net_;
}

void Pin::move(Pin&& rhs) {
    is_special_ = rhs.is_special_;
    rhs.is_special_ = false;
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

bool Pin::getIsSpecial() const { return is_special_; }

void Pin::setIsSpecial(bool flag) { is_special_ = flag; }

bool Pin::getIsConnectNets() const { return is_connect_nets_; }

/// @brief getIsPrimary 
///
/// @return 
bool Pin::getIsPrimary() const
{
    return is_primary_;
}

/// @brief setIsPrimary 
///
/// @param p
void Pin::setIsPrimary(bool p)
{
    is_primary_ = p;
}

}  // namespace db
}  // namespace open_edi
