/* @file  wire.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/wire.h"

#include "db/core/db.h"
#include "db/tech/tech.h"
#include "db/util/array.h"
namespace open_edi {
namespace db {

/**
 * @brief Construct a new Wire Patch:: Wire Patch object
 *
 */
WirePatch::WirePatch() {
    delta_x_1_ = 0;
    delta_x_2_ = 0;
    delta_y_1_ = 0;
    delta_y_2_ = 0;
}

/**
 * @brief Construct a new Wire Patch:: Wire Patch object
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
WirePatch::WirePatch(int x1, int y1, int x2, int y2) {
    delta_x_1_ = x1;
    delta_x_2_ = x2;
    delta_y_1_ = y1;
    delta_y_2_ = y2;
}

/**
 * @brief  get X1
 *
 * @return int
 */
int WirePatch::getX1() const { return delta_x_1_; }

/**
 * @brief get X2
 *
 * @return int
 */
int WirePatch::getX2() const { return delta_x_2_; }

/**
 * @brief get Y1
 *
 * @return int
 */
int WirePatch::getY1() const { return delta_y_1_; }

/**
 * @brief get Y2
 *
 * @return int
 */
int WirePatch::getY2() const { return delta_y_2_; }

/**
 * @brief set X1
 *
 * @param x1
 */
void WirePatch::setX1(int x1) { delta_x_1_ = x1; }

/**
 * @brief set X2
 *
 * @param x2
 */
void WirePatch::setX2(int x2) { delta_x_2_ = x2; }

/**
 * @brief set Y1
 *
 * @param y1
 */
void WirePatch::setY1(int y1) { delta_y_1_ = y1; }

/**
 * @brief set Y2
 *
 * @param y2
 */
void WirePatch::setY2(int y2) { delta_y_2_ = y2; }

/**
 * @brief Print Out
 *
 */
void WirePatch::print() {
    message->info("RECT ( %d %d %d %d ) ", delta_x_1_, delta_y_1_, delta_x_2_,
                  delta_y_2_);
}

/**
 * @brief Print Out
 *
 */
void WirePatch::printDEF(FILE* fp) {
    fprintf(fp, "RECT ( %d %d %d %d ) ", delta_x_1_, delta_y_1_, delta_x_2_,
            delta_y_2_);
}

/**
 * @brief Construct a new Wire Node:: Wire Node object
 *
 */
WireNode::WireNode() {
    is_new_layer_ = 0;
    has_tapper_ = 0;
    has_tapper_rule_ = 0;
    style_ = 0;
    taper_rule_index_ = 0;
    extension_ = 0;
    mask_ = 0;
    status_ = 0;
    is_virtual_ = 0;
    is_via_ = 0;
    patchs_ = 0;
    x_ = 0;
    y_ = 0;
    z_ = 0;
}

/**
 * @brief Construct a new Wire Node:: Wire Node object
 *
 * @param x
 * @param y
 * @param z
 */
WireNode::WireNode(int x, int y, int z) {
    setX(x);
    setY(y);
    setZ(z);
    patchs_ = 0;
    extension_ = 0;
}

/**
 * @brief ceate Patch in wire node
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @return WirePatch*
 */
WirePatch* WireNode::creatPatch(int x1, int y1, int x2, int y2) {
    WirePatch* patch = getTopCell()->createObject<WirePatch>(kObjectTypeWire);
    patch->setX1(x1);
    patch->setX2(x2);
    patch->setY1(y1);
    patch->setY2(y2);
    patch->setOwner(this);
    if (patchs_ == 0) {
        ArrayObject<ObjectId>* patch_vector = getTopCell()->createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        patch_vector->setPool(getTopCell()->getPool());
        patch_vector->reserve(256);      
        patchs_ = patch_vector->getId();
    }
    ArrayObject<ObjectId>* patch_vector =
        addr<ArrayObject<ObjectId>>(patchs_);
    patch_vector->pushBack(patch->getId());
    return patch;
}

/**
 * @brief Set X
 *
 * @param x
 */
void WireNode::setX(int x) { x_ = x; }

/**
 * @brief Set Y
 *
 * @param y
 */
void WireNode::setY(int y) { y_ = y; }

/**
 * @brief Set Z
 *
 * @param z
 */
void WireNode::setZ(int z) { z_ = z; }

