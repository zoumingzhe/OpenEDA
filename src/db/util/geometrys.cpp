/* @file  geometry.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/util/geometrys.h"
#include "db/util/array.h"
#include "db/core/db.h"
#include "db/core/root.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

 Polygon* Geometry::getPath() const{
    StorageUtil *owner_util = Object::getStorageUtilById(this->getOwnerId());
    ediAssert(owner_util != nullptr);
    return owner_util->getPolygonTable()->getPolygonByIndex(path_id);
 }

Polygon* Geometry::getPolygon() const{
    StorageUtil *owner_util = Object::getStorageUtilById(this->getOwnerId());
    ediAssert(owner_util != nullptr);
    return owner_util->getPolygonTable()->getPolygonByIndex(polygon_id);
}

SymbolTable *GeometryVia::getSymbolTable() {
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError,
            "Cannot find top cell when finding symboltable.\n");
        return nullptr;
    }

    return top_cell->getSymbolTable();
}

SymbolIndex GeometryVia::getOrCreateSymbol(const char *name) {
    return getSymbolTable()->getOrCreateSymbol(name);
}

void GeometryVia::setViaMaster(const char *value) {
    std::string name = value;
    Tech *lib = getTopCell()->getTechLib();
    ViaMaster* vm = lib->getViaMaster(name);
    if (vm == nullptr) {
        message->issueMsg(kError, "GeometryVia find via master %s failed.\n", value);
        return;
    }
    via_master_index_ = vm->getId();
}

ViaMaster* GeometryVia::getViaMaster() {
    return addr<ViaMaster>(via_master_index_);
}

LayerGeometry::LayerGeometry() {
    memset((void*)this, 0, sizeof(LayerGeometry));
    min_spacing_ = -1;
    design_rule_width_ = -1;
    width_ = -1;
}

LayerGeometry::~LayerGeometry() {
}

SymbolTable *LayerGeometry::getSymbolTable() {
    StorageUtil *owner_util = Object::getStorageUtilById(this->getOwnerId());
    ediAssert(owner_util != nullptr);  
    return owner_util->getSymbolTable();
}

SymbolIndex LayerGeometry::getOrCreateSymbol(const char *name) {
    return getSymbolTable()->getOrCreateSymbol(name);
}

void LayerGeometry::setLayer(const char *value) {
    Tech *lib = getTopCell()->getTechLib();
    uint32_t index =  lib->getLayerLEFIndexByName(value);
    if (index == -1) {
        message->issueMsg(kError, "Layer Geometry find layer %s failed.\n", value);
        return;
    }
    if (index > 255) {
        message->issueMsg(kError, "Layer Geometry find layer index %d larger than 255.\n", index);
        return;
    }
    layer_index_= index;
}

Layer* LayerGeometry::getLayer() {
    Tech *lib = getTopCell()->getTechLib();
    return lib->getLayer(layer_index_);
}

void LayerGeometry::addGeometry(ObjectId id) {
    IdArray *vct = nullptr;

    if (geometrys_ == 0) {
        geometrys_ = __createObjectIdArray(32);
    }
    vct = addr<IdArray>(geometrys_);
    if (type_ != kVia)
        vct->pushBack(id);
}

void LayerGeometry::addGeometryVia(ObjectId id) {
    IdArray *vct = nullptr;
    if (geometrys_ == 0) {
        geometrys_ = __createObjectIdArray(32);
    }
    vct = addr<IdArray>(geometrys_);
    if (type_ == kVia)
        vct->pushBack(id);
}

Geometry* LayerGeometry::getGeometry(int i) const {
    IdArray *vct = nullptr;
    if (geometrys_ == 0) {
        return nullptr;
    } else {
        vct = addr<IdArray>(geometrys_);
    }
    if (vct && type_ != kVia) {
        Geometry *obj_data = addr<Geometry>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

GeometryVia* LayerGeometry::getGeometryVia(int i) const {
    IdArray *vct = nullptr;
    if (geometrys_ == 0) {
        return nullptr;
    } else {
        vct = addr<IdArray>(geometrys_);
    }
    if (vct && type_ == kVia) {
        GeometryVia *obj_data = addr<GeometryVia>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}
int LayerGeometry::getVecNum() const {
    IdArray *vct = nullptr;
    if (geometrys_ == 0) {
        return -1;
    } else {
        vct = addr<IdArray>(geometrys_);
    }
    if (vct) {
        return vct->getSize();
    }
    return 0;
}

PolygonTable* LayerGeometry::getPolygonTable() {
    StorageUtil *owner_util = Object::getStorageUtilById(this->getOwnerId());
    ediAssert(owner_util != nullptr);
    return owner_util->getPolygonTable();
}

int64_t LayerGeometry::CreatePolygon(Polygon* p) {
    PolygonTable* t = getPolygonTable();
    if (t) return t->addPolygon(p);
    return 0;
}

void LayerGeometry::print() {
    Tech *lib = getTechLib();
    if (getType() != GeometryType::kVia) {
        if (this->getLayer() == nullptr)
            return;
        message->info("         LAYER %s ", this->getLayer()->getName());
    }
    if ( this->hasEXCEPTPGNET())
        message->info("EXCEPTPGNET");
    if ( this->hasSpacing())
        message->info("SPACING %g", lib->dbuToMicrons(this->getMinSpacing()));
    if ( this->hasDRW())
        message->info("DESIGNRULEWIDTH %g",
            lib->dbuToMicrons(this->getdesignRuleWidth()));
    message->info(" ;\n");
    if (this->getWidth() > 0)
        message->info("         WIDTH %g ;\n",
            lib->dbuToMicrons(this->getWidth()));

    if (this->getVecNum() > 0) {
        for (int i = 0; i < this->getVecNum(); i++) {
            if (this->getType() == GeometryType::kVia) {
                GeometryVia *via = this->getGeometryVia(i);
                if (via->getViaMaster() == nullptr)
                    return;
                if (via->getTopMaskNum() != 0 || via->getCutMaskNum() != 0
                    || via->getBottomMaskNum() != 0) {
                    message->info("         VIA MASK %d%d%d %g %g %s ;\n",
                        via->getTopMaskNum(), via->getCutMaskNum(),
                        via->getBottomMaskNum(),
                        lib->dbuToMicrons(via->getPoint().getX()),
                        lib->dbuToMicrons(via->getPoint().getY()),
                        via->getViaMaster()->getName().c_str());
                } else {
                    message->info("         VIA %g %g %s ;\n",
                    lib->dbuToMicrons(via->getPoint().getX()),
                    lib->dbuToMicrons(via->getPoint().getY()),
                    via->getViaMaster()->getName().c_str());
                }
            } else {
                Geometry *geo = this->getGeometry(i);
                switch (geo->getType()) {
                case GeometryType::kPath: {
                    Polygon *p = getPolygonTable()->getPolygonByIndex(geo->getPathID());
                    message->info("         PATH ");
                    if (geo->getNumMask() != 0)
                        message->info("MASK %d ", geo->getNumMask());
                    for (int x = 0; x < p->getNumPoints(); x++) {
                        message->info("%g %g ",
                            lib->dbuToMicrons(p->getPoint(x).getX()),
                            lib->dbuToMicrons(p->getPoint(x).getY()));
                    }
                    message->info(";\n");
                    break;
                }
                case GeometryType::kRect: {
                    message->info("         RECT ");
                    if (geo->getNumMask() != 0)
                        message->info("MASK %d ", geo->getNumMask());
                    message->info("%g %g %g %g ;\n",
                        lib->dbuToMicrons(geo->getBox().getLLX()),
                        lib->dbuToMicrons(geo->getBox().getLLY()),
                        lib->dbuToMicrons(geo->getBox().getURX()),
                        lib->dbuToMicrons(geo->getBox().getURY()));
                    break;
                }
                case GeometryType::kPolygon: {
                    Polygon *p = this->getPolygonTable()->getPolygonByIndex(geo->getPolygonID());
                    message->info("         POLYGON ");
                    if (geo->getNumMask() != 0)
                        message->info("MASK %d ", geo->getNumMask());
                    for (int x = 0; x < p->getNumPoints(); x++)
                        message->info("%g %g ", lib->dbuToMicrons(p->getPoint(x).getX()),
                            lib->dbuToMicrons(p->getPoint(x).getY()));
                    message->info(";\n");
                    break;
                }
                }
            }
        }
    }
}

void LayerGeometry::printLEF(std::ofstream & ofs, bool from_port) {
    Tech *lib = getTechLib();
    if (from_port && this->getType() != GeometryType::kVia)
        ofs << "   ";
	if (getType() != GeometryType::kVia) {
        if (this->getLayer() == nullptr)
            return;
    	ofs << "      LAYER" <<  " " <<  this->getLayer()->getName();
    }
    if (this->hasEXCEPTPGNET())
        ofs << " EXCEPTPGNET";
    if (this->hasSpacing())
        ofs << " SPACING" <<  " " <<  lib->dbuToMicrons(this->getMinSpacing());
    if (this->hasDRW())
        ofs << " DESIGNRULEWIDTH" <<  " " <<
            lib->dbuToMicrons(this->getdesignRuleWidth());
	if (getType() != GeometryType::kVia)
    	ofs << " ;\n";
    if (this->getWidth() > 0) {
        if (from_port)
            ofs << "   ";
        ofs << "         WIDTH" <<  " " <<  lib->dbuToMicrons(this->getWidth()) << " ;\n";
    }

    if (this->getVecNum() > 0) {
        for (int i = 0; i < this->getVecNum(); i++) {
            if (this->getType() == GeometryType::kVia) {
                GeometryVia *via = this->getGeometryVia(i);
                if (via->getViaMaster() == nullptr)
                    return;
                if (from_port)
                    ofs << "   ";
                if (via->getTopMaskNum() != 0 || via->getCutMaskNum() != 0 || via->getBottomMaskNum() != 0) {
                    ofs << "      VIA MASK" <<  " " <<  via->getTopMaskNum() <<
                        via->getCutMaskNum() <<  via->getBottomMaskNum() <<  " " <<
                        lib->dbuToMicrons(via->getPoint().getX()) <<  " " <<
                        lib->dbuToMicrons(via->getPoint().getY()) <<  " " <<
                        via->getViaMaster()->getName().c_str() << " ;\n";
                } else {
                    ofs << "      VIA" <<  " " <<
                        lib->dbuToMicrons(via->getPoint().getX()) <<  " "
                        <<  lib->dbuToMicrons(via->getPoint().getY())
                        <<  " " <<  via->getViaMaster()->getName().c_str() << " ;\n";
                }
            } else {
                Geometry *geo = this->getGeometry(i);
                if (from_port)
                    ofs << "   ";
                switch (geo->getType()) {
                case GeometryType::kPath:
                {
                    Polygon *p = getPolygonTable()->getPolygonByIndex(geo->getPathID());
                    ofs << "         PATH";
                    if (geo->getNumMask() != 0)
                        ofs << " MASK" <<  " " <<  geo->getNumMask();
                    for (int x = 0; x < p->getNumPoints(); x++) {
                        ofs << " " << lib->dbuToMicrons(p->getPoint(x).getX())
                         <<  " " <<  lib->dbuToMicrons(p->getPoint(x).getY());
                    }
                    ofs << " ;\n";
                    break;
                }
                case GeometryType::kRect:
                {
                    ofs << "         RECT";
                    if (geo->getNumMask() != 0)
                        ofs << " MASK" <<  " " <<  geo->getNumMask();
                    ofs << " " <<  lib->dbuToMicrons(geo->getBox().getLLX())
                        <<  " " << lib->dbuToMicrons(geo->getBox().getLLY())
                        <<  " " << lib->dbuToMicrons(geo->getBox().getURX())
                        <<  " " <<  lib->dbuToMicrons(geo->getBox().getURY()) << " ;\n";
                    break;
                }
                case GeometryType::kPolygon:
                {
                    Polygon *p = this->getPolygonTable()->getPolygonByIndex(geo->getPolygonID());
                    ofs << "         POLYGON";
                    if (geo->getNumMask() != 0)
                        ofs << " MASK" <<  " " <<  geo->getNumMask();
                    for (int x = 0; x < p->getNumPoints(); x++)
                        ofs << " " <<  lib->dbuToMicrons(p->getPoint(x).getX()) <<
                         " " <<  lib->dbuToMicrons(p->getPoint(x).getY());
                    ofs << " ;\n";
                    break;
                }
                }
            }
        }
    }
}

}  // namespace db
}  // namespace open_edi
