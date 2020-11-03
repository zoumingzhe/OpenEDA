/* @file  special_wire.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/special_wire.h"

#include "db/core/db.h"
#include "db/tech/tech.h"
#include "db/util/vector_object_var.h"
namespace open_edi {
namespace db {

/**
 * @brief Get the Status object
 *
 * @return Bits
 */
Bits SpecialWire::getStatus() const { return status_; }

/**
 * @brief Set the Status object
 *
 * @param status
 */
void SpecialWire::setStatus(Bits status) { status_ = status; }

/**
 * @brief Get the Mask object
 *
 * @return Bits
 */
Bits SpecialWire::getMask() const { return mask_; }

/**
 * @brief Set the Mask object
 *
 * @param mask
 */
void SpecialWire::setMask(Bits mask) { mask_ = mask; }

/**
 * @brief check the special SpecialWireEdge whether is via
 *
 * @return Bits
 */
Bits SpecialWire::isVia() const { return is_via_; }

/**
 * @brief Set the SpecialWireEdge to Via object
 *
 */
void SpecialWire::setIsVia(Bits is_via) { is_via_ = is_via; }

/**
 * @brief Get the Shape Name object
 *
 * @return const char*
 */
const char* SpecialWire::getShapeName() const { return shape_type_.c_str(); }

/**
 * @brief Set the Shape Name object
 *
 * @param shape
 */
void SpecialWire::setShapeName(char* shape) { shape_type_ = shape; }

/**
 * @brief Get the Box object
 *
 * @return Box
 */
Box SpecialWire::getBox() const { return bbox_; }

/**
 * @brief Set the Box object
 *
 * @param box
 */
void SpecialWire::setBox(Box box) { bbox_ = box; }

/**
 * @brief Get the Via object
 *
 * @return Via*
 */
Via* SpecialWire::getVia() const { return via_; }

/**
 * @brief Set the Via object
 *
 * @param via
 */
void SpecialWire::setVia(Via* via) { via_ = via; }

/**
 * @brief add via point to list
 *
 * @param point
 */
void SpecialWire::addViaPoint(Point point) { via_piont_list_.push_back(point); }

/**
 * @brief print SpecialWire data
 *
 */
void SpecialWire::print() {
    if (getStatus()) message->info("+%d ", getStatus());
    if (getShapeName()) message->info("SHAPE %s ", getShapeName());
    if (mask_) message->info("+MASK %s \n", mask_);
    if (is_via_) {
        ViaMaster* via_master = via_->getMaster();
        if (via_master)
            message->info("+VIA %s ", via_master->getName().c_str());
        for (auto iter = via_piont_list_.begin(); iter < via_piont_list_.end();
             iter++) {
            message->info("( %d %d )", (*iter).getX(), (*iter).getY());
        }
    }
}

/**
 * @brief print SpecialWire data
 *
 */
void SpecialWire::printDEF(FILE* fp) {
    if (getStatus()) fprintf(fp, "+%d ", getStatus());
    if (getShapeName()) fprintf(fp, "SHAPE %s ", getShapeName());
    if (mask_) fprintf(fp, "+MASK %s \n", mask_);
    if (is_via_) {
        ViaMaster* via_master = via_->getMaster();
        if (via_master) fprintf(fp, "+VIA %s ", via_master->getName().c_str());
        for (auto iter = via_piont_list_.begin(); iter < via_piont_list_.end();
             iter++) {
            fprintf(fp, "( %d %d )", (*iter).getX(), (*iter).getY());
        }
    }
}

/**
 * @brief Construct a new SpecialWireEdge Node:: SpecialWireEdge Node object
 *
 * @param x
 * @param y
 * @param z
 */
SpecialWireNode::SpecialWireNode(int x, int y, int z) {
    x_ = x;
    y_ = y;
    z_ = z;
    extension_ = 0;
    mask_ = 0;
    is_via_ = 0;
    via_ = nullptr;
}

/**
 * @brief Set X
 *
 * @param x
 */
void SpecialWireNode::setX(int x) { x_ = x; }

/**
 * @brief Set Y
 *
 * @param y
 */
void SpecialWireNode::setY(int y) { y_ = y; }

/**
 * @brief Set Z
 *
 * @param z
 */
void SpecialWireNode::setZ(int z) { z_ = z; }

/**
 * @brief Get X
 *
 * @return int
 */
