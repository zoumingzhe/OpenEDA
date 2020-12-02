/* @file  export_tech.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/export_tech.h"

#include <unistd.h>

#include "db/core/db.h"
#include "db/tech/tech.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

ExportTechLef::ExportTechLef(const char* tech_name) {
    if (!tech_name) return;

    std::string old_tech_name = tech_name;
    old_tech_name += ".old";
    std::ifstream ifs(tech_name, std::ios::in);
    if (ifs.is_open()) {
        ifs.close();
        ifs.open(old_tech_name, std::ios::in);
        if (ifs.good()) {
            ifs.close();
            unlink(old_tech_name.c_str());
        } else {
            ifs.close();
        }
        rename(tech_name, old_tech_name.c_str());
    }
    tech_lib_ = getTechLib();
    ofs_.open(tech_name, std::ios::out | std::ios::app);

    default_precision_ = cur_precision_ = ofs_.precision();
    getTechPrecision();
}

ExportTechLef::~ExportTechLef() {
    if (ofs_.is_open()) {
        ofs_.close();
    }
}

UInt32 ExportTechLef::getTechPrecision() {
    if (tech_precision_ != 0) {
        return tech_precision_;
    }
    UInt32 tech_dbu = tech_lib_->getUnits()->getLengthFactor();
    while (tech_dbu) {
        if (tech_dbu > 1) ++tech_precision_;
        tech_dbu = tech_dbu / 10;
    }
    if (tech_precision_ == 0) tech_precision_ = 1;

    return tech_precision_;
}

UInt32 ExportTechLef::getCurPrecision() { return cur_precision_; }

UInt32 ExportTechLef::setPrecision(UInt32 prec) {
    if (prec > 0) {
        cur_precision_ = prec;
        ofs_.setf(std::ios::fixed, std::ios::floatfield);
        return ofs_.precision(prec);
    }
    return default_precision_;
}

UInt32 ExportTechLef::resetPrecision() {
    ofs_.unsetf(std::ios::fixed | std::ios::floatfield);
    return ofs_.precision(default_precision_);
}

void ExportTechLef::exportAll() {
    exportVersion();
    exportNameCaseSensitive();
    exportBusBit();
    exportDividerChar();
    exportUseMinSpacing();
    exportClearanceMeasure();

    exportUnits();
    exportMGrid();

    exportAlias();
    exportPropertyDefinitions();
    exportFixedMask();
    exportMaxViaStack();
    exportSite();
    exportAllLayers();
    exportViaMaster();
    exportViaRule();
    exportNDR();
    exportCells();
    exportExtensions();
}

void ExportTechLef::exportVersion() {
    if (tech_lib_->getVersion() != 0.0) {
        ofs_ << "VERSION " << tech_lib_->getVersion() << " ;\n";
    }
}

void ExportTechLef::exportNameCaseSensitive() {
    if (tech_lib_->getCaseSensitive()) {
        ofs_ << "NAMESCASESENSITIVE ON ;\n";
    }
}

void ExportTechLef::exportBusBit() {
    const char* str_value = tech_lib_->getBusBitsName();
    if (strcmp(str_value, "")) {
        ofs_ << "BUSBITCHARS \"" << str_value << "\" ;\n";
    }
}

void ExportTechLef::exportDividerChar() {
    const char* str_value = tech_lib_->getDividerName();
    if (strcmp(str_value, "")) {
        ofs_ << "DIVIDERCHAR \"" << str_value << "\" ;\n";
    }
}

void ExportTechLef::exportUnits() {
    ofs_ << "\n";
    Units* units = tech_lib_->getUnits();
    if (!units) return;
    units->printLEF(ofs_);
}

void ExportTechLef::exportMGrid() {
    double dbl_value = tech_lib_->dbuToMicrons(tech_lib_->getManuGrids());
    ofs_ << "MANUFACTURINGGRID " << dbl_value << " ;\n\n";
}

void ExportTechLef::exportUseMinSpacing() {
    std::string str_value = "OFF";
    if (tech_lib_->getHasOBSUseMinSpacing()) {
        if (tech_lib_->getOBSUseMinSpacing()) {
            str_value = "ON";
        }
        ofs_ << "USEMINSPACING OBS " << str_value << " ;\n";
    }
    if (tech_lib_->getHasPinUseMinSpacing()) {
        if (tech_lib_->getPinUseMinSpacing()) {
            str_value = "ON";
        }
        ofs_ << "USEMINSPACING PIN " << str_value << " ;\n";
    }
}

void ExportTechLef::exportClearanceMeasure() {
    if (!tech_lib_->getHasClearanceMeasure()) {
        return;
    }
    std::string str_value = "EUCLIDEAN";
    if (tech_lib_->getClearanceMeasureUseMaxxy()) {
        str_value = "MAXXY";
    }
    ofs_ << "CLEARANCEMEASURE " << str_value << " ;\n";
}

void ExportTechLef::exportAlias() {}

void ExportTechLef::exportPropertyDefinitions() {
    ofs_ << "PROPERTYDEFINITIONS\n";
    for (int i = 0; i < toInteger(PropType::kUnknown); ++i) {
        ObjectId vobj_id =
            tech_lib_->getPropertyDefinitionVectorId(toEnum<PropType, int>(i));
        if (vobj_id == 0) continue;
        IdArray* arr_ptr = Object::addr<IdArray>(vobj_id);
        if (!arr_ptr) continue;
        for (IdArray::iterator iter = arr_ptr->begin(); 
                iter != arr_ptr->end(); iter++) {
            ObjectId obj_id = (*iter);
            if (!obj_id) continue;
            PropertyDefinition* obj_data =
                Object::addr<PropertyDefinition>(obj_id);
            if (!obj_data) continue;
            if (obj_data->getFromDEF()) continue;
            obj_data->printLEF(ofs_);
        }
    }
    ofs_ << "END PROPERTYDEFINITIONS\n\n";
}

void ExportTechLef::exportFixedMask() {}

void ExportTechLef::exportMaxViaStack() {}

void ExportTechLef::exportSite() {
    ObjectId vobj_id = tech_lib_->getSiteVectorId();
    if (vobj_id == 0) return;
    IdArray *arr_ptr = Object::addr<IdArray>(vobj_id);
    if (!arr_ptr) return;

    for (IdArray::iterator iter = arr_ptr->begin(); iter != arr_ptr->end(); ++iter) {
        ObjectId obj_id = (*iter);
        if (!obj_id) continue;
        Site* obj_data = Object::addr<Site>(obj_id);
        if (!obj_data) continue;
        obj_data->printLEF(ofs_);
    }
}

void ExportTechLef::exportImplantLayer(Layer* layer) {
    ImplantLayerRule* rule = layer->getImplantLayerRule();
    if (!rule) return;
    ofs_ << "LAYER " << layer->getName() << "\n";
    ofs_ << "    TYPE  "
         << "IMPLANT ;\n";
    if (layer->getWidth()) {
        ofs_ << "    WIDTH " << tech_lib_->dbuToMicrons(layer->getWidth())
             << " ;\n";
    }
    ArrayObject<ObjectId> *array = rule->getSpacingLists();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        ImplantSpacing* sp = Object::addr<ImplantSpacing>(*iter);
            ofs_ << "    SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " ";
            if (sp->getLayer2Id() > 0) {
                ofs_ << "LAYER "
                     << tech_lib_->getLayer(sp->getLayer2Id())->getName()
                     << " ";
            }
            ofs_ << ";\n";
        }
    exportLayerProperty(layer);
    ofs_ << "END " << layer->getName() << "\n\n";
}

void ExportTechLef::exportMinArea(const Layer* layer) {
    Layer::minAreaIter iter(layer->getMinAreaRuleId());
    MinArea* area;
    for (; area = iter.getNext();) {
        ofs_ << "    AREA " << tech_lib_->areaDBUToMicrons(area->getArea())
             << " ";
        if (area->getMaskNum()) {
            ofs_ << "MASK " << area->getMaskNum() << " ";
        }
        if (area->isExceptMinWidth()) {
            ofs_ << "    EXCEPTMINWIDTH "
                 << tech_lib_->dbuToMicrons(area->getMinWidth()) << " \n";
        } else if (area->isExceptEdgeLength()) {
            ofs_ << "    EXCEPTEDGELENGTH ";
            if (area->isExceptMinMaxEdgeLength()) {
                ofs_ << tech_lib_->dbuToMicrons(area->getExceptMinEdgeLength())
                     << " "
                     << tech_lib_->dbuToMicrons(area->getExceptMaxEdgeLength());
            } else {
                ofs_ << tech_lib_->dbuToMicrons(area->getExceptEdgeLength())
                     << " ";
            }
            ofs_ << "\n";
            if (area->isExceptMinSize()) {
                ofs_ << "    EXCEPTMINSIZE ";
                UInt32 size = area->getExceptMinSizeSize();
                for (UInt32 ii = 0; ii < size; ++ii) {
                    auto width_pair = area->getExceptMinSizePair(ii);
                    ofs_ << tech_lib_->dbuToMicrons(width_pair->data_first) << " "
                         << tech_lib_->dbuToMicrons(width_pair->data_second) << " ";
                }
                ofs_ << "\n";
            }
            if (area->isExceptStep()) {
                ofs_ << "    EXCEPTSTEP "
                     << tech_lib_->dbuToMicrons(area->getExceptStepLength1())
                     << " "
                     << tech_lib_->dbuToMicrons(area->getExceptStepLength2())
                     << "\n";
            }
        } else if (area->isRectWidth()) {
            ofs_ << "    RECTWIDTH "
                 << tech_lib_->dbuToMicrons(area->getRectWidth()) << "\n";
        } else if (area->isExceptRectangle()) {
            ofs_ << "    EXCEPTRECTANGLE "
                 << " ;\n";
        } else if (area->isLayer()) {
            Layer* trim = area->getLayer();
            UInt32 overlap = area->getOverlap();
            ofs_ << "    LAYER " << trim->getName() << " ";
            if (overlap) {
                ofs_ << "OVERLAP " << overlap << " ;\n";
            }
        }
        ofs_ << " ;\n";
    }
}

void ExportTechLef::exportMinSize(const Layer* layer) {
    RoutingLayerRule* r = layer->getRoutingLayerRule();
    if (r) {
        ArrayObject<ObjectId> *array = r->getMinSizes();
        if (array == nullptr)
            return;
        for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter)
        {
            MinSize *m = Object::addr<MinSize>(*iter);
            if (!m)
                return;
            ofs_ << "   MINSIZE ";
            if (m->isRectOnly())
                ofs_ << "RECTONLY ";
            UInt32 num = m->getMinSizeNum();
            for (UInt32 ii = 0; ii < num; ++ii)
            {
                ofs_ << tech_lib_->dbuToMicrons(m->getWidth(ii)) << " "
                     << tech_lib_->dbuToMicrons(m->getLength(ii)) << " ";
            }
            ofs_ << ";\n";
        }
    }
}

void ExportTechLef::exportMinEnclosedArea(const RoutingLayerRule* layer) {
    ArrayObject<ObjectId> *array = layer->getMinEnclAreas();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        MinEnclArea* mea = Object::addr<MinEnclArea>(*iter);
        ofs_ << "    MINENCLOSEDAREA "
             << tech_lib_->areaDBUToMicrons(mea->getArea()) << " ";
        if (mea->getWidth()) {
            ofs_ << "WIDTH " << tech_lib_->dbuToMicrons(mea->getWidth()) << " ";
        }
        ofs_ << ";\n";
    }
}

void ExportTechLef::exportMinStep(const RoutingLayerRule* layer) {
    ArrayObject<ObjectId> *array = layer->getMinSteps();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        MinStep* ms = Object::addr<MinStep>(*iter);
        ofs_ << "    MINSTEP "
             << tech_lib_->dbuToMicrons(ms->getMinStepLength()) << " ";
        if (ms->isMaxEdges()) {
            ofs_ << "MAXEDGES " << ms->getMaxEdges() << " ";
            // TODO:
            ofs_ << ";\n";
        } else {
            if (ms->isInsideCorner())
                ofs_ << "INSIDECORNER ";
            else if (ms->isOutsideCorner())
                ofs_ << "OUTSIDECORNER ";
            else if (ms->isStep())
                ofs_ << "STEP ";
            if (ms->getMaxLength()) {
                ofs_ << "LENGTHSUM "
                     << tech_lib_->dbuToMicrons(ms->getMaxLength()) << " ";
            }
            ofs_ << ";\n";
        }
    }
}

void ExportTechLef::exportProtrusionWidth(const RoutingLayerRule* layer) {
    ArrayObject<ObjectId> *array = layer->getProtrusionRules();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        ProtrusionRule* rule = Object::addr<ProtrusionRule>(*iter);
        if (rule->isLength()) {
            ofs_ << "    PROTRUSIONWIDTH "
                 << tech_lib_->dbuToMicrons(rule->getWidth1()) << " LENGTH "
                 << tech_lib_->dbuToMicrons(rule->getLength()) << " WIDTH "
                 << tech_lib_->dbuToMicrons(rule->getWidth2()) << " ;\n";
        } else if (rule->isWidth()) {
        }
    }
}

void ExportTechLef::exportRoutingSpacing(RoutingLayerRule* r) {
    ArrayObject<ObjectId> *array = r->getSpacings();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        RoutingSpacing* sp = Object::addr<RoutingSpacing>(*iter);
        if (!sp) return;
        if (sp->isRange()) {
            ofs_ << "    SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " "
                 << "RANGE " << tech_lib_->dbuToMicrons(sp->getRangeMinWidth())
                 << " " << tech_lib_->dbuToMicrons(sp->getRangeMaxWidth())
                 << " ";
            if (sp->isRangeUseLengthThres()) {
                ofs_ << "USELENGTHTHRESHOLD ";
            } else if (sp->isRangeInfluence()) {
                ofs_ << "INFLUENCE "
                     << tech_lib_->dbuToMicrons(sp->getRangeInfluenceValue())
                     << " ";
                if (sp->isRangeInfluenceRange()) {
                    ofs_ << "RANGE "
                         << tech_lib_->dbuToMicrons(sp->getRangeStubMinWidth())
                         << " "
                         << tech_lib_->dbuToMicrons(sp->getRangeStubMaxWidth())
                         << " ";
                }
            } else if (sp->isRangeRange()) {
                ofs_ << "RANGE "
                     << tech_lib_->dbuToMicrons(sp->getRangeRangeMinWidth())
                     << " "
                     << tech_lib_->dbuToMicrons(sp->getRangeRangeMaxWidth())
                     << " ";
            }
            ofs_ << " ;\n";
        } else if (sp->isLengthThreshold()) {
            ofs_ << "    SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " "
                 << "LENGTHTHRESHOLD "
                 << tech_lib_->dbuToMicrons(sp->getLenThresMaxLength()) << " ";
            if (sp->isLengthThresholdRange()) {
                ofs_ << "RANGE "
                     << tech_lib_->dbuToMicrons(sp->getLenThresMinWidth())
                     << " "
                     << tech_lib_->dbuToMicrons(sp->getLenThresMaxWidth())
                     << " ";
            }
            ofs_ << ";\n";
        } else if (sp->isEndOfLine()) {
            ofs_ << "    SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " "
                 << "ENDOFLINE " << tech_lib_->dbuToMicrons(sp->getEOLWidth())
                 << " WITHIN " << tech_lib_->dbuToMicrons(sp->getEOLWithin())
                 << " ";
            if (sp->getEOLPRLEdgeSpace()) {
                ofs_ << "PARALLELEDGE "
                     << tech_lib_->dbuToMicrons(sp->getEOLPRLEdgeSpace())
                     << " WITHIN "
                     << tech_lib_->dbuToMicrons(sp->getEOLPRLEdgeWithin())
                     << " " << (sp->isEOLTwoEdges() ? " TWOEDGES " : " ");
            }
            ofs_ << ";\n";
        } else if (sp->isSameNet()) {
            ofs_ << "    SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " "
                 << "SAMENET " << (sp->isSameNetPGOnly() ? "PGONLY " : " ")
                 << ";\n";
        } else if (sp->isNotchLength()) {
            if (sp->isLef58NotchLength()) {
                ofs_ << "    PROPERTY LEF58_SPACING \"SPACING "
                     << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " "
                     << "NOTCHLENGTH "
                     << tech_lib_->dbuToMicrons(
                            sp->getNotchLengthMinNotchLength())
                     << " ";
                if (sp->getNotchLengthLowExclSpacing() ||
                    sp->getNotchLengthHighExclSpacing()) {
                    ofs_ << "EXCEPWITHIN "
                         << tech_lib_->dbuToMicrons(
                                sp->getNotchLengthLowExclSpacing())
                         << " "
                         << tech_lib_->dbuToMicrons(
                                sp->getNotchLengthHighExclSpacing())
                         << " ";
                }
                if (sp->isNotchLengthWithin()) {
                    ofs_ << "WITHIN "
                         << tech_lib_->dbuToMicrons(sp->getNotchLengthWithin())
                         << " SPANLENGTH "
                         << tech_lib_->dbuToMicrons(
                                sp->getNotchLengthSpanLength())
                         << " ";
                }
                if (sp->isNotchLengthWidth() ||
                    sp->isNotchLengthConcaveEnds()) {
                    ofs_ << (sp->isNotchLengthWidth() ? "WIDTH "
                                                      : "CONCAVEENDS ")
                         << tech_lib_->dbuToMicrons(
                                sp->getNotchLengthSideNotchWidth())
                         << " ";
                }
                if (sp->isNotchLengthNotchWidth()) {
                    ofs_ << "NOTCHWIDTH "
                         << tech_lib_->dbuToMicrons(
                                sp->getNotchLengthNotchWidth())
                         << " ";
                }
                ofs_ << ";\" ;\n";
            } else {
                ofs_ << "    SPACING "
                     << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " "
                     << "NOTCHLENGTH "
                     << tech_lib_->dbuToMicrons(
                            sp->getNotchLengthMinNotchLength())
                     << " ;\n";
            }
        } else if (sp->isEndOfNotchWidth()) {
            ofs_ << "    SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " "
                 << "ENDOFNOTCHWIDTH "
                 << tech_lib_->dbuToMicrons(sp->getEONWidth())
                 << " NOTCHSPACING "
                 << tech_lib_->dbuToMicrons(sp->getEONSpacing())
                 << " NOTCHLENGTH "
                 << tech_lib_->dbuToMicrons(sp->getEONLength()) << " ;\n";
        } else if (sp->isEOLPerp()) {
            ofs_ << "    PROPERTY LEF58_SPACING \"SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing())
                 << " EOLPERPENDICULAR "
                 << tech_lib_->dbuToMicrons(sp->getEOLPerpEolWidth()) << " "
                 << tech_lib_->dbuToMicrons(sp->getEOLPerpPerpWidth())
                 << " ;\" ;\n";
        } else if (sp->isArea()) {
            ofs_ << "    PROPERTY LEF58_SPACING \"SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " AREA "
                 << tech_lib_->areaDBUToMicrons(sp->getArea()) << " ;\" ;\n";
        } else if (sp->isTrimLayerSpacing()) {
        } else if (sp->isSameMask()) {
            ofs_ << "    PROPERTY LEF58_SPACING \"SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing())
                 << " SAMEMASK ;\" ;\n";
        } else if (sp->isWrongDir()) {
            ofs_ << "    PROPERTY LEF58_SPACING \"SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing())
                 << " WRONGDIRECTION ";
            if (sp->getWrongDirEolWidth()) {
                ofs_ << "NOEOL "
                     << tech_lib_->dbuToMicrons(sp->getWrongDirEolWidth())
                     << " ";
            }
            if (sp->getWrongDirPRL()) {
                ofs_ << "PRL " << tech_lib_->dbuToMicrons(sp->getWrongDirPRL())
                     << " ";
            }
            if (sp->getWrongDirLength()) {
                ofs_ << "LENGTH "
                     << tech_lib_->dbuToMicrons(sp->getWrongDirLength()) << " ";
            }
            ofs_ << ";\" ;\n";
        } else if (sp->isNotchSpan()) {
            ofs_ << "    PROPERTY LEF58_SPACING \"SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing())
                 << " NOTCHSPAN "
                 << tech_lib_->dbuToMicrons(sp->getNotchSpanSpan())
                 << " NOTCHSPACING "
                 << tech_lib_->dbuToMicrons(sp->getNotchSpanSpacing())
                 << " EXCEPTNOTCHLENGTH "
                 << tech_lib_->dbuToMicrons(sp->getNotchSpanLength())
                 << " ;\" ;\n";
        } else if (sp->isConvexCorners()) {
        } else {
            ofs_ << "    SPACING "
                 << tech_lib_->dbuToMicrons(sp->getMinSpacing()) << " ;\n";
        }

    }
}

/**
SPACINGTABLE
PARALELLRUNLENGTH AND INFLUENCE
*/
void ExportTechLef::exportSpacingTable(const RoutingLayerRule* r) {
    std::string two_space = "  ";
    std::string four_space = "    ";
    std::string end = " ; \n";
    std::string line = " \n";
    UInt32 i = 0;
    UInt32 j = 0;

    // PARALLELRUNLENGTH
    ArrayObject<ObjectId> *array = r->getWidthSpTbls();
    if (array == nullptr) {
        return;
    } else {
        for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
            WidthSpTbl* edi_sp = Object::addr<WidthSpTbl>(*iter);
            UInt32 prl_dim = edi_sp->getPRLDim();
            ofs_ << four_space << "SPACINGTABLE" << line;

            if (edi_sp->isPRLWidth()) {  // PARALLELRUNLENGTH
                ofs_ << four_space << two_space << " PARALLELRUNLENGTH ";
                for (i = 0; i < prl_dim; i++) {
                    ofs_ << tech_lib_->dbuToMicrons(edi_sp->getPRL(i)) << " ";
                }
            } else if (edi_sp->isTwoWidths()) {  // TWOWIDTHS
                ofs_ << four_space << two_space << " TWOWIDTHS ";
            }

            ofs_ << line;

            UInt32 width_dim = edi_sp->getWidthDim();
            for (i = 0; i < width_dim; i++) {
                ofs_ << four_space << four_space << " WIDTH "
                    << tech_lib_->dbuToMicrons(edi_sp->getWidth(i));

                if (edi_sp->isTwoWidths()) {  // TWOWIDTHS
                    if (edi_sp->hasWidthPRL(i))
                        ofs_ << " PRL "
                         << tech_lib_->dbuToMicrons(edi_sp->getPRL(i));
                    else
                        ofs_ << four_space << four_space;
                }

                ofs_ << four_space;

                for (j = 0; j < prl_dim; j++) {
                    ofs_ << tech_lib_->dbuToMicrons(edi_sp->getSpacing(i, j))
                     << " ";
                }

                if (i != width_dim - 1)
                    ofs_ << line;
                else
                    ofs_ << end;
            }

        }
    }

    // INFLUENCE
    array = r->getInfluenceSpTbls();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        InfluenceSpTbl* edi_inf = Object::addr<InfluenceSpTbl>(*iter);
        ofs_ << four_space << "SPACINGTABLE" << line;
        ofs_ << four_space << two_space << "INFLUENCE" << line;

        UInt32 row_num = edi_inf->getRowNum();

        for (i = 0; i < row_num; i++) {
            UInt32 width = edi_inf->getWidth(i);
            ofs_ << four_space << four_space << " WIDTH "
                 << tech_lib_->dbuToMicrons(edi_inf->getWidth(i)) << " WITHIN "
                 << tech_lib_->dbuToMicrons(edi_inf->getWithin(i))
                 << " SPACING "
                 << tech_lib_->dbuToMicrons(edi_inf->getSpacing(i));
            if (i != row_num - 1)
                ofs_ << line;
            else
                ofs_ << end;
        }

    }
}

