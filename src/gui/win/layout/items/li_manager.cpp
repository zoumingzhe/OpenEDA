#include "li_manager.h"

namespace open_edi {
namespace gui {
LI_Manager::LI_Manager(int* scale_factor) {
    addLi<LI_DieArea>(scale_factor);
    addLi<LI_Instances>(scale_factor);
    addLi<LI_Pins>(scale_factor);
}

LI_Manager::~LI_Manager() {
}

void LI_Manager::preDrawAllItems() {
    for (auto item : li_vec_) {
        item->preDraw();
    }
}

std::vector<LI_Base*> LI_Manager::getLiList() {
    return li_vec_;
}
} // namespace gui
} // namespace open_edi