int SpecialWireNode::getX() { return x_; }

/**
 * @brief Get Y
 *
 * @return int
 */
int SpecialWireNode::getY() { return y_; }

/**
 * @brief Get Z
 *
 * @return int
 */
int SpecialWireNode::getZ() { return z_; }

/**
 * @brief Get mask
 *
 * @return Bits
 */
Bits SpecialWireNode::getMask() const { return mask_; }

/**
 * @brief Set mask
 *
 * @param mask
 */
void SpecialWireNode::setMask(Bits mask) { mask_ = mask; }

/**
 * @brief Get extension
 *
 * @return int
 */
int SpecialWireNode::getExtension() const { return extension_; }

/**
 * @brief Set extension
 *
 * @param extension
 */
void SpecialWireNode::setExtension(int extension) { extension_ = extension; }

/**
 * @brief Add out edge of SpecialWireEdge node
 *
 * @param out
 */
void SpecialWireNode::addOutEdgeList(SpecialWireEdge* out) {
    out_edge_list_.push_back(out);
}

/**
 * @brief Set the Via object
 *
 * @param via
 */
void SpecialWireNode::setVia(Via* via) { via_ = via; }

/**
 * @brief Get the Via object
 *
 * @return Via*
 */
Via* SpecialWireNode::getVia() const { return via_; }

/**
 * @brief Get SpecialWireEdge node type
 *
 * @return Bits
 */
Bits SpecialWireNode::getIsVia() { return is_via_; }

/**
 * @brief Set SpecialWireEdge node type
 *
 * @param is_via
 */
void SpecialWireNode::setIsVia(Bits is_via) { is_via_ = is_via; }

/**
 * @brief Print SpecialWireEdge node
 *
 * ( x y [extValue])
 * { [MASK maskNum] ( x y [extValue])
 * | [MASK viaMaskNum] viaName [orient]
 * [DO numX BY numY STEP stepX stepY]
 * } ...
 *
 */
void SpecialWireNode::print() {
    if (getMask()) message->info("MASK %d ", mask_);
    message->info("( %d %d ", x_, y_);
    if (extension_) {
        message->info("%d ) ", extension_);
    } else {
        message->info(" ) ");
    }
    if (getIsVia() && via_ && via_->getMaster()) {
        message->info("%s ", via_->getMaster()->getName().c_str());
        int orient = via_->getOrient();
        switch (orient) {
            case 0:
                break;
            case 1:
                message->info("W ");
                break;
            case 2:
                message->info("S ");
                break;
            case 3:
                message->info("E ");
                break;
            case 4:
                message->info("FN ");
                break;
            case 5:
                message->info("FW ");
                break;
            case 6:
                message->info("FS ");
                break;
            case 7:
                message->info("FE ");
            default:
                break;
        }

        if (via_->getIsArray())
            message->info("DO %d BY %d STEP %d %d ", via_->getCol(),
                          via_->getRow(), via_->getSpaceX(), via_->getSpaceY());
    }
}

/**
 * @brief Print SpecialWireEdge node
 *
 * ( x y [extValue])
 * { [MASK maskNum] ( x y [extValue])
 * | [MASK viaMaskNum] viaName [orient]
 * [DO numX BY numY STEP stepX stepY]
 * } ...
 *
 */
void SpecialWireNode::printDEF(FILE* fp) {
    if (getMask()) fprintf(fp, "MASK %d ", mask_);
    fprintf(fp, "( %d %d", x_, y_);
    if (extension_) {
        fprintf(fp, " %d ) ", extension_);
    } else {
        fprintf(fp, " ) ");
    }
    if (getIsVia() && via_ && via_->getMaster()) {
        fprintf(fp, "%s ", via_->getMaster()->getName().c_str());
        int orient = via_->getOrient();
        switch (orient) {
            case 0:
                break;
            case 1:
                fprintf(fp, "W ");
                break;
            case 2:
                fprintf(fp, "S ");
                break;
            case 3:
                fprintf(fp, "E ");
                break;
            case 4:
                fprintf(fp, "FN ");
                break;
            case 5:
                fprintf(fp, "FW ");
                break;
            case 6:
                fprintf(fp, "FS ");
                break;
            case 7:
                fprintf(fp, "FE ");
            default:
                break;
        }

        if (via_->getIsArray())
            fprintf(fp, "DO %d BY %d STEP %d %d ", via_->getCol(),
                    via_->getRow(), via_->getSpaceX(), via_->getSpaceY());
    }
}