void ExportTechLef::exportMinCut(const RoutingLayerRule* r) {
    std::string four_space = "    ";
    std::string end = " ; \n";

    ArrayObject<ObjectId> *array = r->getMinCuts();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        MinCut* min_cut = Object::addr<MinCut>(*iter);
        ofs_ << four_space << "MINIMUMCUT " << min_cut->getNumCuts();
        ofs_ << " WIDTH " << tech_lib_->dbuToMicrons(min_cut->getWidth());

        if (min_cut->getCutWithin() > 0) {
            ofs_ << " WITHIN "
                 << tech_lib_->dbuToMicrons(min_cut->getCutWithin());
        }

        if (min_cut->isFromAbove()) {
            ofs_ << " FROMABOVE ";
        } else if (min_cut->isFromBelow()) {
            ofs_ << " FROMBELOW ";
        }

        if (min_cut->getLength() > 0) {
            ofs_ << " LENGTH " << tech_lib_->dbuToMicrons(min_cut->getLength());
        }

        if (min_cut->getLengthWithin() > 0) {
            ofs_ << " WITHIN "
                 << tech_lib_->dbuToMicrons(min_cut->getLengthWithin());
        }

        ofs_ << end;
    }
}

void ExportTechLef::exportLayerAntennaModel(const Layer* layer) {
    int model_num = kMaxOxideNum;
    int epsilon = 0.00001;
    for (UInt32 ii = 1; ii < model_num; ++ii) {
        const AntennaModel* model = layer->getAntennaModel(ii);
        if (!model || !model->isSet()) continue;
        ofs_ << "    ANTENNAMODEL OXIDE" << ii << " ;\n";
        if (model->getAreaRatio() > epsilon) {
            ofs_ << "    ANTENNAAREARATIO " << model->getAreaRatio() << " ;\n";
        }
        if (model->isDiffAreaRatioPWL()) {
            ofs_ << "    ANTENNADIFFAREARATIO PWL (";
            for (int kk = 0; kk < model->getDiffAreaRatioPWLSize(); ++kk) {
                auto pwl = model->getDiffAreaRatioPWL(kk);
                ofs_ << " ( " << pwl->data_first << " " << pwl->data_second << " ) ";
            }
            ofs_ << ") ;\n";
        } else if (model->getDiffAreaRatio() > epsilon) {
            ofs_ << "    ANTENNADIFFAREARATIO " << model->getDiffAreaRatio()
                 << " ;\n";
        }
        if (!model->isDefaultAreaFactor() && model->getAreaFactor() > epsilon) {
            ofs_ << "    ANTENNAAREAFACTOR " << model->getAreaFactor() << " ";
            if (model->isDiffUseOnly()) {
                ofs_ << "DIFFUSEONLY ";
            }
            ofs_ << ";\n";
        }
        if (model->getCumAreaRatio() > epsilon) {
            ofs_ << "    ANTENNACUMAREARATIO " << model->getCumAreaRatio()
                 << " ;\n";
        }
        if (model->isCumDiffAreaRatioPWL()) {
            ofs_ << "    ANTENNACUMDIFFAREARATIO PWL (";
            for (int kk = 0; kk < model->getCumDiffAreaRatioPWLSize(); ++kk) {
                auto pwl =
                    model->getCumDiffAreaRatioPWL(kk);
                ofs_ << " ( " << pwl->data_first << " " << pwl->data_second << " ) ";
            }
            ofs_ << ") ;\n";
        } else if (model->getCumDiffAreaRatio() > epsilon) {
            ofs_ << "    ANTENNACUMDIFFAREARATIO "
                 << model->getCumDiffAreaRatio() << " ;\n";
        }
        if (model->isCumRoutingPlusCut()) {
            ofs_ << "    ANTENNACUMROUTINGPLUSCUT ;\n";
        }
        if (model->getGatePlusDiffId()) {
            ofs_ << "    PROPERTY LEF58_ANTENNAGATEPLUSDIFF "
                    "\"ANTENNAGATEPLUSDIFF OXIDE"
                 << model->getGatePlusDiffId() << " ";
            if (model->getGatePlusDiffPWLSize()) {
                ofs_ << "PWL (";
                for (int kk = 0; kk < model->getGatePlusDiffPWLSize(); ++kk) {
                    auto pwl =
                        model->getGatePlusDiffPWL(kk);
                    ofs_ << "( " << pwl->data_first << " " << pwl->data_second << " ) ";
                }
                ofs_ << ")\" ;";
            } else {
                ofs_ << model->getGatePlusDiff() << " ;\" ;\n";
            }
        } else if (model->getGatePlusDiff() > epsilon) {
            ofs_ << "    ANTENNAGATEPLUSDIFF " << model->getGatePlusDiff()
                 << " ;\n";
        }
        if (model->getAreaMinusDiff() > epsilon) {
            ofs_ << "    ANTENNAAREAMINUSDIFF " << model->getAreaMinusDiff()
                 << " ;\n";
        }
        if (model->getAreaDiffReducePWLSize()) {
            ofs_ << "    ANTENNAAREADIFFREDUCEPWL (";
            for (int kk = 0; kk < model->getAreaDiffReducePWLSize(); ++kk) {
                FloatPair* pwl = model->getAreaDiffReducePWL(kk);
                ofs_ << " ( " << pwl->data_first << " " << pwl->data_second << " ) ";
            }
            ofs_ << ") ;\n";
        }
        if (model->getDiffGatePWLSize()) {
            ofs_ << "    PROPERTY LEF58_ANTENNADIFFGATEPWL "
                    "\"ANTENNADIFFGATEPWL OXIDE"
                 << model->getDiffGatePWLId() << " (";
            for (int kk = 0; kk < model->getDiffGatePWLSize(); ++kk) {
                auto pwl = model->getDiffGatePWL(kk);
                ofs_ << "( " << pwl->data_first << " " << pwl->data_second << " ) ";
            }
            ofs_ << ")\" ;";
        }
        if (model->getGatePWLSize()) {
            ofs_ << "    PROPERTY LEF58_ANTENNAGATEPWL \"ANTENNAGATEPWL OXIDE"
                 << model->getDiffGatePWLId() << " (";
            for (int kk = 0; kk < model->getGatePWLSize(); ++kk) {
                auto pwl = model->getGatePWL(kk);
                ofs_ << "( " << pwl->data_first << " " << pwl->data_second << " ) ";
            }
            ofs_ << ")\" ;";
        }
        if (layer->isRouting()) {
            if (model->getSideAreaRatio() > epsilon) {
                ofs_ << "    ANTENNASIDEAREARATIO " << model->getSideAreaRatio()
                     << " ;\n";
            }
            if (model->getDiffSideAreaRatioPWLSize()) {
                ofs_ << "    ANTENNADIFFSIDEAREARATIO PWL (";
                for (int kk = 0; kk < model->getDiffSideAreaRatioPWLSize();
                     ++kk) {
                    auto pwl =
                        model->getDiffSideAreaRatioPWL(kk);
                    ofs_ << " ( " << pwl->data_first << " " << pwl->data_second << " ) ";
                }
                ofs_ << ") ;\n";
            } else if (model->getDiffSideAreaRatio() > epsilon) {
                ofs_ << "    ANTENNADIFFSIDEAREARATIO "
                     << model->getDiffSideAreaRatio() << " ;\n";
            }
            if (model->getCumSideAreaRatio() > epsilon) {
                ofs_ << "    ANTENNACUMSIDEAREARATIO "
                     << model->getCumSideAreaRatio() << " ;\n";
            }
            if (model->getCumDiffSideAreaRatioPWLSize()) {
                ofs_ << "    ANTENNACUMDIFFSIDEAREARATIO PWL (";
                for (int kk = 0; kk < model->getCumDiffSideAreaRatioPWLSize();
                     ++kk) {
                    auto pwl =
                        model->getCumDiffSideAreaRatioPWL(kk);
                    ofs_ << " ( " << pwl->data_first << " " << pwl->data_second << " ) ";
                }
                ofs_ << ") ;\n";
            } else if (model->getCumDiffSideAreaRatio() > epsilon) {
                ofs_ << "    ANTENNACUMDIFFSIDEAREARATIO "
                     << model->getCumDiffSideAreaRatio() << " ;\n";
            }
            if (model->getSideAreaFactor() > epsilon) {
                ofs_ << "    ANTENNASIDEAREAFACTOR "
                     << model->getSideAreaFactor() << " ";
                if (model->isSideDiffUseOnly()) {
                    ofs_ << "DIFFUSEONLY ";
                }
                ofs_ << ";\n";
            }
        }
    }
}