/**
 * @brief Get X
 *
 * @return int
 */
int WireNode::getX() { return x_; }

/**
 * @brief Get Y
 *
 * @return int
 */
int WireNode::getY() { return y_; }

/**
 * @brief Get Z
 *
 * @return int
 */
int WireNode::getZ() { return z_; }
/**
 * @brief get the Status object
 *
 * @param status
 */
Bits WireNode::getStatus() const { return status_; }

/**
 * @brief Set the Status object
 *
 * @param status
 */
void WireNode::setStatus(Bits status) { status_ = status; }

/**
 * @brief Get the Mask object
 *
 * @return Bits
 */

/**
 * @brief Get mask
 *
 * @return Bits
 */
Bits WireNode::getMask() const { return mask_; }

/**
 * @brief Set mask
 *
 * @param mask
 */
void WireNode::setMask(Bits mask) { mask_ = mask; }

/**
 * @brief Get extension
 *
 * @return int
 */
int WireNode::getExtension() const { return extension_; }

/**
 * @brief Set extension
 *
 * @param extension
 */
void WireNode::setExtension(int extension) { extension_ = extension; }

/**
 * @brief Get virtul status
 *
 * @return Bits
 */
Bits WireNode::getIsVirtul() const { return is_virtual_; }

/**
 * @brief set virtul status
 *
 * @param is_virtual
 */
void WireNode::setIsVirtul(Bits is_virtual) { is_virtual_ = is_virtual; }

/**
 * @brief Add out edge of wire node
 *
 * @param out
 */
void WireNode::addOutEdgeList(Wire* out) { out_edges_.push_back(out); }

/**
 * @brief Set via name
 *
 * @param name
 */
int WireNode::setViaName(std::string const& via_name) {
    int64_t index = getTopCell()->getOrCreateSymbol(via_name.c_str());
    if (index == -1) return false;

    via_index_ = index;
    getTopCell()->addSymbolReference(via_index_, this->getId());
    return true;
}

/**
 * @brief Get via name
 *
 * @return const char*
 */
std::string const& WireNode::getViaName() {
    return getTopCell()->getSymbolByIndex(via_index_);
}

/**
 * @brief Get wire node type
 *
 * @return Bits
 */
Bits WireNode::getIsVia() { return is_via_; }

/**
 * @brief Set wire node type
 *
 * @param is_via
 */
void WireNode::setIsVia(Bits is_via) { is_via_ = is_via; }

/**
 * @brief get new layer type
 *
 * @return true
 * @return false
 */
bool WireNode::isNewLayer() { return is_new_layer_; }

/**
 * @brief get new layer type
 *
 * @return true
 * @return false
 */
bool WireNode::isStartNode() { return is_start_node_; }

/**
 * @brief set new layer type
 *
 * @param is_new_layer
 */
void WireNode::setIsNewLayer(Bits is_new_layer) {
    is_new_layer_ = is_new_layer;
}

/**
 * @brief set start node type
 *
 * @param is_start_node
 */
void WireNode::setIsStartNode(Bits is_start_node) {
    is_start_node_ = is_start_node;
}

/**
 * @brief Taper checker
 *
 * @return true
 * @return false
 */
bool WireNode::hasTaper() { return has_tapper_; }

/**
 * @brief set Taper status
 *
 * @param has_taper
 */
void WireNode::setHasTaper(Bits has_taper) { has_tapper_ = has_taper; }

/**
 * @brief Taper rule checker
 *
 * @return true
 * @return false
 */
bool WireNode::hasTaperRule() { return has_tapper_rule_; }

/**
 * @brief Taper rule setting
 *
 * @param has_taper_rule
 */
void WireNode::setHasTaperRule(Bits has_taper_rule) {
    has_tapper_rule_ = has_taper_rule;
}

/**
 * @brief Get taper rule
 *
 * @return const char*
 */
const char* WireNode::getTaperRule() {
    return getTopCell()->getSymbolByIndex(taper_rule_index_).c_str();
}

/**
 * @brief Set taper rule
 *
 * @param taper_rule
 */
bool WireNode::setTaperRule(std::string const& taper_rule) {
    int64_t index = getTopCell()->getOrCreateSymbol(taper_rule.c_str());
    if (index == -1) return false;

    taper_rule_index_ = index;
    getTopCell()->addSymbolReference(taper_rule_index_, this->getId());
    return true;
}

