/* @file  via_master.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/via_master.h"

#include "db/core/db.h"
#include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {

/**
 * @brief Construct a new Via Layer:: Via Layer object
 *
 */
ViaLayer::ViaLayer() { name_index_ = 0; }

/**
 * @brief Get the Name Index object
 *
 * @return SymbolIndex
 */
SymbolIndex ViaLayer::getNameIndex() const { return name_index_; }

/**
 * @brief Set the Name object
 *
 * @param name
 */
bool ViaLayer::setName(std::string const& name) {
    int64_t index = getTopCell()->getOrCreateSymbol(name.c_str());
    if (index == -1) return false;

    name_index_ = index;
    getTopCell()->addSymbolReference(name_index_, this->getId());
    return true;
}
/**
 * @brief Get the Name object
 *
 * @return const char*
 */
std::string const& ViaLayer::getName() {
    return getTopCell()->getSymbolByIndex(name_index_);
}

/**
 * @brief add mask
 *
 * @param num
 */
void ViaLayer::addMask(int num) { masks_.push_back(num); }

/**
 * @brief Get the Mask Num object
 *
 * @param num
 * @return int
 */
int ViaLayer::getMaskNum(int num) { return masks_[num]; }

/**
 * @brief add rect
 *
 * @param rect
 */
void ViaLayer::addRect(Box* rect) { rects_.push_back(rect); }

/**
 * @brief Get the Rect object
 *
 * @param num
 * @return Box*
 */
Box* ViaLayer::getRect(int num) { return rects_[num]; }

/**
 * @brief Get the Rects object
 *
 * @return std::vector<Box*>
 */
std::vector<Box*> ViaLayer::getRects() { return rects_; }

/**
 * @brief print out
 *
 * @param is_def
 */
void ViaLayer::print(int is_def) {
    if (!is_def) {
        message->info("   LAYER %s ;\n", getName().c_str());
        std::vector<Box*> rects = getRects();
        for (int i = 0; i < rects.size(); ++i) {
            Box* box = getRect(i);
            message->info("      RECT  %d %d %d %d ;\n", box->getLLX(),
                          box->getLLY(), box->getURX(), box->getURY());
        }
    } else {
        message->info("   + RECT %s ", getName().c_str());
        std::vector<Box*> rects = getRects();
        for (int i = 0; i < rects.size(); ++i) {
            Box* box = getRect(i);
            if (getMaskNum(i)) {
                message->info(" + MASK  %d ", getMaskNum(i));
            }
            message->info(" ( %d %d ) ( %d %d ) \n", box->getLLX(),
                          box->getLLY(), box->getURX(), box->getURY());
        }
    }
}

void ViaLayer::printLEF(std::ofstream& ofs, uint32_t num_spaces) {
    Tech* lib = getTopCell()->getTechLib();
    std::string space_str = getSpaceStr(num_spaces);

    ofs << space_str << "   LAYER " << getName().c_str() << " ;\n";
    std::vector<Box*> rects = getRects();
    for (int i = 0; i < rects.size(); ++i) {
        Box* box = getRect(i);
        ofs << space_str << "      RECT  " << lib->dbuToMicrons(box->getLLX())
            << " " << lib->dbuToMicrons(box->getLLY()) << " "
            << lib->dbuToMicrons(box->getURX()) << " "
            << lib->dbuToMicrons(box->getURY()) << " ;\n";
    }
}

void ViaLayer::printDEF(FILE* fp) {
    fprintf(fp, "\n   + RECT %s ", getName().c_str());
    std::vector<Box*> rects = getRects();
    for (int i = 0; i < rects.size(); ++i) {
        Box* box = getRect(i);
        if (getMaskNum(i)) {
            fprintf(fp, " + MASK  %d ", getMaskNum(i));
        }
        fprintf(fp, " ( %d %d ) ( %d %d )", box->getLLX(), box->getLLY(),
                box->getURX(), box->getURY());
    }
}

/**
 * @brief Construct a new Via Master:: Via Master object
 *
 */