void ExportTechLef::exportLayerCurrentDensityACItem(const Layer* layer,
                                                    CurrentDen* den) {
    if (!den) return;
    ofs_ << "    ACCURRENTDENSITY ";
    if (den->isPeak())
        ofs_ << "PEAK ";
    else if (den->isAverage())
        ofs_ << "AVERAGE ";
    else if (den->isRMS())
        ofs_ << "RMS ";
    bool is_simple_value = false;
    if (den->getFrequenciesNum()) {
        is_simple_value = true;
        ofs_ << "\n        FREQUENCY ";
        for (int kk = 0; kk < den->getFrequenciesNum(); ++kk) {
            ofs_ << den->getFrequency(kk) << " ";
        }
        ofs_ << ";\n";
    }

    if (den->getWidthsNum()) {
        is_simple_value = true;
        if (layer->isRouting())
            ofs_ << "        WIDTH ";
        else
            ofs_ << "        CUTAREA ";
        for (int kk = 0; kk < den->getWidthsNum(); ++kk) {
            if (layer->isRouting()) {
                ofs_ << tech_lib_->dbuToMicrons(den->getWidth(kk))
                     << " ";  // width & cut area use same storage
            } else {
                ofs_ << tech_lib_->areaDBUToMicrons(den->getCutArea(kk))
                     << " ";  // width & cut area use same storage
            }
        }
        ofs_ << ";\n";
    }
    if (den->hasTableEntries()) {
        is_simple_value = true;
        ofs_ << "        TABLEENTRIES";
        UInt32 row_num = den->getFrequenciesNum();
        UInt32 col_num = den->getWidthsNum();
        if (col_num == 0) {  // FREQUENCY without WIDTH or CUTAREA
            col_num = row_num;
            row_num = 1;
        }
        if (row_num > 1) {
            ofs_ << "\n            ";
        } else {
            ofs_ << " ";
        }
        for (int row_idx = 0; row_idx < row_num; ++row_idx) {
            for (int col_idx = 0; col_idx < col_num; ++col_idx) {
                ofs_ << den->getTableElem(row_idx, col_idx) << " ";
            }
            if (row_idx == (row_num - 1))
                ofs_ << ";\n";
            else
                ofs_ << "\n          ";
        }
    }
    if (!is_simple_value) {
        ofs_ << den->getCurrentDen() << " ;\n";
    }
}

