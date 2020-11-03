/* @file  fill.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_PHYSICAL_FILL_H_
#define EDI_DB_PHYSICAL_FILL_H_

#include <string>
#include <vector>

#include "db/core/object.h"
#include "db/tech/type_def.h"
#include "db/tech/via_master.h"
#include "util/point.h"

namespace open_edi {
namespace db {

/*
 * [FILLS  numFills ;
 *     [- LAYER  layerName  [+ MASK  maskNum ] [+ OPC]
 *         {RECT  pt   pt  | POLYGON  pt   pt   pt  ...} ... ;] ...
 *     [- VIA  viaName  [+ MASK  viaMaskNum ] [+ OPC]  pt  ... ;] ...
 * END FILLS]
 */

class Fill : public Object {
  public:
    Fill();
    ~Fill();

    bool getIsLayer() const;
    void setIsLayer(bool is_layer);
    bool getIsVia() const;
    void setIsVia(bool is_via);
    bool getIsOpc() const;
    void setIsOpc(bool is_opc);
    bool getIsRect(Int32 id) const;
    Int32 getLayerId() const;
    void setLayerId(Int32 id);
    Int32 getLayerMask() const;
    void setLayerMask(Int32 mask);
    Int32 getViaBotMask() const;
    void setViaBotMask(Int32 mask);
    Int32 getViaCutMask() const;
    void setViaCutMask(Int32 mask);
    Int32 getViaTopMask() const;
    void setViaTopMask(Int32 mask);
    std::vector<std::vector<Point> > *getPointsArray();
    void addPoints(const std::vector<Point> &points);
    ViaMaster *getVia() const;
    void setVia(ViaMaster *via);

  private:
    Bits is_layer_ : 1;
    Bits is_via_ : 1;
    Bits is_opc_ : 1;
    Bits unused_ : 29;

    Int32 layer_id_;
    // is_layer, mask_[0] is used; is_via, mask_ 0/1/2 stands for bot/cut/top
    Int32 mask_[3];
    // [RECT pt pt] ..., points_array_[n][2], multiple rectangles(two points)
    // [POLYGON pt ...] ..., points_array_[n][m], multiple polygons and mulitple
    // points in each polygon VIA pt ..., points_array_[0[[n], one via and
    // multiple points
    std::vector<std::vector<Point> > points_array_;
    ViaMaster *via_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_FILL_H_
