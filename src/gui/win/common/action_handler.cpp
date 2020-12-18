#include "action_handler.h"
#include "../actions/action_edit_undo.h"
#include "../actions/action_view_zoomin.h"
#include "../actions/action_import_design.h"

namespace open_edi {
namespace gui {

ActionHandler::ActionHandler(QObject *parent)
    : QObject(parent)
    , view_(nullptr)
{

}

ActionHandler::~ActionHandler()
{

}

void ActionHandler::setView(QGraphicsView *view){
    view_ = view;
}

ActionAbstract* ActionHandler::setCurrentAction(EDAGui::ActionType type)
{
    ActionAbstract* action = nullptr;

    if(!view_){
        return nullptr;
    }

    switch (type)
    {

    case EDAGui::ActionFileImportDesign:
        action = new ActionFileImportDesign(*view_, this);
        break;

    case EDAGui::ActionFileSaveDesign:
        break;

    case EDAGui::ActionFileSetPreference:
        break;

    case EDAGui::ActionEditUndo:
        action = new ActionEditUndo(*view_, this);
        break;

    case EDAGui::ActionEditRedo:
        break;

    case EDAGui::ActionEditHighLight:
        break;

    case EDAGui::ActionViewZoomIn:
        action = new ActionViewZoomin(*view_, this);
        break;

    case EDAGui::ActionViewZoomOut:
        break;

    default:
        break;
    }

    return action;
}

ActionAbstract* ActionHandler::getCurrentAction()
{

}

void ActionHandler::slotImportDesign()
{
    setCurrentAction(EDAGui::ActionFileImportDesign);
}


void ActionHandler::slotSaveDesign()
{
    setCurrentAction(EDAGui::ActionFileSaveDesign);
}

void ActionHandler::slotSetPreference()
{
    setCurrentAction(EDAGui::ActionFileSetPreference);
}

void ActionHandler::slotFindSelectObject()
{
    setCurrentAction(EDAGui::ActionFileFindSelectObject);
}

void ActionHandler::slotEditUndo()
{
    setCurrentAction(EDAGui::ActionEditUndo);
}

void ActionHandler::slotEditRedo()
{
    setCurrentAction(EDAGui::ActionEditRedo);
}

void ActionHandler::slotEditHighLight()
{
    setCurrentAction(EDAGui::ActionEditHighLight);
}

void ActionHandler::slotViewZoomIn()
{
    setCurrentAction(EDAGui::ActionViewZoomIn);
}

void ActionHandler::slotViewZoomOut()
{
    setCurrentAction(EDAGui::ActionViewZoomOut);
}

}
}
