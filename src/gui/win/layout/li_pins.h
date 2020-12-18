#ifndef LI_PINS_H
#define LI_PINS_H

#include <QPainter>
#include <qmath.h>
#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/io/write_def.h"
#include "db/util/array.h"
#include "db/util/property_definition.h"
#include "db/util/vector_object_var.h"
#include "graphics_scene.h"
#include "lgi_pins.h"
#include "li_base.h"
#include "util/util.h"

namespace open_edi {
namespace gui {
class LI_Pins : public LI_Base {
  public:
    explicit LI_Pins(int* scale_factor);
    LI_Pins(const LI_Pins& other) = delete;
    LI_Pins& operator=(const LI_Pins& rhs) = delete;
    ~LI_Pins();

    virtual void preDraw() override;
    LGI_Pins*    getGraphicItem();
    void         update();

  private:
    LGI_Pins* item_;
    QPen      pen_;
    void      __drawPins(open_edi::db::Inst& ins);
};
} // namespace gui
} // namespace open_edi

#endif