void ExportTechLef::exportLayerCurrentDensityDCItem(const Layer* layer,
                                                    CurrentDen* den) {
    if (den) {
        ofs_ << "    DCCURRENTDENSITY AVERAGE ";
        UInt32 col_num = den->getWidthsNum();
        if (col_num) {
            if (layer->isRouting())
                ofs_ << "\n        WIDTH ";
            else
                ofs_ << "\n        CUTAREA ";
            for (int col_idx = 0; col_idx < col_num; ++col_idx) {
                if (layer->isRouting()) {
                    ofs_ << tech_lib_->dbuToMicrons(den->getWidth(col_idx))
                         << " ";
                } else {
                    ofs_ << tech_lib_->areaDBUToMicrons(
                                den->getCutArea(col_idx))
                         << " ";
                }
            }
            ofs_ << ";\n";
            if (den->hasTableEntries()) {
                ofs_ << "        TABLEENTRIES ";
                for (int col_idx = 0; col_idx < col_num; ++col_idx) {
                    ofs_ << den->getTableElem(0, col_idx) << " ";
                }
                ofs_ << ";\n";
            }
        } else {
            ofs_ << den->getCurrentDen() << " ;\n";
        }
    }
}

void ExportTechLef::exportLayerCurrentDensity(const Layer* layer) {
    // ACCURRENTDENSITY
    CurrentDenContainer* ac = layer->getACCurrentDenContainer();
    if (ac) {
        exportLayerCurrentDensityACItem(layer, ac->getACPeak());
        exportLayerCurrentDensityACItem(layer, ac->getACAverage());
        exportLayerCurrentDensityACItem(layer, ac->getACRMS());
    }

    // DCCURRENTDENSITY
    CurrentDenContainer* dc = layer->getDCCurrentDenContainer();
    if (dc) {
        exportLayerCurrentDensityDCItem(layer, dc->getDCAverage());
    }
}