/**
 * @brief Construct a new Special Wire Edge:: Special Wire Edge object
 *
 */
SpecialWireEdge::SpecialWireEdge() {
    head_ = 0;
    tail_ = 0;
}
/**
 * @brief Construct a new Special Wire Edge:: Special Wire Edge object
 *
 * @param head
 * @param tail
 */
SpecialWireEdge::SpecialWireEdge(SpecialWireNode* head, SpecialWireNode* tail) {
    head_ = head->getId();
    tail_ = tail->getId();
}

/**
 * @brief set head node
 *
 * @param head
 */
void SpecialWireEdge::setHead(SpecialWireNode* head) {
    if (head) head_ = head->getId();
}

/**
 * @brief set tail node
 *
 * @param tail
 */
void SpecialWireEdge::setTail(SpecialWireNode* tail) {
    if (tail) tail_ = tail->getId();
}

/**
 * @brief Construct a new Special Wire Graph:: Special Wire Graph object
 *
 */
SpecialWireGraph::SpecialWireGraph() {
    is_new_layer_ = 0;
    shape_ = 0;
    style_ = 0;
    width_ = 0;
    nodes_ = 0;
}

/**
 * @brief get new layer type
 *
 * @return true
 * @return false
 */
bool SpecialWireGraph::isNewLayer() { return is_new_layer_; }

/**
 * @brief set new layer type
 *
 * @param is_new_layer
 */
void SpecialWireGraph::setIsNewLayer(Bits is_new_layer) {
    is_new_layer_ = is_new_layer;
}

/**
 * @brief Get the Width object
 *
 * @return int
 */
int SpecialWireGraph::getWidth() const { return width_; }

/**
 * @brief Set the Width object
 *
 * @param width
 */
void SpecialWireGraph::setWidth(int width) { width_ = width; }

/**
 * @brief Get the Shape object
 *
 * @return Bits
 */
Bits SpecialWireGraph::getShape() const { return shape_; }

/**
 * @brief Set the Shape object
 *
 * @param shape
 */
void SpecialWireGraph::setShape(Bits shape) { shape_ = shape; }

/**
 * @brief Get style
 *
 * @return Bits
 */
Bits SpecialWireGraph::getStyle() const { return style_; }

/**
 * @brief Set style
 *
 * @param style
 */
void SpecialWireGraph::setStyle(Bits style) { style_ = style; }

/**
 * @brief create a node in graph
 *
 * @return SpecialWireNode*
 */
SpecialWireNode* SpecialWireGraph::createSpecialWireNode() {
    SpecialWireNode* node =
        getTopCell()->createObject<SpecialWireNode>(kObjectTypeWire);
    node->setX(0);
    node->setY(0);
    node->setZ(0);
    node->setExtension(0);
    node->setMask(0);
    node->setIsVia(0);
    node->setVia(nullptr);
    return node;
}

/**
 * @brief create a edge in graph
 *
 * @param tail
 * @param head
 * @return SpecialWireEdge*
 */
SpecialWireEdge* SpecialWireGraph::createSpecialWireEdge(
    SpecialWireNode* tail, SpecialWireNode* head) {
    SpecialWireEdge* edge =
        getTopCell()->createObject<SpecialWireEdge>(kObjectTypeWire);
    edge->setHead(head);
    edge->setTail(tail);
    return edge;
}

/**
 * @brief create a node in graph
 *
 * @param x
 * @param y
 * @param z
 * @return SpecialWireNode*
 */
SpecialWireNode* SpecialWireGraph::createSpecialWireNode(int x, int y, int z) {
    SpecialWireNode* node =
        getTopCell()->createObject<SpecialWireNode>(kObjectTypeWire);
    node->setX(x);
    node->setY(y);
    node->setZ(z);
    node->setExtension(0);
    node->setIsVia(0);
    node->setVia(nullptr);

    return node;
}

/**
 * @brief add edge to graph
 *
 * @param edge
 * @return int
 */
int SpecialWireGraph::addWireEdge(SpecialWireEdge* edge) {
    if (edges_ == 0) {
        edges_ = getTopCell()->createVectorObject<VectorObject64>()->getId();
    }
    VectorObject64* edge_vector = addr<VectorObject64>(edges_);
    edge_vector->push_back(edge->getId());
    return 0;
}

