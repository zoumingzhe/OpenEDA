#ifndef LI_MANAGER_H
#define LI_MANAGER_H

#include <vector>

#include "li_die_area.h"
#include "li_instances.h"
#include "li_pins.h"

namespace open_edi {
namespace gui {
class LI_Manager {
  public:
    explicit LI_Manager(int* scale_factor);
    LI_Manager(const LI_Manager& other) = delete;
    LI_Manager& operator=(const LI_Manager& rhs) = delete;
    LI_Manager();
    ~LI_Manager();

    void                  preDrawAllItems();
    std::vector<LI_Base*> getLiList();

  private:
    std::vector<LI_Base*> li_vec_;
    template <typename LI>
    void addLi(int* scale_factor) {
        li_vec_.push_back(new LI(scale_factor));
    };
};
} // namespace gui
} // namespace open_edi

#endif