void ExportTechLef::exportRoutingLayer(Layer* layer) {
    RoutingLayerRule* r = layer->getRoutingLayerRule();
    float epsilon = 0.00001;
    ofs_ << "LAYER " << layer->getName() << "\n";
    ofs_ << "    TYPE  " << (layer->isRouting() ? "ROUTING" : "ERROR")
         << " ;\n";
    if (layer->getNumMask() > 1)
        ofs_ << "    MASK " << layer->getNumMask() << " ;\n";
    ofs_ << "    DIRECTION ";
    if (r->isHoriz())
        ofs_ << "HORIZONTAL ;\n";
    else if (r->isVert())
        ofs_ << "VERTICAL ;\n";
    else if (r->isDiag45())
        ofs_ << "DIAG45 ;\n";
    else if (r->isDiag135())
        ofs_ << "DIAG135 ;\n";
    else
        ofs_ << "ERROR ;\n";
    if (r->isXYPitch()) {
        ofs_ << "    PITCH " << tech_lib_->dbuToMicrons(r->getPitchX()) << " "
             << tech_lib_->dbuToMicrons(r->getPitchY()) << " ;\n";
    } else if (r->getPitchX()) {
        ofs_ << "    PITCH " << tech_lib_->dbuToMicrons(r->getPitchX())
             << " ;\n";
    }
    if (r->isXYDiagPitch()) {
        ofs_ << "    DIAGPITCH " << tech_lib_->dbuToMicrons(r->getDiagPitch45())
             << " " << tech_lib_->dbuToMicrons(r->getDiagPitch135()) << " ;\n";
    } else if (r->getDiagPitch()) {
        ofs_ << "    DIAGPITCH " << tech_lib_->dbuToMicrons(r->getDiagPitch())
             << " ;\n";
    }
    if (layer->getWidth()) {
        ofs_ << "    WIDTH " << tech_lib_->dbuToMicrons(layer->getWidth())
             << " ; \n";
    }
    if (r->isXYOffset()) {
        ofs_ << "    OFFSET " << tech_lib_->dbuToMicrons(r->getOffsetX()) << " "
             << tech_lib_->dbuToMicrons(r->getOffsetY()) << " ;\n";
    } else {
        ofs_ << "    OFFSET " << tech_lib_->dbuToMicrons(r->getOffset())
             << " ;\n";
    }
    if (r->getDiagWidth()) {
        ofs_ << "    DIAGWIDTH " << tech_lib_->dbuToMicrons(r->getDiagWidth())
             << " ;\n";
    }
    if (r->getDiagSpacing()) {
        ofs_ << "    DIAGSPACING "
             << tech_lib_->dbuToMicrons(r->getDiagSpacing()) << " ;\n";
    }
    if (r->getDiagMinEdgeLength()) {
        ofs_ << "    DIAGMINEDGELENGTH "
             << tech_lib_->dbuToMicrons(r->getDiagMinEdgeLength()) << " ;\n";
    }
    if (r->getMinWidth()) {
        ofs_ << "    MINWIDTH " << tech_lib_->dbuToMicrons(r->getMinWidth())
             << " ;\n";
    }
    if (r->getMaxWidth()) {
        ofs_ << "    MAXWIDTH " << tech_lib_->dbuToMicrons(r->getMaxWidth())
             << " ;\n";
    }
    if (r->getWireExt()) {
        ofs_ << "    WIREEXTENSION " << tech_lib_->dbuToMicrons(r->getWireExt())
             << " ;\n";
    }

    exportMinArea(layer);
    exportMinSize(layer);
    exportRoutingSpacing(r);
    exportSpacingTable(r);
    exportMinCut(r);
    exportMinEnclosedArea(r);
    exportMinStep(r);
    exportProtrusionWidth(r);

    if (r->getResPerSq() > epsilon) {
        ofs_ << "    RESISTANCE RPERSQ " << r->getResPerSq() << " ;\n";
    }
    if (r->getCapPerSq() > epsilon) {
        ofs_ << "    CAPACITANCE CPERSQDIST " << r->getCapPerSq() << " ;\n";
    }
    if (r->getHeight()) {
        ofs_ << "    HEIGHT " << tech_lib_->dbuToMicrons(r->getHeight())
             << " ;\n";
    }
    if (r->getThickness()) {
        ofs_ << "    THICKNESS " << tech_lib_->dbuToMicrons(r->getThickness())
             << " ;\n";
    }
    if (r->getShrinkage()) {
        ofs_ << "    SHRINKAGE " << tech_lib_->dbuToMicrons(r->getShrinkage())
             << " ;\n";
    }
    if (r->getCapMul() > epsilon) {
        ofs_ << "    CAPMULTIPLIER " << r->getCapMul() << " ;\n";
    }
    if (r->getEdgeCap() > epsilon) {
        ofs_ << "    EDGECAPACITANCE " << r->getEdgeCap() << " ;\n";
    }
    if (r->getFillMinDensity() > epsilon) {
        ofs_ << "    MINIMUMDENSITY " << r->getFillMinDensity() << " ;\n";
    }
    if (r->getFillMaxDensity() > epsilon) {
        ofs_ << "    MAXIMUMDENSITY " << r->getFillMaxDensity() << " ;\n";
    }
    if (r->getFillCheckWinLength() || r->getFillCheckWinWidth()) {
        ofs_ << "    DENSITYCHECKWINDOW "
             << tech_lib_->dbuToMicrons(r->getFillCheckWinLength()) << " "
             << tech_lib_->dbuToMicrons(r->getFillCheckWinWidth()) << " ;\n";
    }
    if (r->getFillCheckWinStep()) {
        ofs_ << "    DENSITYCHECKSTEP "
             << tech_lib_->dbuToMicrons(r->getFillCheckWinStep()) << " ;\n";
    }
    if (r->getFillActiveSpacing()) {
        ofs_ << "    FILLACTIVESPACING "
             << tech_lib_->dbuToMicrons(r->getFillActiveSpacing()) << " ;\n";
    }

    exportLayerProperty(layer);
    exportLayerAntennaModel(layer);
    exportLayerCurrentDensity(layer);

    ofs_ << "END " << layer->getName() << "\n\n";
}

