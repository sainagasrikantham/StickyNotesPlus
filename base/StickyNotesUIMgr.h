/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesUIMgr.h */
/** Purpose : This file contains the definition for the class CStickyNotesUIMgr which is primarily responsible to deal
              with all the UI componens of the project */

#ifndef CSTICKYNOTESUIMGR_H
#define CSTICKYNOTESUIMGR_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <vector>
#include "StickyAction.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QIcon;
class QLineEdit;
class QWidgetAction;
QT_END_NAMESPACE

using std::vector;

class CStickyNotesUIMgr : public QDialog
{
    Q_OBJECT
public:
    /** Creates the "My Notes" sub-menu items */
    void CreateMyNotes(int aNumberOfNotes);

    /** Registers the "actions" or signals and slots associated with different UI components */
    void RegisterActions();

    /** Constructor of the class */
    CStickyNotesUIMgr();

    /** Overriden function that enables / disables menu items from the systray context menu */
    void setVisible(bool visible);

    /** Destructor of the class */
    ~CStickyNotesUIMgr();

    /** Evaluates if the 'Hide All Notes' option of the systray should be active */
    void EvaluateShowNHideAll();

    /** Show the pop-up to start "time tracking" */
    void ShowTimeTrackingPopup();

    /** Shows the "All Notes" menu */
    void ShowCurrentlyHoveredNotesMenu();

    /** Returns the member variable iCurrentlyHoveredMenu */
    MyNotesMenus GetCurrentlyHoveredMenu();

public slots:
    /** Called when the user clicks on the systray icon */
    void IconActivated(QSystemTrayIcon::ActivationReason aReason);

    /** Called when the 'Hide All Notes' item from the systray context menu is selected */
    void HideAllTriggered();

    /** Called when the 'Show All Notes' item from the systray context menu is selected */
    void ShowAllTriggered();

    /** Called when the 'Show All Notes' item from the systray context menu is selected */
    void DeleteAllTriggered();

    /** Called when the 'Donate' item from the systray context menu is selected */
    void DonateTriggered();

    /** Called when the 'Preferences' item from the systray context menu is selected */
    void PreferencesTriggered();

    /** Called when the 'About' item from the systray context menu is selected */
    void AboutTriggered();

    /** Called when the 'Today's Reminders' item from the systray context menu is selected */
    void TodaysRemindersTriggered();

    /** Called when the 'Tomorrow's Reminders' item from the systray context menu is selected */
    void TomorrowsRemindersTriggered();

    /** Called when the 'All Reminders' item from the systray context menu is selected */
    void AllRemindersTriggered();

    /** Displays the Reminders of any given day of the week */
    bool ShowReminders(int dayOfWeek);

    /** Called when the 'Hour Note' item from the systray context menu is selected */
    void AddHourNoteTriggered();

    /** Called when the 'Date Note' item from the systray context menu is selected */
    void AddDateNoteTriggered();

    /** Called when the 'Generic' item from the systray context menu is selected */
    void AddGenericTriggered();

    /** Called when the 'Recurring DOW' item from the systray context menu is selected */
    void AddRecurringDOWTriggered();

    /** Called when the 'Quit' item from the systray context menu is selected */
    void SaveAndQuit();

    /** Called when the text in the "Search" text box is changed */
    void HandleSearchTextChange(QString);

    /** Called when the "Search" text box receives focus */
    void HandleSearchTextFocus(int, int);

    /** Called when the user hovers over the "All Notes" menu */
    void AllNotesHovered();

    /** Called when the user hovers over the "Date Notes" menu */
    void DateNotesHovered();

    /** Called when the user hovers over the "Hour Notes" menu */
    void HourNotesHovered();

    /** Called when the user hovers over the "Generic Notes" menu */
    void GenericNotesHovered();

    /** Called when the user hovers over the "Reminder Notes" menu */
    void ReminderNotesHovered();

private:
    /** Displays a pop up containing the login information of the user */
    void DisplayLoginInfo();

    /** Display the TO DO list */
    void DisplayToDoList();

private:
    /** Instance of the framework class needed to show a "system tray" icon */
    class CStickySystemTrayIcon* iSystemTrayIcon;

    /** The context menu associated with the system tray */
    QMenu* iSystemTrayMenu;
    QMenu* iAddNoteMenu;
    QMenu* iMyNotesMenu;
    QMenu* iRemindersMenu;

    /** "Action" items associated with every item of the context menu */
    QAction* iDonateAction;
    QAction* iShowAllNotes;
    QAction* iDeleteAllNotes;
    QAction* iHideAllNotes;
    QAction* iQuitAction;
    QAction* iPreferencesAction;
    QAction* iAboutAction;
    QAction* iTodaysRemindersAction;
    QAction* iTomorrowsRemindersAction;
    QAction* iAllRemindersAction;

    /** Actions associated with the submenu 'Add Note' */
    QAction* iAddDateNoteAction;
    QAction* iAddHourNoteAction;
    QAction* iAddGenericNoteAction;
    QAction* iAddRecurringDOWNoteAction;

    /** "My Notes" menus */
    QMenu* iMyNotesAllNotesMenu;
    QMenu* iMyNotesDateNotesMenu;
    QMenu* iMyNotesHourNotesMenu;
    QMenu* iMyNotesGenericNotesMenu;
    QMenu* iMyNotesReminderNotesMenu;

    /** The "Search" functionality UI components */
    QLineEdit*     iSearchLineEdit;
    QWidgetAction* iSearchWidgetAction;

    vector <CStickyAction*> iStickyActions;

    MyNotesMenus iCurrentlyHoveredMenu;
};

/** CStickySystemTrayIcon declaration */
class CStickySystemTrayIcon : public QSystemTrayIcon
{
public:
    /** Constructor */
    CStickySystemTrayIcon(QWidget* aParent);

    /** This function catches all the events sent to this widget */
    bool event(QEvent *event);
};

#endif // CSTICKYNOTESUIMGR_H
