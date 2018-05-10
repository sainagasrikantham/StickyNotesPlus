#ifndef STICKYACTION_H
#define STICKYACTION_H

#include <QAction>
#include "StickyNote.h"

class CStickyAction : public QAction
{
    Q_OBJECT
public:
    /** Constructor */
    CStickyAction(int aActionId, const QString &aText, CStickyNote* aNote, QObject* aParent);

    /** This function returns the action id */
    int GetActionId();

    /** This function returns the associated note */
    CStickyNote* GetAssociatedNote();

private slots:
    /** This function displays the note associated with this action */
    void ActionTriggered();

    /** Called when the mouse is hovered above a note's title in the Notes' list */
    void HandleMouseHoverOverNote();

private:
    /** The identifier for the action */
    int iActionId;

    /** The Note the action is associated with */
    CStickyNote* iAssociatedNote;
};

#endif // STICKYACTION_H
