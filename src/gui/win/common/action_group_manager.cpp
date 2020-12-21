#include "action_group_manager.h"
#include <QActionGroup>

namespace open_edi {
namespace gui {

ActionGroupManager::ActionGroupManager(QObject* parent)
    : QObject (parent)
    , file_(new QActionGroup(this))
    , edit_(new QActionGroup(this))
    , view_(new QActionGroup(this))
    , small_tool_(new QActionGroup(this))
    , report_(new QActionGroup(this))
    , tool_(new QActionGroup(this))
    , floorplan_(new QActionGroup(this))
    , partition_(new QActionGroup(this))
    , wire_edit_(new QActionGroup(this))
    , wire_operate_(new QActionGroup(this))
    , design_view_(new QActionGroup(this))
    , ruler_(new QActionGroup(this))
    , ruler_shape_(new QActionGroup(this))
    , ruler_snap_(new QActionGroup(this))
{
    file_->setObjectName(tr("File"));
    edit_->setObjectName(tr("Edit"));
    view_->setObjectName(tr("View"));
    small_tool_->setObjectName(tr("SmallTool"));
    report_->setObjectName(tr("Report"));
    tool_->setObjectName(tr("Tool"));
    floorplan_->setObjectName(tr("FloorPlan"));
    partition_->setObjectName(tr("Partition"));
    wire_edit_->setObjectName(tr("WireEdit"));
    wire_operate_->setObjectName(tr("WireOperate"));
    design_view_->setObjectName(tr("DesignView"));
    ruler_->setObjectName(tr("Ruler"));
    ruler_shape_->setObjectName(tr("RulerShape"));
    ruler_snap_->setObjectName(tr("RulerSnap"));

    QList<QActionGroup*> agList = findChildren<QActionGroup*>();
    foreach(auto group, agList)
    {
        group->setExclusive(false);
        if(group->objectName() != tr("File")){
 //           connect(parent, SIGNAL(windowChanged(bool)), group, SLOT(setEnable(bool)));
        }
    }
}

QList<QActionGroup*> ActionGroupManager::toolGroups()
{
    QList<QActionGroup*> agList;
    agList << file_
            << edit_
            << view_
            << small_tool_
            << report_
            << tool_
            << floorplan_
            << partition_
            << wire_edit_
            << wire_operate_
            << design_view_
            << ruler_
            << ruler_shape_
            << ruler_snap_;

    return agList;
}

QList<QActionGroup*> ActionGroupManager::commonGroups()
{
    QList<QActionGroup*> agList;
    agList << file_
            << edit_
            << view_
            << small_tool_
            << report_;

    return agList;
}

QMap<QString, QActionGroup*> ActionGroupManager::getAllGroups()
{
    QMap<QString, QActionGroup*> agMap;

    QList<QActionGroup*> agList = findChildren<QActionGroup*>();
    foreach(auto ag, agList){
        agMap[ag->objectName()] = ag;
    }

    return agMap;
}


}
}


