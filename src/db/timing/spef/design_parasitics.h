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
#include "parasitic_device.h"

namespace open_edi {
namespace db {


class DesignParasitics : public Object {
  public:
    using BaseType = Object;
    using CellNetParaMap = std::unordered_map<ObjectId, ObjectId>;

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

    std::vector<std::vector<OptParaNode>> getOptNetParasiticNodes(ObjectId net_id);
    // DNetParasitics* getNetDNetCaps(Net *net) const;
    //std::vector<ParasiticDevice *>& get_net_capacitors(Net *net) {} //To add
    //std::vector<ParasiticDevice *>& get_net_resistors(Net *net) {} //To add

    void addCellId(ObjectId cellId);
    //Get cells vector id
    ObjectId getCells() const { return cell_vec_id_; }

    void addSpef(SymbolIndex index);
    //Get Spef SymbolIndex vector id
    ObjectId getSpefs() const { return spef_vec_id_; }

    void addNetsParasitics(ObjectId netsPara);
    //Get NetsParasitics vector id
    ObjectId getNetsParasitics() const { return netsparasitics_vec_id_; }

    void setSpefField(unsigned spefField) { spef_field_ = spefField; }
    uint8_t getSpefField() const { return spef_field_; }

  protected:
    /// @brief copy object
    void copy(DesignParasitics const &rhs);
    /// @brief move object
    void move(DesignParasitics &&rhs);
    /// @brief overload output stream
    friend std::ofstream &operator<<(std::ofstream &os, DesignParasitics const &rhs);

  private:
    ObjectId cell_vec_id_;
    //Spef SymbolIndex vector id
    ObjectId spef_vec_id_;
    /// ArrayObject<Netsparasitics*> *
    ObjectId netsparasitics_vec_id_;
    /// The value to determine which value to read in from triplet value in SPEF
    uint8_t spef_field_;
    /// Cell indexed Net to DNetParasitics map
    mutable CellNetParaMap *net_detailed_para_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TIMING_SPEF_DESIGN_PARASITICS_H_