/**
 * @brief add node to graph
 *
 * @param node
 * @return int
 */
int SpecialWireGraph::addWireNode(SpecialWireNode* node) {
    if (nodes_ == 0) {
        nodes_ = getTopCell()->createVectorObject<VectorObject64>()->getId();
    }
    VectorObject64* node_vector = addr<VectorObject64>(nodes_);
    node_vector->push_back(node->getId());
    return 0;
}

/**
 * @brief Print SpecialWireEdge graph
 *
 */
void SpecialWireGraph::print() {
    Tech* lib = getTopCell()->getTechLib();
    Layer* layer = nullptr;
    SpecialWireNode* first_node = nullptr;
    if (nodes_) {
        VectorObject64* node_vector =
            addr<VectorObject64>(nodes_);
        auto iter = node_vector->begin();
        ObjectId id = (*iter);
        if (id) first_node = addr<SpecialWireNode>(id);
    }
    if (isNewLayer()) {
        message->info("\n   NEW");
    } else {
        message->info("\n   ");
    }
    if (first_node) layer = lib->getLayer(first_node->getZ());
    if (layer) {
        message->info(" %s ", layer->getName());
    } else {
        message->info(" LAYER_UNKOWN ");
    }

    if (width_) message->info("%d ", width_);

    if (shape_) {
        message->info(" + SHAPE ");
        switch (shape_) {
            case 1:
                message->info("RING ");
                break;
            case 2:
                message->info("PADRING ");
                break;
            case 3:
                message->info("BLOCKRING ");
                break;
            case 4:
                message->info("STRIPE ");
                break;
            case 5:
                message->info("FOLLOWPIN ");
                break;
            case 6:
                message->info("IOWIRE ");
                break;
            case 7:
                message->info("COREWIRE ");
                break;
            case 8:
                message->info("BLOCKWIRE ");
                break;
            case 9:
                message->info("BLOCKAGEWIRE ");
                break;
            case 10:
                message->info("FILLWIRE ");
                break;
            case 11:
                message->info("FILLWIREOPC ");
                break;
            case 12:
                message->info("DRCFILL ");
                break;
            default:
                break;
        }
    }

    if (getStyle()) message->info(" + STYLE %d ", style_);

    if (nodes_) {
        VectorObject64* node_vector =
            addr<VectorObject64>(nodes_);
        for (VectorObject64::iterator iter = node_vector->begin();
             iter != node_vector->end(); ++iter) {
            SpecialWireNode* node = nullptr;
            ObjectId id = (*iter);
            if (id) node = addr<SpecialWireNode>(id);
            if (node) node->print();
        }
    }
}

/**
 * @brief Print SpecialWireEdge graph
 *
 */
void SpecialWireGraph::printDEF(FILE* fp) {
    Tech* lib = getTopCell()->getTechLib();
    Layer* layer = nullptr;
    SpecialWireNode* first_node = nullptr;
    if (nodes_) {
        VectorObject64* node_vector =
            addr<VectorObject64>(nodes_);
        auto iter = node_vector->begin();
        ObjectId id = (*iter);
        if (id) first_node = addr<SpecialWireNode>(id);
    }
    if (isNewLayer()) {
        fprintf(fp, "\n   NEW");
    } else {
        fprintf(fp, " ");
    }
    if (first_node) layer = lib->getLayer(first_node->getZ());
    if (layer) {
        fprintf(fp, " %s ", layer->getName());
    } else {
        fprintf(fp, " LAYER_UNKOWN ");
    }

    if (width_) fprintf(fp, "%d ", width_);

    if (shape_) {
        fprintf(fp, " + SHAPE ");
        switch (shape_) {
            case 1:
                fprintf(fp, "RING ");
                break;
            case 2:
                fprintf(fp, "PADRING ");
                break;
            case 3:
                fprintf(fp, "BLOCKRING ");
                break;
            case 4:
                fprintf(fp, "STRIPE ");
                break;
            case 5:
                fprintf(fp, "FOLLOWPIN ");
                break;
            case 6:
                fprintf(fp, "IOWIRE ");
                break;
            case 7:
                fprintf(fp, "COREWIRE ");
                break;
            case 8:
                fprintf(fp, "BLOCKWIRE ");
                break;
            case 9:
                fprintf(fp, "BLOCKAGEWIRE ");
                break;
            case 10:
                fprintf(fp, "FILLWIRE ");
                break;
            case 11:
                fprintf(fp, "FILLWIREOPC ");
                break;
            case 12:
                fprintf(fp, "DRCFILL ");
                break;
            default:
                break;
        }
    }

    if (getStyle()) fprintf(fp, " + STYLE %d ", style_);

    if (nodes_) {
        VectorObject64* node_vector =
            addr<VectorObject64>(nodes_);
        for (VectorObject64::iterator iter = node_vector->begin();
             iter != node_vector->end(); ++iter) {
            SpecialWireNode* node = nullptr;
            ObjectId id = (*iter);
            if (id) node = addr<SpecialWireNode>(id);
            if (node) node->printDEF(fp);
        }
    }
}