/**
 * @brief Get style
 *
 * @return Bits
 */
Bits WireNode::getStyle() { return style_; }

/**
 * @brief Set style
 *
 * @param style
 */
void WireNode::setStyle(Bits style) { style_ = style; }

/**
 * @brief Print wire node
 *
 */
void WireNode::print() {
    if (getIsVirtul()) message->info("VIRTUAL ");
    if (getMask()) message->info("MASK %d ", mask_);
    message->info("( %d %d ", x_, y_);
    if (extension_) {
        message->info("%d ) ", extension_);
    } else {
        message->info(" ) ");
    }
    if (getIsVia()) message->info("%s ", getViaName().c_str());
    if (patchs_) {
        ArrayObject<ObjectId>* patch_vector =
            addr<ArrayObject<ObjectId>>(patchs_);
        for (ArrayObject<ObjectId>::iterator iter = patch_vector->begin();
             iter != patch_vector->end(); ++iter) {
            ObjectId id = (*iter);
            WirePatch* patch = addr<WirePatch>(id);
            patch->print();
        }
    }
}

/**
 * @brief Print wire node
 *
 */
void WireNode::printDEF(FILE* fp) {
    Tech* lib = getTopCell()->getTechLib();
    Layer* layer = nullptr;
    if (is_start_node_) {
        if (isNewLayer()) {
            fprintf(fp, "\n    NEW ");
        }
        layer = lib->getLayer(getZ());
        if (layer) fprintf(fp, "%s ", layer->getName());
        if (hasTaper()) fprintf(fp, "TAPER ");
        if (hasTaperRule()) fprintf(fp, "TAPERRULE %s ", getTaperRule());
        if (getStyle()) fprintf(fp, "STYLE %d ", style_);
    }
    if (getIsVirtul()) fprintf(fp, "VIRTUAL ");
    if (getMask()) fprintf(fp, "MASK %d ", mask_);
    fprintf(fp, "( %d %d ", x_, y_);
    if (extension_) {
        fprintf(fp, "%d ) ", extension_);
    } else {
        fprintf(fp, ") ");
    }
    if (getIsVia()) fprintf(fp, "%s ", getViaName().c_str());
    if (patchs_) {
        ArrayObject<ObjectId>* patch_vector =
            addr<ArrayObject<ObjectId>>(patchs_);
        for (ArrayObject<ObjectId>::iterator iter = patch_vector->begin();
             iter != patch_vector->end(); ++iter) {
            ObjectId id = (*iter);
            WirePatch* patch = addr<WirePatch>(id);
            patch->printDEF(fp);
        }
    }
}

/**
 * @brief Construct a new Wire:: Wire object
 *
 * @param tail
 * @param head
 */
Wire::Wire(ObjectId tail, ObjectId head) {
    head_ = head;
    tail_ = tail;
}

/**
 * @brief Set the Net object
 *
 * @param net
 */
void Wire::setNet(ObjectId net) { net_ = net; }

/**
 * @brief Set the Net object
 *
 * @param net
 */
void Wire::setNet(Net* net) { net_ = net->getId(); }

/**
 * @brief set head node
 *
 * @param head
 */
void Wire::setHead(WireNode* head) { head_ = head->getId(); }

/**
 * @brief set tail node
 *
 * @param tail
 */
void Wire::setTail(WireNode* tail) { tail_ = tail->getId(); }

/**
 * @brief Construct a new Wire Graph:: Wire Graph object
 *
 */
WireGraph::WireGraph() {
    status_ = 0;
    nodes_ = 0;
    edges_ = 0;
}

/**
 * @brief creat a wire node in graph
 *
 * @param x
 * @param y
 * @param z
 * @return WireNode*
 */
WireNode* WireGraph::creatWireNode(int x, int y, int z) {
    WireNode* node = getTopCell()->createObject<WireNode>(kObjectTypeWire);
    node->setX(x);
    node->setY(y);
    node->setZ(z);
    node->setOwner(this);
    return node;
}

/**
 * @brief add wire node to wire graph
 *
 * @param node
 */
