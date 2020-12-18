#ifndef LI_INSTANCES_H
#define LI_INSTANCES_H

#include <QPainter>
#include <qmath.h>
#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/io/write_def.h"
#include "db/util/array.h"
#include "db/util/property_definition.h"
#include "db/util/vector_object_var.h"
#include "graphics_scene.h"
#include "lgi_instances.h"
#include "li_base.h"
#include "util/util.h"

namespace open_edi {
namespace gui {
class LI_Instances : public LI_Base {
  public:
    explicit LI_Instances(int* scale_factor);
    LI_Instances(const LI_Instances& other) = delete;
    LI_Instances& operator=(const LI_Instances& rhs) = delete;
    ~LI_Instances();

    virtual void   preDraw() override;
    LGI_Instances* getGraphicItem();
    void           update();

  private:
    LGI_Instances* item_;
    QPen           pen_;
};
} // namespace gui
} // namespace open_edi

#endif