ViaMaster::ViaMaster() {
    name_index_ = 0;
    pattern_index_ = 0;
    is_default_ = 0;
    has_via_rule_ = 0;
    is_array_ = 0;
    is_from_def_ = 0;
    has_origin_ = 0;
    has_offset_ = 0;
    has_cut_pattern_ = 0;
    has_resistance_ = 0;
    x_size_ = 0;
    y_size_ = 0;
    via_rule_ = "";
    lower_layer_ = "";
    cut_layer_ = "";
    upper_layer_ = "";
    x_cut_spacing_ = 0;
    y_cut_spacing_ = 0;
    x_lower_enclosure_ = 0;
    y_lower_enclosure_ = 0;
    x_upper_enclosure_ = 0;
    y_upper_enclosure_ = 0;
    rows_ = 0;
    columns_ = 0;
    x_offset_ = 0;
    y_offset_ = 0;
    x_lower_offset_ = 0;
    y_lower_offset_ = 0;
    x_upper_offset_ = 0;
    y_upper_offset_ = 0;
    cut_patten_index_ = 0;
    resistance_ = 0;
    is_from_ndr_ = 0;
}
/**
 * @brief Get the Name Index object
 *
 * @return SymbolIndex
 */
SymbolIndex ViaMaster::getNameIndex() const { return name_index_; }

/**
 * @brief Get the Name object
 *
 * @return std::string const&
 */
std::string const& ViaMaster::getName() {
    return getTopCell()->getSymbolByIndex(name_index_);
}

/**
 * @brief Set the Name object
 *
 * @param name
 */
bool ViaMaster::setName(std::string const& name) {
    int64_t index = getTopCell()->getOrCreateSymbol(name.c_str());
    if (index == -1) return false;

    name_index_ = index;
    getTopCell()->addSymbolReference(name_index_, this->getId());
    return true;
}

/**
 * @brief Get the Name object
 *
 * @return std::string const&
 */
std::string const& ViaMaster::getPattern() {
    return getTopCell()->getSymbolByIndex(pattern_index_);
}

/**
 * @brief Set the Name object
 *
 * @param name
 */
bool ViaMaster::setPattern(std::string const& pattern) {
    int64_t index = getTopCell()->getOrCreateSymbol(pattern.c_str());
    if (index == -1) return false;

    pattern_index_ = index;
    getTopCell()->addSymbolReference(pattern_index_, this->getId());
    return true;
}
/**
 * @brief Get the Cut Size X object
 *
 * @return int
 */
int ViaMaster::getCutSizeX() const { return x_size_; }

/**
 * @brief Get the Cut Size Y object
 *
 * @return int
 */
int ViaMaster::getCutSizeY() const { return y_size_; }

/**
 * @brief Get the Cut Spacing X object
 *
 * @return int
 */
int ViaMaster::getCutSpacingX() const { return x_cut_spacing_; }

/**
 * @brief Get the Cut Spacing Y object
 *
 * @return int
 */
int ViaMaster::getCutSpacingY() const { return y_cut_spacing_; }

/**
 * @brief Get the Lower Enc X object
 *
 * @return int
 */
int ViaMaster::getLowerEncX() const { return x_lower_enclosure_; }

/**
 * @brief Get the Lower Enc Y object
 *
 * @return int
 */
int ViaMaster::getLowerEncY() const { return y_lower_enclosure_; }

/**
 * @brief Get the Upper Enc X object
 *
 * @return int
 */
int ViaMaster::getUpperEncX() const { return x_upper_enclosure_; }

/**
 * @brief Get the Upper Enc Y object
 *
 * @return int
 */
int ViaMaster::getUpperEncY() const { return y_upper_enclosure_; }

/**
 * @brief Get the Row object
 *
 * @return int
 */
int ViaMaster::getRow() const { return rows_; }

/**
 * @brief Get the Col object
 *
 * @return int
 */
int ViaMaster::getCol() const { return columns_; }

/**
 * @brief Get the Offset X object
 *
 * @return int
 */
int ViaMaster::getOffsetX() const { return x_offset_; }

/**
 * @brief Get the Offset Y object
 *
 * @return int
 */
int ViaMaster::getOffsetY() const { return y_offset_; }

/**
 * @brief Get the Lower Offset X object
 *
 * @return int
 */
int ViaMaster::getLowerOffsetX() const { return x_lower_offset_; }

/**
 * @brief Get the Lower Offset Y object
 *
 * @return int
 */
