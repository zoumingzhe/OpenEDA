#include <iostream>
#include <sstream>
#include <string>
#include "util/util.h"
#include "tool_bar.h"

namespace open_edi {
namespace gui {

ToolBar::ToolBar(const QString &title, QWidget *parent):QToolBar(parent)
{
   setWindowTitle(title);
   setObjectName(title);

   setIconSize(QSize(32,32));
   std::stringstream png_path;
   png_path << open_edi::util::getInstallPath() << "src/gui/res";
   std::string png_name = png_path.str() + "/zoomIn.png";
   addAction(QPixmap(png_name.c_str()), tr("Zoom In"));
   png_name = png_path.str() + "/zoomout.png";
   addAction(QPixmap(png_name.c_str()), tr("Zoom Out"));
   png_name = png_path.str() + "/zoomfit.png";
   addAction(QPixmap(png_name.c_str()), tr("Zoom Fit"));
   png_name = png_path.str() + "/ruler.png";
   addAction(QPixmap(png_name.c_str()), tr("Ruler"));
   png_name = png_path.str() + "/design.png";
   addAction(QPixmap(png_name.c_str()), tr("Design"));
   png_name = png_path.str() + "/fix.png";
   addAction(QPixmap(png_name.c_str()), tr("Fix"));
   png_name = png_path.str() + "/floorplan.png";
   addAction(QPixmap(png_name.c_str()), tr("Floorplan Toolbox"));
   png_name = png_path.str() + "/redo.png";
   addAction(QPixmap(png_name.c_str()), tr("Redo"));
   png_name = png_path.str() + "/undo.png";
   addAction(QPixmap(png_name.c_str()), tr("Undo"));
   png_name = png_path.str() + "/move.png";
   addAction(QPixmap(png_name.c_str()), tr("Move"));
   png_name = png_path.str() + "/redraw.png";
   addAction(QPixmap(png_name.c_str()), tr("Redraw"));
   png_name = png_path.str() + "/cutwire.png";
   addAction(QPixmap(png_name.c_str()), tr("Cut Wire"));
   png_name = png_path.str() + "/zoomIn.png";
   addAction(QPixmap(png_name.c_str()), tr("Zoom In"));
   png_name = png_path.str() + "/unfix.png";
   addAction(QPixmap(png_name.c_str()), tr("Unfix"));
}

ToolBar::~ToolBar()
{
}

}
}
