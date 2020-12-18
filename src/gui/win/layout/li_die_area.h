#ifndef LI_DIE_AREA_H
#define LI_DIE_AREA_H

#include <QImage>
#include <qmath.h>

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/io/write_def.h"
#include "db/util/array.h"
#include "db/util/property_definition.h"
#include "db/util/vector_object_var.h"
#include "graphics_scene.h"
#include "lgi_die_area.h"
#include "li_base.h"
#include "util/util.h"

namespace open_edi {
namespace gui {
class LI_DieArea : public LI_Base {
  public:
    explicit LI_DieArea(int* scale_factor);
    LI_DieArea(const LI_DieArea& other) = delete;
    LI_DieArea& operator=(const LI_DieArea& rhs) = delete;
    ~LI_DieArea();

    virtual void preDraw() override;
    LGI_DieArea* getGraphicItem();
    void         update();

  private:
    LGI_DieArea* item_{nullptr};
    QPen         pen_;
};
} // namespace gui
} // namespace open_edi

#endif