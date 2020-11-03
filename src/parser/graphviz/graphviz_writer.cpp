/**
 * File              : graphviz_writer.cpp
 * Author            : Yibo Lin <yibolin@pku.edu.cn>
 * Date              : 06.19.2020
 * Last Modified Date: 06.19.2020
 * Last Modified By  : Yibo Lin <yibolin@pku.edu.cn>
 */

#include "parser/graphviz/graphviz_writer.h"
#include "db/core/db.h"

EDI_BEGIN_NAMESPACE

GraphvizWriter::GraphvizWriter(Database const& db)
  : db_(db) {
}

bool GraphvizWriter::run(std::string const& filename) const {
    /* remove-design
  std::ofstream os (filename.c_str()); 
  if (!os.good()) {
    ediPrint(kError, "failed to open file %s for write\n", filename.c_str());
    return false; 
  }

  // write top design 
  auto design = db_.getDesign(); 
  os << "digraph " << "p" << design << " ";
  os << "{\n";
  os << "rankdir=LR;\n";
  writeDesign(os, design);
  os << "}\n";

  os.close();
  */
  return true; 
}
/* remove-design
void GraphvizWriter::writeDesign(std::ofstream& os, Design const* design) const {
  if (design) {
    // declare instances 
    for (Database::IndexType j = 0; j < design->numInsts(); ++j) {
      Inst const* inst = design->getInst(j); 
      Cell const* inst_cell = inst->getCell(); 
      std::string name; 
      if (inst->getAttr()) {
        if (inst->getAttr()->getName().empty()) {
          std::ostringstream oss; 
          oss << inst_cell; 
          name = oss.str();
        } else {
          name = inst->getAttr()->getName();
        }
      } else { // in case split cell does not have a name 
        name = inst_cell->getName(); 
      }
      switch (inst_cell->getCellType()) {
        case CellType::kCell:
        case CellType::kIOPin:
          os << "p" << inst << "[label=\"" << name << "\"];" << "\n";
          break; 
        case CellType::kModule:
          if (design->getUniquified()) {
            os << "subgraph " << "cluster_p" << inst << " "; 
            os << "{\n";
            os << "label=\"" << name << "\";\n";
            writeDesign(os, inst->getDesign());
            os << "}\n";
          } else {
            os << "p" << inst << "[label=\"" << name << "\"];" << "\n";
          }
          break; 
        case CellType::kSplitCell:
          os << "p" << inst << "[label=\"" << name << "\"" << ", shape=box, color=\"blue\"" << "];" << "\n";
          break; 
        case CellType::kTerm:
          os << "p" << inst << "[label=\"" << name << "\"" << ", shape=box, color=\"green\"" << "];" << "\n";
          break; 
        default:
          ediPrint(kWarn, "ignore unknown cell type %s\n", toString(inst_cell->getCellType()).c_str());
          break; 
      }
    }

    // declare wires 
    for (Database::IndexType j = 0; j < design->numNets(); ++j) {
      Net const* net = design->getNet(j); 
      for (Database::IndexType ko = 0; ko < net->numPins(); ++ko) {
        Pin const* oterm = net->getPin(ko); 
        Inst const* oinst = oterm->getInst();
        // find output terms
        if (oterm->getAttr()->getTerm()->getSignalDirect() == SignalDirection::kOutput
            || oterm->getAttr()->getTerm()->getSignalDirect() == SignalDirection::kInout) {
          for (Database::IndexType ki = 0; ki < net->numPins(); ++ki) {
            Pin const* iterm = net->getPin(ki); 
            Inst const* iinst = iterm->getInst();
            // find input terms 
            if (iterm->getAttr()->getTerm()->getSignalDirect() == SignalDirection::kInput
                || iterm->getAttr()->getTerm()->getSignalDirect() == SignalDirection::kInout) {
              if (oterm != iterm) {
                std::ostringstream oss; 
                char const* delimiter = ""; 
                if (design->getUniquified() && oinst->getCell()->getCellType() == CellType::kModule) {
                  ediAssert(oinst->getDesign());
                  os << "p" << oinst->getDesign()->getPin(oterm->getAttr()->getTerm()->getId()); 
                  oss << "ltail=" << "cluster_p" << oinst; 
                  delimiter = ", ";
                } else {
                  os << "p" << oterm->getInst();
                }
                os << " -> ";
                if (design->getUniquified() && iinst->getCell()->getCellType() == CellType::kModule) {
                  ediAssert(iinst->getDesign());
                  os << "p" << iinst->getDesign()->getPin(iterm->getAttr()->getTerm()->getId()); 
                  oss << delimiter << "lhead=" << "cluster_p" << iinst;
                } else {
                  os << "p" << iterm->getInst();
                }
                if (!oss.str().empty()) {
                  os << "[" << oss.str() << "]";
                }
                os << ";\n";
              }
            }
          }
        }
      }
    }
  }
}
*/

EDI_END_NAMESPACE
