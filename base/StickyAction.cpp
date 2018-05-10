#include "StickyAction.h"
#include "StickyNotesPlus.h"
#include "StickyNote.h"
#include "StickyNotesDataMgr.h"
#include "StickyNotesUIMgr.h"

#include<vector>

using std::vector;

/** Constructor */
CStickyAction::CStickyAction(int aActionId, const QString &aText, CStickyNote* aNote, QObject *aParent):QAction(aText, aParent), iActionId(aActionId)
{
    iAssociatedNote = aNote;
}

/** This function returns the action id */
int CStickyAction::GetActionId()
{
    return iActionId;
}

/** This function returns the associated note */
CStickyNote* CStickyAction::GetAssociatedNote()
{
    return iAssociatedNote;
}

/** This function displays the note associated with this action */
void CStickyAction::ActionTriggered()
{
    if(iAssociatedNote->isHidden() && iAssociatedNote)
    {
        iAssociatedNote->show();
        CStickyNotesPlus::GetInstance()->GetUIMgr()->EvaluateShowNHideAll();
    }
}

/** Called when the mouse is hovered above a note's title in the Notes' list */
void CStickyAction::HandleMouseHoverOverNote()
{
    CStickyNotesUIMgr* uiMgr = CStickyNotesPlus::GetInstance()->GetUIMgr();

    if( uiMgr->GetCurrentlyHoveredMenu() != FAKE_MENU )
    {
        if(iAssociatedNote && iAssociatedNote->isHidden())
        {
            uiMgr->HideAllTriggered();
            iAssociatedNote->show();
            uiMgr->ShowCurrentlyHoveredNotesMenu();
        }
    }
}