void ExportTechLef::exportCutLayer(Layer* layer) {
    ofs_ << "LAYER " << layer->getName() << "\n";
    ofs_ << "    TYPE  " << (layer->isCut() ? "CUT" : "ERROR") << " ;\n";

    exportLayerProperty(layer);
    CutLayerRule* cut_rule = layer->getCutLayerRule();
    if (cut_rule) {
        exportCutSpacing(cut_rule);
        exportEnclosure(cut_rule);
        exportPreferEnclosure(cut_rule);
        exportArraySpacing(cut_rule);
    }
    exportLayerAntennaModel(layer);
    exportLayerCurrentDensity(layer);

    ofs_ << "END " << layer->getName() << "\n\n";
}

void ExportTechLef::exportCutSpacing(const CutLayerRule* cut_rule) {
    ArrayObject<ObjectId> *array = cut_rule->getCutSpacingArray();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        CutSpacing* cut_spacing = Object::addr<CutSpacing>(*iter);
        if (!cut_spacing)
            return;
        ofs_ << "    SPACING  " 
             << tech_lib_->dbuToMicrons(cut_spacing->getSpacing());
        if (cut_spacing->getIsC2C()) {
            ofs_ << " CENTERTOCENTER ";
        }
        if (cut_spacing->getIsSameNet()) {
            ofs_ << " SAMENET ";
        }
        if (cut_spacing->getIsAdjCuts()) {
            AdjacentCuts* adj_cuts = cut_spacing->getAdjCuts();
            if (adj_cuts) {
                ofs_ << "  ADJACENTCUTS  " << adj_cuts->getCutNum()
                     << "  WITHIN  "
                     << tech_lib_->dbuToMicrons(adj_cuts->getCutWithin());
                if (adj_cuts->getIsExceptSamePGNet()) {
                    ofs_ << "  EXCEPTSAMEPGNET ";
                }
            }
        } else if (cut_spacing->getIsParallelOverlap()) {
            CutSpacingPrlOvlp* prl_ovlp = cut_spacing->getParallelOverlap();
            if (prl_ovlp) {
                if (prl_ovlp->getIsParallelOverlap()) {
                    ofs_ << "  PARALLELOVERLAP  ";
                }
            }
        } else if (cut_spacing->getIsSecondLayer()) {
            SecondLayer* sec_layer = cut_spacing->getSecondLayer();
            if (sec_layer) {
                Layer* layer = getTopCell()->getLayerByLayerId(
                    sec_layer->getSecondLayerId());
                if (layer) {
                    ofs_ << "  LAYER " << layer->getName();
                    if (sec_layer->getIsStack()) {
                        ofs_ << "  STACK ";
                    }
                }
            }
        } else if (cut_spacing->getIsCutArea()) {
            ofs_ << "  AREA "
                 << tech_lib_->areaDBUToMicrons(cut_spacing->getCutArea());
        }
        ofs_ << " ;\n";
    }
}

