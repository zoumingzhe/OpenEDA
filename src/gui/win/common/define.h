#ifndef DEFINE_H
#define DEFINE_H


namespace open_edi {
namespace gui {

class EDAGui{

public:
    enum ActionType{

        ActionNone,

        ActionFileImportDesign,
        ActionFileSaveDesign,
        ActionFileSetPreference,
        ActionFileFindSelectObject,

        ActionEditUndo,
        ActionEditRedo,
        ActionEditHighLight,
        ActionViewZoomIn,
        ActionViewZoomOut
    };

    enum CursorShape{

        ArrowCursor,
        CrossCursor,
        HandCursor,
        WaitCursor
    };
};


}

}








#endif