int ViaMaster::getLowerOffsetY() const { return y_lower_offset_; }

/**
 * @brief Get the Upper Offset X object
 *
 * @return int
 */
int ViaMaster::getUpperOffsetX() const { return x_upper_offset_; }

/**
 * @brief Get the Upper Offset Y object
 *
 * @return int
 */
int ViaMaster::getUpperOffsetY() const { return y_upper_offset_; }

/**
 * @brief judge is array
 *
 * @return true
 * @return false
 */
bool ViaMaster::isArray() { return is_array_; }

/**
 * @brief judge is from def
 *
 * @return true
 * @return false
 */
bool ViaMaster::isFromDEF() { return is_from_def_; }
/**
 * @brief judge is from ndr
 *
 * @return true
 * @return false
 */
bool ViaMaster::isFromNDR() { return is_from_ndr_; }

/**
 * @brief has origin
 *
 * @return true
 * @return false
 */
bool ViaMaster::hasOrigin() { return has_origin_; }

/**
 * @brief has offset
 *
 * @return true
 * @return false
 */
bool ViaMaster::hasOffset() { return has_offset_; }

/**
 * @brief has cut patten
 *
 * @return true
 * @return false
 */
bool ViaMaster::hasCutPatten() { return has_cut_pattern_; }

/**
 * @brief is generated
 *
 * @return true
 * @return false
 */
bool ViaMaster::hasGenerated() { return has_generated_; }

/**
 * @brief has resistance
 *
 * @return true
 * @return false
 */
bool ViaMaster::hasResistance() { return has_resistance_; }

/**
 * @brief Get the Via Layer Vector object
 *
 * @return ObjectId
 */
ObjectId ViaMaster::getViaLayerVector() { return via_layers_; }

/**
 * @brief Set the Cut Size X object
 *
 * @param x
 */
void ViaMaster::setCutSizeX(int x) { x_size_ = x; }

/**
 * @brief Set the Cut Size Y object
 *
 * @param y
 */
void ViaMaster::setCutSizeY(int y) { y_size_ = y; }

/**
 * @brief Set the Cut Spacing X object
 *
 * @param x
 */
void ViaMaster::setCutSpacingX(int x) { x_cut_spacing_ = x; }

/**
 * @brief Set the Cut Spacing Y object
 *
 * @param y
 */
void ViaMaster::setCutSpacingY(int y) { y_cut_spacing_ = y; }

/**
 * @brief Set the Lower Enc X object
 *
 * @param x
 */
void ViaMaster::setLowerEncX(int x) { x_lower_enclosure_ = x; }

/**
 * @brief Set the Lower Enc Y object
 *
 * @param y
 */
void ViaMaster::setLowerEncY(int y) { y_lower_enclosure_ = y; }

/**
 * @brief Set the Upper Enc X object
 *
 * @param x
 */
void ViaMaster::setUpperEncX(int x) { x_upper_enclosure_ = x; }

/**
 * @brief Set the Upper Enc Y object
 *
 * @param y
 */
void ViaMaster::setUpperEncY(int y) { y_upper_enclosure_ = y; }

/**
 * @brief Set the Row object
 *
 * @param rows
 */
void ViaMaster::setRow(int rows) { rows_ = rows; }

/**
 * @brief Set the Col object
 *
 * @param col
 */
void ViaMaster::setCol(int col) { columns_ = col; }

/**
 * @brief Set the Offset X object
 *
 * @param x
 */
void ViaMaster::setOffsetX(int x) { x_offset_ = x; }

/**
 * @brief Set the Offset Y object
 *
 * @param y
 */
void ViaMaster::setOffsetY(int y) { y_offset_ = y; }

/**
 * @brief Set the Lower Offset X object
 *
 * @param x
 */
void ViaMaster::setLowerOffsetX(int x) { x_lower_offset_ = x; }

/**
 * @brief Set the Lower Offset Y object
 *
 * @param y
 */
void ViaMaster::setLowerOffsetY(int y) { y_lower_offset_ = y; }

/**
 * @brief Set the Upper Offset X object
 *
 * @param x
 */
void ViaMaster::setUpperOffsetX(int x) { x_upper_offset_ = x; }

/**
 * @brief Set the Upper Offset Y object
 *
 * @param y
 */