/**
 * @brief Construct a new Special Wire Section:: Special Wire Section object
 *
 */
SpecialWireSection::SpecialWireSection() {
    graphs_ = 0;
    status_ = 0;
    shield_net_id_ = 0;
}

/**
 * @brief get wire routing status
 *
 * @return Bits
 */
Bits SpecialWireSection::getStatus() { return status_; }

/**
 * @brief set wire routing status
 *
 * @param status
 */
void SpecialWireSection::setStatus(int status) { status_ = status; }

/**
 * @brief set shield net
 *
 * @param net name
 */
void SpecialWireSection::setShieldNetName(const char* net_name) {
    Cell* top_cell = getTopCell();
    shield_net_name_ = net_name;
    Net* net = top_cell->getNet(shield_net_name_);
    if (!net) {
        // message->issueMsg(kWarn, "cannot find shieled net %s when reading
        // special net of DEF.\n", net_name);
    } else {
        shield_net_id_ = net->getId();
    }
    return;
}
/**
 * @brief get shield net
 *
 * @param
 */
Net* SpecialWireSection::getShieldNet() {
    return addr<Net>(shield_net_id_);
}
/**
 * @brief get shield net name
 *
 * @param
 */
std::string SpecialWireSection::getShieldNetName() { return shield_net_name_; }

/**
 * @brief create graph
 *
 * @return SpecialWireGraph*
 */
SpecialWireGraph* SpecialWireSection::creatGraph() {
    SpecialWireGraph* graph =
        getTopCell()->createObject<SpecialWireGraph>(kObjectTypeSpecialWire);

    return graph;
}

/**
 * @brief add graph to section
 *
 * @param graph
 * @return int
 */
int SpecialWireSection::addGraph(SpecialWireGraph* graph) {
    VectorObject64* graph_vector = nullptr;
    if (graphs_ == 0) {
        graphs_ = getTopCell()->createVectorObject<VectorObject64>()->getId();
    }
    if (graphs_) graph_vector = addr<VectorObject64>(graphs_);
    if (graph_vector) graph_vector->push_back(graph->getId());

    return 0;
}

/**
 * @brief print out
 *
 */
void SpecialWireSection::print() {
    if (graphs_) {
        VectorObject64* graph_vector =
            addr<VectorObject64>(graphs_);
        for (VectorObject64::iterator iter = graph_vector->begin();
             iter != graph_vector->end(); ++iter) {
            ObjectId id = (*iter);
            SpecialWireGraph* graph =
                addr<SpecialWireGraph>(id);
            graph->print();
        }
    }
}

/**
 * @brief print out
 *
 */
void SpecialWireSection::printDEF(FILE* fp) {
    if (status_) {
        switch (status_) {
            case 1:
                fprintf(fp, "\n  + COVER");
                break;
            case 2:
                fprintf(fp, "\n  + FIXED");
                break;
            case 3:
                fprintf(fp, "\n  + ROUTED");
                break;
            case 4:
                fprintf(fp, "\n  + SHIELD %s", getShieldNetName().c_str());
                break;

            default:
                fprintf(fp, "\n");
                break;
        }
    }
    if (graphs_) {
        VectorObject64* graph_vector =
            addr<VectorObject64>(graphs_);
        for (VectorObject64::iterator iter = graph_vector->begin();
             iter != graph_vector->end(); ++iter) {
            ObjectId id = (*iter);
            SpecialWireGraph* graph =
                addr<SpecialWireGraph>(id);
            graph->printDEF(fp);
        }
    }
}

}  // namespace db
}  // namespace open_edi