void ExportTechLef::exportEnclosure(const CutLayerRule* cut_rule) {
    ArrayObject<ObjectId> *array = cut_rule->getEnclosureArray();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        Enclosure* enclosure = Object::addr<Enclosure>(*iter);
        if (!enclosure)
            return;
        ofs_ << "    ENCLOSURE  ";
        if (enclosure->getIsAbove()) {
            ofs_ << "ABOVE  ";
        } else if (enclosure->getIsBelow()) {
            ofs_ << "BELOW ";
        }
        if (enclosure->getIsOverhang()) {
            EnclosureOverhang* enc_overhang = enclosure->getOverhang();
            if (enc_overhang) {
                ofs_ << tech_lib_->dbuToMicrons(enc_overhang->getOverhang1())
                     << " "
                     << tech_lib_->dbuToMicrons(enc_overhang->getOverhang2())
                     << " ";
                if (enc_overhang->isWidth()) {
                    ofs_ << "WIDTH "
                         << tech_lib_->dbuToMicrons(enc_overhang->getMinWidth())
                         << " ";
                    if (enc_overhang->isExceptExtraCut()) {
                        ofs_ << "EXCEPTEXTRACUT "
                             << tech_lib_->dbuToMicrons(
                                    enc_overhang->getExceptExtraCutWithin())
                             << " ";
                    }
                } else if (enc_overhang->isLength()) {
                    ofs_ << "LENGTH "
                         << tech_lib_->dbuToMicrons(
                                enc_overhang->getMinLength())
                         << " ";
                }
                ofs_ << "; \n";
            }
        } else if (enclosure->getIsEol()) {
        }
    }
}

void ExportTechLef::exportPreferEnclosure(const CutLayerRule* cut_rule) {
    ArrayObject<ObjectId> *array = cut_rule->getPreferEnclosureArray();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        Enclosure* prefer_enclosure = Object::addr<Enclosure>(*iter);
        if (!prefer_enclosure)
            return;
        ofs_ << "    PREFERENCLOSURE  ";
        if (prefer_enclosure->getIsAbove()) {
            ofs_ << "ABOVE  ";
        } else if (prefer_enclosure->getIsBelow()) {
            ofs_ << "BELOW ";
        }
        if (prefer_enclosure->getIsOverhang()) {
            EnclosureOverhang* enc_overhang = prefer_enclosure->getOverhang();
            if (enc_overhang) {
                ofs_ << tech_lib_->dbuToMicrons(enc_overhang->getOverhang1())
                     << " "
                     << tech_lib_->dbuToMicrons(enc_overhang->getOverhang2())
                     << " ";
                if (enc_overhang->isWidth()) {
                    ofs_ << "WIDTH "
                         << tech_lib_->dbuToMicrons(enc_overhang->getMinWidth())
                         << " ";
                }
                ofs_ << "; \n";
            }
        }
    }
}