void ViaMaster::setUpperOffsetY(int y) { y_upper_offset_ = y; }

/**
 * @brief Set the Is Array object
 *
 * @param is_array
 */
void ViaMaster::setIsArray(Bits is_array) { is_array_ = is_array; }

/**
 * @brief Set the Is From D E F object
 *
 * @param is_from_def
 */
void ViaMaster::setIsFromDEF(Bits is_from_def) { is_from_def_ = is_from_def; }

/**
 * @brief Set the Is From ndr object
 *
 * @param is_from_def
 */
void ViaMaster::setIsFromNDR(Bits is_from_ndr) { is_from_ndr_ = is_from_ndr; }

/**
 * @brief Set the Has Origin object
 *
 * @param has_origin
 */
void ViaMaster::setHasOrigin(int has_origin) {
    if (has_origin > 0) has_origin_ = 1;
}

/**
 * @brief Set the Has Offset object
 *
 * @param has_offset
 */
void ViaMaster::setHasOffset(int has_offset) {
    if (has_offset > 0) has_offset_ = 1;
}

/**
 * @brief set has Generated
 *
 * @param has_generated
 */
void ViaMaster::sethasGenerated(Bits has_generated) {
    has_generated_ = has_generated;
}

/**
 * @brief Set the Has Resistance object
 *
 * @param has_resistance
 */
void ViaMaster::setHasResistance(Bits has_resistance) {
    has_resistance_ = has_resistance;
}

/**
 * @brief Set the Has Cut Patten object
 *
 * @param has_cut_pattern
 */
void ViaMaster::setHasCutPatten(Bits has_cut_pattern) {
    has_cut_pattern_ = has_cut_pattern;
}

/**
 * @brief Get the Name object
 *
 * @return std::string const&
 */
std::string const& ViaMaster::getCutPatten() {
    return getTopCell()->getSymbolByIndex(cut_patten_index_);
}

/**
 * @brief Set the Name object
 *
 * @param name
 */
bool ViaMaster::setCutPatten(std::string const& cut_pattern) {
    int64_t index = getTopCell()->getOrCreateSymbol(cut_pattern.c_str());
    if (index == -1) return false;

    cut_patten_index_ = index;
    getTopCell()->addSymbolReference(cut_patten_index_, this->getId());
    return true;
}
#ifdef USE_SYMBOL_TABLE_
/**
 * @brief Get the Rule Index object
 *
 * @return ObjectIndex
 */
ObjectIndex ViaMaster::getViaRule() const { return rule_; }

/**
 * @brief Set the Rule Index object
 *
 * @param index
 */
void ViaMaster::setViaRule(ObjectIndex index) { rule_ = index; }

/**
 * @brief Get the Lower Layer Index object
 *
 * @return ObjectIndex
 */
ObjectIndex ViaMaster::getLowerLayerIndex() const { return lower_layer_; }

/**
 * @brief Set the Lower Layer Index object
 *
 * @param index
 */
void ViaMaster::setLowerLayerIndex(ObjectIndex index) { lower_layer_ = index; }

/**
 * @brief Get the Cut Layer Index object
 *
 * @return ObjectIndex
 */
ObjectIndex ViaMaster::getCutLayerIndex() const { return cut_layer_; }

/**
 * @brief Set the Cut Layer Index object
 *
 * @param index
 */
void ViaMaster::setCutLayerIndex(ObjectIndex index) { cut_layer_ = index; }

/**
 * @brief Get the Uper Layer Index object
 *
 * @return ObjectIndex
 */
ObjectIndex ViaMaster::getUperLayerIndex() const { return upper_layer_; }

/**
 * @brief Set the Uper Layer Index object
 *
 * @param index
 */
void ViaMaster::setUperLayerIndex(ObjectIndex index) { upper_layer_ = index; }
#else
/**
 * @brief Get the Via Rule object
 *
 * @return const char*
 */
const char* ViaMaster::getViaRule() const { return via_rule_.c_str(); }

/**
 * @brief Set the Via Rule object
 *
 * @param via_rule
 */
void ViaMaster::setViaRule(char* via_rule) { via_rule_ = via_rule; }

/**
 * @brief Get the Lower Layer Index object
 *
 * @return ObjectIndex
 */
