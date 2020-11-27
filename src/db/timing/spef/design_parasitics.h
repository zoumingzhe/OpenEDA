/** 
 * @file design_parasitics.h
 * @date 2020-11-02
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TIMING_SPEF_DESIGN_PARASITICS_H_
#define EDI_DB_TIMING_SPEF_DESIGN_PARASITICS_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector> 

#include "db/core/object.h"
#include "db/util/array.h"
#include "util/data_traits.h"

namespace open_edi {
namespace db {

class DesignParasitics : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    DesignParasitics();

    /// @brief destructor
    ~DesignParasitics();

    /// @brief constructor
    DesignParasitics(Object *owner, IndexType id);

    /// @brief copy constructor
    DesignParasitics(DesignParasitics const &rhs);

    /// @brief move constructor
    DesignParasitics(DesignParasitics &&rhs) noexcept;

    /// @brief copy assignment
    DesignParasitics &operator=(DesignParasitics const &rhs);

    /// @brief move assignment
    DesignParasitics &operator=(DesignParasitics &&rhs) noexcept;

    /// @brief add NetsParasitics from a SPEF file
    void addSpefMap(ObjectId cellId, SymbolIndex spefFileIdx) { spef_map_[cellId] = spefFileIdx; }
    void addParasiticsMap(ObjectId cellId, ObjectId netsParasitcsId) { parasitics_map_[cellId] = netsParasitcsId; }

    //std::vector<ParasiticNode *>& get_net_parasitic_nodes(Net *net) {} //To add
    //std::vector<ParasiticDevice *>& get_net_capacitors(Net *net) {} //To add
    //std::vector<ParasiticDevice *>& get_net_resistors(Net *net) {} //To add

    void setSpefField(unsigned spefField) { spefField_ = spefField; }
    uint8_t getSpefField() const { return spefField_; }

    const std::unordered_map<ObjectId, SymbolIndex>& getSpefMap() const { return spef_map_; }
    const std::unordered_map<ObjectId, ObjectId>& getParasiticsMap() const { return parasitics_map_; }

  protected:
    /// @brief copy object
    void copy(DesignParasitics const &rhs);
    /// @brief move object
    void move(DesignParasitics &&rhs);
    /// @brief overload output stream
    friend std::ofstream &operator<<(std::ofstream &os, DesignParasitics const &rhs);

  private:
    /// SPEF Design Name ObjectId and file path map
    std::unordered_map<ObjectId, SymbolIndex> spef_map_;
    /// SPEF Design Name ObjectId and NetsParasitics ObjectId map
    std::unordered_map<ObjectId, ObjectId> parasitics_map_;
    /// The value to determine which value to read in from Tiplet value in SPEF
    uint8_t spefField_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TIMING_SPEF_DESIGN_PARASITICS_H_