void WireGraph::addWireNode(WireNode* node) {
    ArrayObject<ObjectId>* node_vector = nullptr;
    if (nodes_ == 0) {
        node_vector = getTopCell()->createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        node_vector->setPool(getTopCell()->getPool());
        node_vector->reserve(256);
        nodes_ = node_vector->getId();
    }
    if (nodes_) node_vector = addr<ArrayObject<ObjectId>>(nodes_);
    if (node_vector) node_vector->pushBack(node->getId());
}

/**
 * @brief creat a wire edge
 *
 * @param tail
 * @param head
 * @return Wire*
 */
Wire* WireGraph::creatWireEdge(WireNode* tail, WireNode* head) {
    Wire* edge = getTopCell()->createObject<Wire>(kObjectTypeWire);
    edge->setHead(head);
    edge->setTail(tail);
    edge->setOwner(this);

    return edge;
}

/**
 * @brief add wire edge to graph
 *
 * @param edge
 */
void WireGraph::addWireEdge(Wire* edge) {
    ArrayObject<ObjectId>* edge_vector = nullptr;
    if (edges_ == 0) {
        edge_vector = getTopCell()->createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        edge_vector->setPool(getTopCell()->getPool());
        edge_vector->reserve(256);      
        edges_ = edge_vector->getId();
    }
    if (edges_) edge_vector = addr<ArrayObject<ObjectId>>(edges_);
    if (edge_vector) edge_vector->pushBack(edge->getId());
}

/**
 * @brief get wire routing status
 *
 * @return Bits
 */
Bits WireGraph::getStatus() { return status_; }

/**
 * @brief set wire routing status
 *
 * @param status
 */
void WireGraph::setStatus(int status) { status_ = status; }

/**
 * @brief Print wire graph
 *
 */
void WireGraph::print() {
    Tech* lib = getTopCell()->getTechLib();
    Layer* layer = nullptr;
    WireNode* first_node = nullptr;
    if (nodes_) {
        ArrayObject<ObjectId>* node_vector =
            addr<ArrayObject<ObjectId>>(nodes_);
        if (node_vector) {
            ArrayObject<ObjectId>::iterator iter = node_vector->begin();
            ObjectId id = (*iter);
            if (id) first_node = addr<WireNode>(id);
        }
    }

    if (first_node) layer = lib->getLayer(first_node->getZ());
    if (layer) message->info("%s ", layer->getName());
    /*if (hasTaper()) message->info("TAPER ");
    if (hasTaperRule()) message->info("TAPERRULE %s", getTaperRule());
    if (getStyle()) message->info("STYLE %d ", style_);
*/
    if (nodes_) {
        ArrayObject<ObjectId>* node_vector =
            addr<ArrayObject<ObjectId>>(nodes_);
        for (ArrayObject<ObjectId>::iterator iter = node_vector->begin();
             iter != node_vector->end(); ++iter) {
            WireNode* node = nullptr;
            ObjectId id = (*iter);
            if (id) node = addr<WireNode>(id);
            if (node) node->print();
        }
    }
}

/**
 * @brief Print wire graph
 *
 */
void WireGraph::printDEF(FILE* fp) {
    Tech* lib = getTopCell()->getTechLib();
    Layer* layer = nullptr;
    WireNode* first_node = nullptr;
    if (nodes_) {
        ArrayObject<ObjectId>* node_vector =
            addr<ArrayObject<ObjectId>>(nodes_);
        if (node_vector) {
            ArrayObject<ObjectId>::iterator iter = node_vector->begin();
            ObjectId id = (*iter);
            if (id) first_node = addr<WireNode>(id);
        }
    }

    if (status_) {
        switch (status_) {
            case 1:
                fprintf(fp, "\n  + COVER ");
                break;
            case 2:
                fprintf(fp, "\n  + FIXED ");
                break;
            case 3:
                fprintf(fp, "\n  + ROUTED ");
                break;
            case 4:
                fprintf(fp, "\n  + NOSHIELD ");
                break;

            default:
                fprintf(fp, "\n");
                break;
        }
    }

    if (nodes_) {
        ArrayObject<ObjectId>* node_vector =
            addr<ArrayObject<ObjectId>>(nodes_);
        for (ArrayObject<ObjectId>::iterator iter = node_vector->begin();
             iter != node_vector->end(); ++iter) {
            WireNode* node = nullptr;
            ObjectId id = (*iter);
            if (id) node = addr<WireNode>(id);
            if (node) node->printDEF(fp);
        }
    }
}

}  // namespace db
}  // namespace open_edi