const char* ViaMaster::getLowerLayerIndex() const {
    return lower_layer_.c_str();
}

/**
 * @brief Set the Lower Layer Index object
 *
 * @param index
 */
void ViaMaster::setLowerLayerIndex(char* index) { lower_layer_ = index; }

/**
 * @brief Get the Cut Layer Index object
 *
 * @return ObjectIndex
 */
const char* ViaMaster::getCutLayerIndex() const { return cut_layer_.c_str(); }

/**
 * @brief Set the Cut Layer Index object
 *
 * @param index
 */
void ViaMaster::setCutLayerIndex(char* index) { cut_layer_ = index; }

/**
 * @brief Get the Uper Layer Index object
 *
 * @return ObjectIndex
 */
const char* ViaMaster::getUperLayerIndex() const {
    return upper_layer_.c_str();
}

/**
 * @brief Set the Uper Layer Index object
 *
 * @param index
 */
void ViaMaster::setUperLayerIndex(char* index) { upper_layer_ = index; }
#endif

/**
 * @brief get resistance
 *
 * @return double
 */
double ViaMaster::getResistance() const { return resistance_; }

/**
 * @brief set resistance
 *
 * @param resistance
 */
void ViaMaster::setResistance(double resistance) { resistance_ = resistance; }

/**
 * @brief Get the viaMaster default status
 *
 * @return true
 * @return false
 */
bool ViaMaster::isDefault() const { return is_default_; }

/**
 * @brief creatViaLayer
 *
 * @param name
 * @return ViaLayer*
 */
ViaLayer* ViaMaster::creatViaLayer(std::string& name) {
    ViaLayer* via_layer =
        getTopCell()->createObject<ViaLayer>(kObjectTypeViaMaster);
    via_layer->setName(name);
    return via_layer;
}

int ViaMaster::addViaLayer(ViaLayer* via_layer) {
    if (via_layers_ == 0) {
        via_layers_ =
            getTopCell()->createVectorObject<VectorObject64>()->getId();
    }
    VectorObject64* via_layer_vector =
        addr<VectorObject64>(via_layers_);
    via_layer_vector->push_back(via_layer->getId());
    return 0;
}

uint64_t ViaMaster::numProperties() const {
    if (!properties_) return 0;

    return addr<VectorObject16>(properties_)->totalSize();
}

void ViaMaster::setPropertySize(uint64_t v) {
    if (v == 0) {
        if (properties_) {
            VectorObject16::deleteDBVectorObjectVar(properties_);
        }
        return;
    }
    if (!properties_) {
        VectorObject16* vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        properties_ = vobj->getId();
    }
}