void ExportTechLef::exportArraySpacing(const CutLayerRule* cut_rule) {
    ArrayObject<ObjectId> *array = cut_rule->getArraySpacingArray();
    if (array == nullptr)
        return;
    for (ArrayObject<ObjectId>::iterator iter = array->begin(); iter != array->end(); ++iter) {
        ArraySpacing* array_spacing = Object::addr<ArraySpacing>(*iter);
        if (!array_spacing)
            return;
        ofs_ << "    ARRAYSPACING  ";
        if (array_spacing->getIsLongArray()) {
            ofs_ << "LONGARRAY  ";
        }
        if (array_spacing->hasViaWidth()) {
            ofs_ << "WIDTH  "
                 << tech_lib_->dbuToMicrons(array_spacing->getViaWidth())
                 << "  ";
        }
        ofs_ << "CUTSPACING  "
             << tech_lib_->dbuToMicrons(array_spacing->getCutSpacing()) << "  ";
        for (int i = 0; i < array_spacing->getNumArrayCuts(); i++) {
            ofs_ << "ARRAYCUTS  " << array_spacing->getArrayCuts(i) << " ";
            ofs_ << "SPACING  "
                 << tech_lib_->dbuToMicrons(array_spacing->getArraySpacing(i))
                 << "  ";
        }
        ofs_ << " ;\n";
    }
}

void ExportTechLef::exportMastersliceLayer(Layer* layer) {
    ofs_ << "LAYER " << layer->getName() << "\n";
    ofs_ << "    TYPE  "
         << "MASTERSLICE ;\n";
    exportLayerProperty(layer);
    ofs_ << "END " << layer->getName() << "\n\n";
}

void ExportTechLef::exportOverlapLayer(Layer* layer) {
    ofs_ << "LAYER " << layer->getName() << "\n";
    ofs_ << "    TYPE  "
         << "OVERLAP ;\n";
    exportLayerProperty(layer);
    ofs_ << "END " << layer->getName() << "\n\n";
}

void ExportTechLef::exportLayerProperty(const Layer* layer) {
    if (layer->getPropId()) {
        IdArray* vobj =
            Object::addr<IdArray>(layer->getPropId());
        if (vobj && vobj->getSize()) {
            for (int ii = 0; ii < vobj->getSize(); ++ii) {
                Property* prop = Object::addr<Property>((*vobj)[ii]);
                std::ostringstream oss;
                std::string header = "    PROPERTY ";
                oss << header;
                switch (prop->getDataType()) {
                    case PropDataType::kInt:
                        oss << prop->getPropName() << " " << prop->getIntValue()
                            << " ";
                        break;
                    case PropDataType::kReal:
                        oss << prop->getPropName() << " "
                            << prop->getRealValue() << " ";
                        break;
                    default:
                        if (strncmp("LEF", prop->getPropName(),
                                    3)) {  // don't export LEF* property because
                                           // they are exported by rules
                            oss << prop->getPropName() << " \""
                                << prop->getStringValue() << "\" ";
                        }
                        break;
                }
                if (oss.str().length() > header.length()) {
                    ofs_ << oss.str() << ";\n";
                }
            }
        }
    }
}

void ExportTechLef::exportLayer(Layer* layer) {
    if (!layer) return;
    if (layer->isRouting()) {
        exportRoutingLayer(layer);
    } else if (layer->isCut()) {
        exportCutLayer(layer);
    } else if (layer->isImplant()) {
        exportImplantLayer(layer);
    } else if (layer->isMS()) {
        exportMastersliceLayer(layer);
    } else if (layer->isOverlap()) {
        exportOverlapLayer(layer);
    }
}

void ExportTechLef::exportAllLayers() {
    UInt32 num_layers = tech_lib_->getNumLayers();
    for (UInt32 ii = 0; ii < num_layers; ++ii) {
        exportLayer(tech_lib_->getLayer(ii));
    }
}

void ExportTechLef::exportViaMaster() {
    ObjectId vobj_id = tech_lib_->getViaMasterVectorId();
    if (vobj_id == 0) return;
    IdArray* arr_ptr = Object::addr<IdArray>(vobj_id);
    if (!arr_ptr) return;

    for (IdArray::iterator iter = arr_ptr->begin(); iter != arr_ptr->end();
         ++iter) {
        ObjectId obj_id = (*iter);
        if (!obj_id) continue;
        ViaMaster* obj_data = Object::addr<ViaMaster>(obj_id);
        if (!obj_data) continue;
        if (obj_data->isFromDEF()) continue;
        if (obj_data->isFromNDR()) continue;
        obj_data->printLEF(ofs_);
    }
}

void ExportTechLef::exportViaRule() {
    ObjectId vobj_id = tech_lib_->getViaRuleVectorId();
    if (vobj_id == 0) return;
    IdArray* arr_ptr= Object::addr<IdArray>(vobj_id);
    if (!arr_ptr) return;

    for (IdArray::iterator iter = arr_ptr->begin(); iter != arr_ptr->end();
         ++iter) {
        ObjectId obj_id = (*iter);
        if (!obj_id) continue;
        ViaRule* obj_data = Object::addr<ViaRule>(obj_id);
        if (!obj_data) continue;

        obj_data->printLEF(ofs_);
    }
}

void ExportTechLef::exportNDR() {
    ObjectId vobj_id = tech_lib_->getNonDefaultRuleVectorId();
    if (vobj_id == 0) return;
    IdArray* arr_ptr = Object::addr<IdArray>(vobj_id);
    if (!arr_ptr) return;

    for (IdArray::iterator iter = arr_ptr->begin(); iter != arr_ptr->end();
         ++iter) {
        ObjectId obj_id = (*iter);
        if (!obj_id) continue;
        NonDefaultRule* obj_data =
            Object::addr<NonDefaultRule>(obj_id);
        if (!obj_data) continue;
        if (obj_data->getFromDEF()) continue;
        obj_data->printLEF(ofs_);
    }
}

void ExportTechLef::exportCells() {
    Tech* tech_lib = getTechLib();
    if (!tech_lib) return;
    for (uint64_t i = 0; i < tech_lib->getNumOfCells(); i++) {
        Cell* cell = tech_lib->getCell(i);
        if (cell) {
            // message->info("print out %dth macro to topcell\n", test_counttt);
            cell->printLEF(ofs_);
            // cell->print();
        }
    }
}

// this section also display end-of-library
void ExportTechLef::exportExtensions() {
    const char* str_value = tech_lib_->getExtensionsName();
    if (strcmp(str_value, "")) {
        ofs_ << str_value;
    }
    ofs_ << "\nEND LIBRARY\n";
}

}  // namespace db
}  // namespace open_edi