void ViaMaster::addProperty(ObjectId obj_id) {
    VectorObject16* vobj = nullptr;
    if (obj_id == 0) return;

    if (properties_ == 0) {
        vobj = VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        properties_ = vobj->getId();
    } else {
        vobj = addr<VectorObject16>(properties_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
}

ObjectId ViaMaster::getPropertiesId() const { return properties_; }
/**
 * @brief Set the viaMaster default status
 *
 * @param is_default
 */
void ViaMaster::setDefault(bool is_default) { is_default_ = is_default; }

/**
 * @brief Via Master print out
 *
 */
void ViaMaster::print() {
    if (!is_from_def_) {  // lef output format
        message->info("VIA %s", getName().c_str());

        if (isDefault()) {
            message->info(" DEFAULT");
        }

        if (hasGenerated()) {
            message->info(" GENERATED");
        }

        message->info("\n");

        if (strcmp(getViaRule(), "")) {
            message->info("  VIARULE %s ;\n", getViaRule());
            message->info("    CUTSIZE %d %d ;\n", getCutSizeX(),
                          getCutSizeY());
            message->info("    LAYERS %s %s %s ;\n", getLowerLayerIndex(),
                          getCutLayerIndex(), getUperLayerIndex());
            message->info("    CUTSPACING %d %d ;\n", getCutSpacingX(),
                          getCutSpacingY());
            message->info("    ENCLOSURE %d %d %d %d ;\n", getLowerEncX(),
                          getLowerEncY(), getUpperEncX(), getUpperEncY());
            if (isArray()) {
                message->info("    ROWCOL %d %d ;\n", getRow(), getCol());
            }

            if (hasOrigin()) {
                message->info("    ORIGIN %d %d ;\n", getOffsetX(),
                              getOffsetY());
            }

            if (hasOffset()) {
                message->info("    OFFSET %d %d %d %d ;\n", getLowerOffsetX(),
                              getLowerOffsetY(), getUpperOffsetX(),
                              getUpperOffsetY());
            }
            if (getCutPatten().c_str()) {
                message->info("    PATTERN %s ;\n", getCutPatten().c_str());
            }
        }

        if (via_layers_) {
            VectorObject64* via_layer_vector =
                addr<VectorObject64>(via_layers_);
            for (VectorObject64::iterator iter = via_layer_vector->begin();
                 iter != via_layer_vector->end(); ++iter) {
                ObjectId id = (*iter);
                ViaLayer* via_layer = addr<ViaLayer>(id);
                via_layer->print(is_from_def_);
            }
        }
        if (getResistance())
            message->info("   RESISTANCE %f ;\n", getResistance());

        message->info("END %s\n\n", getName().c_str());
    } else {  // def via output format
        message->info(" - %s ", getName().c_str());

        if (isDefault()) {
            message->info(" DEFAULT\n");
        } else {
            message->info("\n");
        }

        if (pattern_index_)
            message->info(" + PATTERNNAME %s", getPattern().c_str());

        if (strcmp(getViaRule(), "")) {
            message->info("    + VIARULE %s ;\n", getViaRule());
            message->info("    + CUTSIZE %d %d ;\n", getCutSizeX(),
                          getCutSizeY());
            message->info("    + LAYERS %s %s %s ;\n", getLowerLayerIndex(),
                          getCutLayerIndex(), getUperLayerIndex());
            message->info("    + CUTSPACING %d %d ;\n", getCutSpacingX(),
                          getCutSpacingY());
            message->info("    + ENCLOSURE %d %d %d %d ;\n", getLowerEncX(),
                          getLowerEncY(), getUpperEncX(), getUpperEncY());
            if (isArray()) {
                message->info("    + ROWCOL %d %d ;\n", getRow(), getCol());
            }

            if (getCutPatten().c_str()) {
                message->info("    + PATTERN %s ;\n", getCutPatten().c_str());
            }

            if (hasOrigin()) {
                message->info("    + ORIGIN %d %d ;\n", getOffsetX(),
                              getOffsetY());
            }

            if (hasOffset()) {
                message->info("    + OFFSET %d %d %d %d ;\n", getLowerOffsetX(),
                              getLowerOffsetY(), getUpperOffsetX(),
                              getUpperOffsetY());
            }
        }

        if (getResistance())
            message->info("  RESISTANCE %f ;\n", getResistance());
        if (via_layers_) {
            VectorObject64* via_layer_vector =
                addr<VectorObject64>(via_layers_);
            for (VectorObject64::iterator iter = via_layer_vector->begin();
                 iter != via_layer_vector->end(); ++iter) {
                ObjectId id = (*iter);
                ViaLayer* via_layer = addr<ViaLayer>(id);
                via_layer->print(is_from_def_);
            }
        }

        message->info("\n");
    }
}

void ViaMaster::printDEF(FILE* fp) {
    fprintf(fp, " - %s ", getName().c_str());

    if (isDefault()) {
        fprintf(fp, " DEFAULT");
    }

    if (pattern_index_)
        fprintf(fp, "\n + PATTERNNAME %s", getPattern().c_str());

    if (strcmp(getViaRule(), "")) {
        fprintf(fp, "\n    + VIARULE %s", getViaRule());
        fprintf(fp, "\n    + CUTSIZE %d %d", getCutSizeX(), getCutSizeY());
        fprintf(fp, "\n    + LAYERS %s %s %s", getLowerLayerIndex(),
                getCutLayerIndex(), getUperLayerIndex());
        fprintf(fp, "\n    + CUTSPACING %d %d", getCutSpacingX(),
                getCutSpacingY());
        fprintf(fp, "\n    + ENCLOSURE %d %d %d %d", getLowerEncX(),
                getLowerEncY(), getUpperEncX(), getUpperEncY());
        if (isArray()) {
            fprintf(fp, "\n    + ROWCOL %d %d", getRow(), getCol());
        }

        if (getCutPatten().c_str()) {
            fprintf(fp, "\n    + PATTERN %s", getCutPatten().c_str());
        }

        if (hasOrigin()) {
            fprintf(fp, "\n    + ORIGIN %d %d", getOffsetX(), getOffsetY());
        }

        if (hasOffset()) {
            fprintf(fp, "\n    + OFFSET %d %d %d %d", getLowerOffsetX(),
                    getLowerOffsetY(), getUpperOffsetX(), getUpperOffsetY());
        }
    }

    if (getResistance()) fprintf(fp, "\n  RESISTANCE %f", getResistance());
    if (via_layers_) {
        VectorObject64* via_layer_vector =
            addr<VectorObject64>(via_layers_);
        int i = 0;
        int size = via_layer_vector->totalSize();
        for (VectorObject64::iterator iter = via_layer_vector->begin();
             iter != via_layer_vector->end(); ++iter) {
            ObjectId id = (*iter);
            ViaLayer* via_layer = addr<ViaLayer>(id);
            via_layer->printDEF(fp);
            ++i;
            if (i == size) {
                fprintf(fp, " ;");
            }
        }
    } else {
        fprintf(fp, " ;");
    }

    fprintf(fp, "\n\n");
}

void ViaMaster::printLEF(std::ofstream& ofs, uint32_t num_spaces) {
    std::string space_str = getSpaceStr(num_spaces);

    ofs << space_str << "VIA " << getName().c_str();

    if (isDefault()) ofs << " DEFAULT";

    if (hasGenerated()) ofs << " GENERATED";

    ofs << " \n";

    if (strcmp(getViaRule(), "")) {
        ofs << space_str << "  VIARULE " << getViaRule() << " ;\n";
        ofs << space_str << "    CUTSIZE " << getCutSizeX() << " "
            << getCutSizeY() << " ;\n";
        ofs << space_str << "    LAYERS " << getLowerLayerIndex() << " "
            << getCutLayerIndex() << " " << getUperLayerIndex() << " ;\n";
        ofs << space_str << "    CUTSPACING " << getCutSpacingX() << " "
            << getCutSpacingY() << " ;\n";
        ofs << space_str << "    ENCLOSURE " << getLowerEncX() << " "
            << getLowerEncY() << " " << getUpperEncX() << " " << getUpperEncY()
            << " ;\n";
        if (isArray()) {
            ofs << space_str << "    ROWCOL " << getRow() << " " << getCol()
                << " ;\n";
        }

        if (hasOrigin()) {
            ofs << space_str << "    ORIGIN " << getOffsetX() << " "
                << getOffsetY() << " ;\n";
        }

        if (hasOffset()) {
            ofs << space_str << "    OFFSET " << getLowerOffsetX() << " "
                << getLowerOffsetY() << " " << getUpperOffsetX() << " "
                << getUpperOffsetY() << " ;\n";
        }
        if (cut_patten_index_) {
            ofs << space_str << "    PATTERN " << getCutPatten().c_str()
                << " ;\n";
        }
    }

    if (getResistance())
        ofs << space_str << "   RESISTANCE " << getResistance() << " ;\n";

    if (via_layers_) {
        VectorObject64* via_layer_vector =
            addr<VectorObject64>(via_layers_);
        for (VectorObject64::iterator iter = via_layer_vector->begin();
             iter != via_layer_vector->end(); ++iter) {
            ObjectId id = (*iter);
            ViaLayer* via_layer = addr<ViaLayer>(id);
            via_layer->printLEF(ofs, num_spaces);
        }
    }

    if (numProperties() > 0) {
        ofs << space_str << "   PROPERTY";
        VectorObject16* vobj =
            addr<VectorObject16>(properties_);
        for (int i = 0; i < numProperties(); i++) {
            ObjectId obj_id = (*vobj)[i];
            Property* obj_data = addr<Property>(obj_id);
            if (obj_data == nullptr) continue;
            ofs << " ";
            obj_data->printLEF(ofs);
        }
        ofs << " ;\n";
    }

    ofs << space_str << "END " << getName().c_str() << "\n\n";
}

}  // namespace db
}  // namespace open_edi
