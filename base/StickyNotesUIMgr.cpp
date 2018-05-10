/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesUIMgr.cpp */
/** Purpose : This file contains the definition for the class CStickyNotesUIMgr which is primarily responsible to deal
              with all the UI componens of the project */

#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QString>
#include <QApplication>
#include <QLineEdit>
#include <QWidgetAction>
#include <cstdlib>
#include <vector>
#include <fstream>

#include "StickyNotesUIMgr.h"
#include "StickyNotesLoginMgr.h"
#include "StickyNote.h"
#include "StickyNotesPlus.h"
#include "StickyNotesDataMgr.h"
#include "StickyToDoList.h"
#include "StickyAction.h"

using std::vector;
using std::fstream;
using std::ios;

/** Constructor of the class */
CStickyNotesUIMgr::CStickyNotesUIMgr()
{
    /** Creating "actions" */
    iQuitAction                = new QAction( tr("&Quit Sticky Notes Plus") , this );
    iShowAllNotes              = new QAction( tr("&Show All Notes")         , this );
    iDeleteAllNotes            = new QAction( tr("&Delete All Notes")       , this );
    iHideAllNotes              = new QAction( tr("&Hide All Notes")         , this );
    iDonateAction              = new QAction( tr("&Donate")                 , this );
    iAddDateNoteAction         = new QAction( tr("&Date Note")              , this );
    iAddHourNoteAction         = new QAction( tr("&Hour Note")              , this );
    iAddGenericNoteAction      = new QAction( tr("&Generic Note")           , this );
    iAddRecurringDOWNoteAction = new QAction( tr("&Recurring DOW Note")     , this );
    iPreferencesAction         = new QAction( tr("&Preferences")            , this );
    iAboutAction               = new QAction( tr("About")                   , this );
    iTodaysRemindersAction     = new QAction( tr("Today's")                 , this );
    iTomorrowsRemindersAction  = new QAction( tr("Tomorrow's")              , this );
    iAllRemindersAction        = new QAction( tr("All")                     , this );

    iQuitAction->setIcon(CStickyNotesPlus::GetQuitIcon());
    iShowAllNotes->setIcon(CStickyNotesPlus::GetShowNotesIcon());
    iDeleteAllNotes->setIcon(CStickyNotesPlus::GetDeleteIcon());
    iDonateAction->setIcon(CStickyNotesPlus::GetDonateIcon());
    iPreferencesAction->setIcon(CStickyNotesPlus::GetPreferencesIcon());
    iAboutAction->setIcon(CStickyNotesPlus::GetInfoIcon());

    iHideAllNotes->setEnabled(false);

    /** Creating the system tray context menu */
    iSystemTrayMenu = new QMenu(this);

    iAddNoteMenu = iSystemTrayMenu->addMenu(tr("&Create New Note"));
    iAddNoteMenu->addAction(iAddDateNoteAction);
    iAddNoteMenu->addSeparator();
    iAddNoteMenu->addAction(iAddHourNoteAction);
    iAddNoteMenu->addSeparator();
    iAddNoteMenu->addAction(iAddGenericNoteAction);
    iAddNoteMenu->addSeparator();
    iAddNoteMenu->addAction(iAddRecurringDOWNoteAction);
    iSystemTrayMenu->addSeparator();
    iAddNoteMenu->setIcon(CStickyNotesPlus::GetNewNoteIcon());

    iSystemTrayMenu->addAction(iShowAllNotes);
    iSystemTrayMenu->addAction(iHideAllNotes);
    iSystemTrayMenu->addSeparator();

    iSystemTrayMenu->addAction(iDeleteAllNotes);
    iSystemTrayMenu->addSeparator();

    iSystemTrayMenu->addAction(iDonateAction);
    iSystemTrayMenu->addSeparator();

    iSystemTrayMenu->addAction(iPreferencesAction);
    iSystemTrayMenu->addSeparator();

    iSystemTrayMenu->addAction(iAboutAction);
    iSystemTrayMenu->addSeparator();

    iRemindersMenu = iSystemTrayMenu->addMenu(tr("&Reminders"));
    iRemindersMenu->addAction(iTodaysRemindersAction);
    iRemindersMenu->addSeparator();
    iRemindersMenu->addAction(iTomorrowsRemindersAction);
    iRemindersMenu->addSeparator();
    iRemindersMenu->addAction(iAllRemindersAction);
    iRemindersMenu->addSeparator();
    iSystemTrayMenu->addSeparator();

    iMyNotesMenu = iSystemTrayMenu->addMenu(tr("&My Notes"));
    iMyNotesMenu->setIcon(CStickyNotesPlus::GetStickyNotesIcon());
    iSystemTrayMenu->addSeparator();

    iSystemTrayMenu->addAction(iQuitAction);

    /** Sub-Menus for "My Notes" */
    iMyNotesAllNotesMenu      = new QMenu( tr("&All Notes") , iMyNotesMenu );
    iMyNotesDateNotesMenu     = new QMenu( tr("&Date")      , iMyNotesMenu );
    iMyNotesHourNotesMenu     = new QMenu( tr("&Hour")      , iMyNotesMenu );
    iMyNotesGenericNotesMenu  = new QMenu( tr("&Generic")   , iMyNotesMenu );
    iMyNotesReminderNotesMenu = new QMenu( tr("&Reminders") , iMyNotesMenu );

    /** The "Search" functionality UI components */
    iSearchLineEdit     = new QLineEdit(this);
    iSearchWidgetAction = new QWidgetAction(this);
    iSearchLineEdit->setText(tr("Search..."));
    iSearchWidgetAction->setIcon(CStickyNotesPlus::GetSearchIcon());
    iSearchWidgetAction->setDefaultWidget(iSearchLineEdit);

    /** Creating the system tray icon */
    iSystemTrayIcon = new CStickySystemTrayIcon(this);
    iSystemTrayIcon->setToolTip(tr("Sticky Notes Plus\nVersion 0.5.1"));

    iSystemTrayIcon->setContextMenu(iSystemTrayMenu);
    iSystemTrayIcon->setIcon(CStickyNotesPlus::GetStickyNotesIcon());
    iSystemTrayIcon->show();

    this->setWindowIcon(CStickyNotesPlus::GetStickyNotesIcon());

    iCurrentlyHoveredMenu = FAKE_MENU;
}

/** Registers the "actions" or signals and slots associated with different UI components */
void CStickyNotesUIMgr::RegisterActions()
{
    /** Actions associated with the systray menu for creating a "New Note" */
    connect( iAddDateNoteAction,         SIGNAL(triggered()), this, SLOT(AddDateNoteTriggered())      );
    connect( iAddHourNoteAction,         SIGNAL(triggered()), this, SLOT(AddHourNoteTriggered())      );
    connect( iAddGenericNoteAction,      SIGNAL(triggered()), this, SLOT(AddGenericTriggered())       );
    connect( iAddRecurringDOWNoteAction, SIGNAL(triggered()), this, SLOT(AddRecurringDOWTriggered())  );

    /** Actions associated with a "Sticky Note" window */
    connect( iHideAllNotes,             SIGNAL(triggered()), this, SLOT(HideAllTriggered())            );
    connect( iShowAllNotes,             SIGNAL(triggered()), this, SLOT(ShowAllTriggered())            );
    connect( iDeleteAllNotes,           SIGNAL(triggered()), this, SLOT(DeleteAllTriggered())          );
    connect( iDonateAction,             SIGNAL(triggered()), this, SLOT(DonateTriggered())             );
    connect( iPreferencesAction,        SIGNAL(triggered()), this, SLOT(PreferencesTriggered())        );
    connect( iAboutAction,              SIGNAL(triggered()), this, SLOT(AboutTriggered())              );
    connect( iTodaysRemindersAction,    SIGNAL(triggered()), this, SLOT(TodaysRemindersTriggered())    );
    connect( iTomorrowsRemindersAction, SIGNAL(triggered()), this, SLOT(TomorrowsRemindersTriggered()) );
    connect( iAllRemindersAction,       SIGNAL(triggered()), this, SLOT(AllRemindersTriggered())       );
    connect( iQuitAction,               SIGNAL(triggered()), this, SLOT(SaveAndQuit())                 );

    /** Actions associated with the "My Notes" systray menu */
    // To do: The following actions are associated with "preview" of a note, that functionality is not working as expected.
    /*connect( iMyNotesAllNotesMenu,      SIGNAL(hovered(QAction *)), this, SLOT(AllNotesHovered())      );
    connect( iMyNotesDateNotesMenu,     SIGNAL(hovered(QAction *)), this, SLOT(DateNotesHovered())     );
    connect( iMyNotesHourNotesMenu,     SIGNAL(hovered(QAction *)), this, SLOT(HourNotesHovered())     );
    connect( iMyNotesGenericNotesMenu,  SIGNAL(hovered(QAction *)), this, SLOT(GenericNotesHovered())  );
    connect( iMyNotesReminderNotesMenu, SIGNAL(hovered(QAction *)), this, SLOT(ReminderNotesHovered()) );*/

    /** Actions associated with the "Search" functionality */
    connect( iSearchLineEdit, SIGNAL(textChanged(QString)),            this, SLOT(HandleSearchTextChange(QString)) );
    connect( iSearchLineEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(HandleSearchTextFocus(int, int)) );

    /** Actions associatd with the Systray Icon */
    connect(iSystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
}

/** Destructor of the class */
CStickyNotesUIMgr::~CStickyNotesUIMgr()
{
    /** Actions */
    if(iQuitAction)
    {
        delete iQuitAction;
        iQuitAction = NULL;
    }
    if(iShowAllNotes)
    {
        delete iShowAllNotes;
        iShowAllNotes = NULL;
    }
    if(iHideAllNotes)
    {
        delete iHideAllNotes;
        iHideAllNotes = NULL;
    }
    if(iDonateAction)
    {
        delete iDonateAction;
        iDonateAction = NULL;
    }
    if(iAddDateNoteAction)
    {
        delete iAddDateNoteAction;
        iAddDateNoteAction = NULL;
    }
    if(iAddHourNoteAction)
    {
        delete iAddHourNoteAction;
        iAddHourNoteAction = NULL;
    }
    if(iAddGenericNoteAction)
    {
        delete iAddGenericNoteAction;
        iAddGenericNoteAction = NULL;
    }
    if(iAddRecurringDOWNoteAction)
    {
        delete iAddRecurringDOWNoteAction;
        iAddRecurringDOWNoteAction = NULL;
    }
    if(iPreferencesAction)
    {
        delete iPreferencesAction;
        iPreferencesAction = NULL;
    }

    /** Menus */
    if(iSystemTrayMenu)
    {
        delete iSystemTrayMenu;
        iSystemTrayMenu = NULL;
    }

    /** Systray Icon */
    if(iSystemTrayIcon)
    {
        delete iSystemTrayIcon;
        iSystemTrayIcon = NULL;
    }
}

/** Creates the "my notes" sub-menu items */
void CStickyNotesUIMgr::CreateMyNotes(int aNumberOfNotes)
{
    // clearing whatever we have so far
    iStickyActions.clear();
    iMyNotesMenu->clear();
    iMyNotesHourNotesMenu->clear();
    iMyNotesDateNotesMenu->clear();
    iMyNotesGenericNotesMenu->clear();
    iMyNotesReminderNotesMenu->clear();
    iMyNotesAllNotesMenu->clear();

    iMyNotesMenu->addMenu(iMyNotesDateNotesMenu);
    iMyNotesMenu->addSeparator();
    iMyNotesMenu->addMenu(iMyNotesGenericNotesMenu);
    iMyNotesMenu->addSeparator();
    iMyNotesMenu->addMenu(iMyNotesHourNotesMenu);
    iMyNotesMenu->addSeparator();
    iMyNotesMenu->addMenu(iMyNotesReminderNotesMenu);
    iMyNotesMenu->addSeparator();
    iMyNotesMenu->addMenu(iMyNotesAllNotesMenu);

    bool atleastOneHourNote     = false;
    bool atleastOneDateNote     = false;
    bool atleastOneGenericNote  = false;
    bool atleastOneReminderNote = false;

    if(aNumberOfNotes > 0)
    {
        vector<CStickyNote*>::iterator notesBeginIterator = CStickyNotesPlus::GetInstance()->GetDataMgr()->GetNotesBeginIterator();
        
        for(int index=0; index<aNumberOfNotes; ++index)
        {
            CStickyAction* stickyAction = new CStickyAction(index, (*notesBeginIterator)->GetTitle(), (*notesBeginIterator), this);
            iStickyActions.push_back(stickyAction);
            (*notesBeginIterator)->SetAction(stickyAction);
            stickyAction->setIcon(CStickyNotesPlus::GetStickyNotesIcon());
            // To do: The "preview" note functionality is not working as expected.
            //connect( stickyAction, SIGNAL(hovered()), stickyAction, SLOT(HandleMouseHoverOverNote()) );
            switch( (*notesBeginIterator)->GetNoteType() )
            {
            case STICKY_EXPIRY_IN_HOURS:
                iMyNotesHourNotesMenu->addAction(stickyAction);
                atleastOneHourNote = true;
                break;
            case STICKY_EXPIRY_ON_DATE:
                iMyNotesDateNotesMenu->addAction(stickyAction);
                atleastOneDateNote = true;
                break;
            case STICKY_GENERIC_NOTE:
                iMyNotesGenericNotesMenu->addAction(stickyAction);
                atleastOneGenericNote = true;
                break;
            case STICKY_RECURRING_DOW:
                iMyNotesReminderNotesMenu->addAction(stickyAction);
                atleastOneReminderNote = true;
                break;
            }

            iMyNotesAllNotesMenu->addAction(stickyAction);

            connect(stickyAction, SIGNAL(triggered()), stickyAction, SLOT(ActionTriggered()));
            ++notesBeginIterator;
        }

        iMyNotesAllNotesMenu->addAction(iSearchWidgetAction);
    }
    else
    {
        iMyNotesAllNotesMenu->addAction(new CStickyAction(0, tr("None"), NULL, this));
    }

    if(!atleastOneHourNote)
        iMyNotesHourNotesMenu->addAction     ( new CStickyAction(0, tr("None"), NULL, this) );

    if(!atleastOneDateNote)
        iMyNotesDateNotesMenu->addAction     ( new CStickyAction(0, tr("None"), NULL, this) );

    if(!atleastOneGenericNote)
        iMyNotesGenericNotesMenu->addAction  ( new CStickyAction(0, tr("None"), NULL, this) );

    if(!atleastOneReminderNote)
        iMyNotesReminderNotesMenu->addAction ( new CStickyAction(0, tr("None"), NULL, this) );
}

/** Displays a pop up containing the login information of the user */
void CStickyNotesUIMgr::DisplayLoginInfo()
{
    QString loginDuration = CStickyNotesLoginMgr::GetInstance()->GetLoginDuration();
    iSystemTrayIcon->showMessage(tr("Login Information"),
                                 loginDuration, QSystemTrayIcon::Information);
}

/** Display the TO DO list */
void CStickyNotesUIMgr::DisplayToDoList()
{
    CStickyToDoList* stickyTodoList = CStickyNotesPlus::GetInstance()->GetToDoList();

    stickyTodoList->SetTimerInactive();
    if(stickyTodoList->isVisible())
    {
        stickyTodoList->hide();
        stickyTodoList->SaveAllTasks();
    }
    else
    {
        stickyTodoList->show();
    }
}

/** Overriden function that enables / disables menu items from the systray context menu */
void CStickyNotesUIMgr::setVisible(bool visible)
{
    QDialog::setVisible(visible);
}

/** Called when the user clicks on the systray icon */
void CStickyNotesUIMgr::IconActivated(QSystemTrayIcon::ActivationReason aReason)
{
    switch (aReason) {
    case QSystemTrayIcon::Trigger:
        DisplayLoginInfo();
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        DisplayToDoList();
        break;
    default:
        ;
    }
}

/** Called when the 'Hide All Notes' item from the systray context menu is selected */
void CStickyNotesUIMgr::HideAllTriggered()
{
    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();

    vector<CStickyNote*>::iterator notesBeginIterator = dataMgr->GetNotesBeginIterator();
    vector<CStickyNote*>::iterator notesEndIterator = dataMgr->GetNotesEndIterator();

    while(notesBeginIterator != notesEndIterator)
    {
        (*notesBeginIterator)->hide();
        ++notesBeginIterator;
    }
    iHideAllNotes->setEnabled(false);
    iShowAllNotes->setEnabled(true);

    /** Save the notes, just in case anything was modified */
    dataMgr->SaveAllNotes();
}

/** Called when the 'Show All Notes' item from the systray context menu is selected */
void CStickyNotesUIMgr::ShowAllTriggered()
{
    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();

    vector<CStickyNote*>::iterator notesBeginIterator = dataMgr->GetNotesBeginIterator();
    vector<CStickyNote*>::iterator notesEndIterator = dataMgr->GetNotesEndIterator();

    if(notesBeginIterator == notesEndIterator)
    {
        QMessageBox::information(this, tr("Sticky Notes Plus"),
                                 tr("You haven't created any notes yet. Get started!"));
        return;
    }

    while(notesBeginIterator != notesEndIterator)
    {
        (*notesBeginIterator)->show();
        ++notesBeginIterator;
    }

    iHideAllNotes->setEnabled(true);
    iShowAllNotes->setEnabled(false);
}

/** Called when the 'Show All Notes' item from the systray context menu is selected */
void CStickyNotesUIMgr::DeleteAllTriggered()
{
    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();

    if(dataMgr->GetNumberOfNotes() > 0)
    {
        int keepIt = 0;

        keepIt = QMessageBox::question(this, tr("Sticky Notes Plus"),
                                       tr("This action cannot be reverted. Continue?"),
                                       tr("Yes, go ahead"),
                                       tr("No, hold on!!"), 0, 0, 1);

        if(!keepIt) /** Wishes to delete the notes */
        {
            /** Clearing the file */
            fstream binaryFile("StickyNotesPlus.dat",ios::out|ios::binary);
            binaryFile.close();

            /** Clearing the vector */
            dataMgr->DeleteAllNotes();
            CreateMyNotes(dataMgr->GetNumberOfNotes());
        }

        iHideAllNotes->setEnabled(false);
        iShowAllNotes->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this, tr("Sticky Notes Plus"),
                                 tr("Sorry but, there are no notes to delete!"));
    }
}

/** Called when the 'Hour Note' item from the systray context menu is selected */
void CStickyNotesUIMgr::AddHourNoteTriggered()
{
    if(!iHideAllNotes->isEnabled())
        iHideAllNotes->setEnabled(true);

    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();
    dataMgr->AddNote(STICKY_EXPIRY_IN_HOURS);

    CreateMyNotes(dataMgr->GetNumberOfNotes()); // Refresh the list of "My Notes"
}

/** Called when the 'Date Note' item from the systray context menu is selected */
void CStickyNotesUIMgr::AddDateNoteTriggered()
{
    if(!iHideAllNotes->isEnabled())
        iHideAllNotes->setEnabled(true);

    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();
    dataMgr->AddNote(STICKY_EXPIRY_ON_DATE);

    CreateMyNotes(dataMgr->GetNumberOfNotes()); // Refresh the list of "My Notes"
}

/** Called when the 'Generic' item from the systray context menu is selected */
void CStickyNotesUIMgr::AddGenericTriggered()
{
    if(!iHideAllNotes->isEnabled())
        iHideAllNotes->setEnabled(true);

    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();
    dataMgr->AddNote(STICKY_GENERIC_NOTE);

    CreateMyNotes(dataMgr->GetNumberOfNotes()); // Refresh the list of "My Notes"
}

/** Called when the 'Recurring DOW' item from the systray context menu is selected */
void CStickyNotesUIMgr::AddRecurringDOWTriggered()
{
    if(!iHideAllNotes->isEnabled())
        iHideAllNotes->setEnabled(true);

    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();
    dataMgr->AddNote(STICKY_RECURRING_DOW);

    CreateMyNotes(dataMgr->GetNumberOfNotes()); // Refresh the list of "My Notes"
}

/** Called when the 'Donate' item from the systray context menu is selected */
void CStickyNotesUIMgr::DonateTriggered()
{
    QMessageBox::information(this, tr("Sticky Notes Plus :: Donate"),
                             tr("Thank You for your gesture. We'll be accepting donations soon!"));
}

/** Called when the 'Preferences' item from the systray context menu is selected */
void CStickyNotesUIMgr::PreferencesTriggered()
{
    QMessageBox::information(this, tr("Sticky Notes Plus :: Preferences"),
                             tr("Under Construction!"));
}

/** Called when the 'About' item from the systray context menu is selected */
void CStickyNotesUIMgr::AboutTriggered()
{
    QMessageBox::information(this, tr("Sticky Notes Plus :: About"),
                             tr("Sticky Notes Plus version 0.5.1\nDeveloper: Sai Srikantham (ssn.kishore@gmail.com)\nAll rights reserved"));
}

/** Called when the 'Today's Reminders' item from the systray context menu is selected */
void CStickyNotesUIMgr::TodaysRemindersTriggered()
{
    QDate todaysDate = QDate::currentDate();

    if(!ShowReminders(todaysDate.dayOfWeek()))
    {
        QMessageBox::information(this, tr("Sticky Notes Plus :: Reminders"),
                                 tr("You have no reminders today"));
    }
}

/** Called when the 'Tomorrow's Reminders' item from the systray context menu is selected */
void CStickyNotesUIMgr::TomorrowsRemindersTriggered()
{
    QDate todaysDate = QDate::currentDate();

    if(!ShowReminders(todaysDate.dayOfWeek() + 1))
    {
        QMessageBox::information(this, tr("Sticky Notes Plus :: Reminders"),
                                 tr("You have no reminders for tomorrow"));
    }
}

/** Called when the 'All Reminders' item from the systray context menu is selected */
void CStickyNotesUIMgr::AllRemindersTriggered()
{
    bool atleastOneReminder;

    for( int dow = MONDAY; dow <= SUNDAY; ++dow)
    {
        atleastOneReminder = ShowReminders(dow);
    }

    if(!atleastOneReminder)
    {
        QMessageBox::information(this, tr("Sticky Notes Plus :: Reminders"),
                                 tr("You have no reminders"));
    }
}

/** Displays the Reminders of any given day of the week */
bool CStickyNotesUIMgr::ShowReminders(int dayOfWeek)
{
    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();

    vector<CStickyNote*>::iterator notesBeginIterator = dataMgr->GetNotesBeginIterator();
    vector<CStickyNote*>::iterator notesEndIterator = dataMgr->GetNotesEndIterator();

    bool atleastOneReminder = false;

    if(notesBeginIterator != notesEndIterator)
    {
        while(notesBeginIterator != notesEndIterator)
        {
            if((*notesBeginIterator)->GetNoteType() == STICKY_RECURRING_DOW)
            {
                switch(dayOfWeek)
                {
                case MONDAY:
                    if((*notesBeginIterator)->IsReminderMonday())
                    {
                        (*notesBeginIterator)->show();
                        atleastOneReminder = true;
                    }
                    break;
                case TUESDAY:
                    if((*notesBeginIterator)->IsReminderTuesday())
                    {
                        (*notesBeginIterator)->show();
                        atleastOneReminder = true;
                    }
                    break;
                case WEDNESDAY:
                    if((*notesBeginIterator)->IsReminderWednesday())
                    {
                        (*notesBeginIterator)->show();
                        atleastOneReminder = true;
                    }
                    break;
                case THURSDAY:
                    if((*notesBeginIterator)->IsReminderThursday())
                    {
                        (*notesBeginIterator)->show();
                        atleastOneReminder = true;
                    }
                    break;
                case FRIDAY:
                    if((*notesBeginIterator)->IsReminderFriday())
                    {
                        (*notesBeginIterator)->show();
                        atleastOneReminder = true;
                    }
                    break;
                case SATURDAY:
                    if((*notesBeginIterator)->IsReminderSaturday())
                    {
                        (*notesBeginIterator)->show();
                        atleastOneReminder = true;
                    }
                    break;
                case SUNDAY:
                    if((*notesBeginIterator)->IsReminderSunday())
                    {
                        (*notesBeginIterator)->show();
                        atleastOneReminder = true;
                    }
                    break;
                }
            }

            ++notesBeginIterator;
        }
    }

    return atleastOneReminder;
}

/** Called when the 'Quit' item from the systray context menu is selected */
void CStickyNotesUIMgr::SaveAndQuit()
{
    CStickyNotesPlus::GetInstance()->GetDataMgr()->SaveAllNotes();
    CStickyNotesPlus::GetInstance()->GetToDoList()->SaveAllTasks();
    exit(EXIT_SUCCESS);
}

/** Evaluates if the 'Hide All Notes' option of the systray should be active */
void CStickyNotesUIMgr::EvaluateShowNHideAll()
{
    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();

    vector<CStickyNote*>::iterator notesBeginIterator = dataMgr->GetNotesBeginIterator();
    vector<CStickyNote*>::iterator notesEndIterator = dataMgr->GetNotesEndIterator();

    bool atLeastOneHidden = false;
    bool atLeastOneVisible = false;

    if(notesBeginIterator != notesEndIterator)
    {
        while(notesBeginIterator != notesEndIterator)
        {
            if((*notesBeginIterator)->isVisible())
                atLeastOneVisible = true;
            else
                atLeastOneHidden = true;

            ++notesBeginIterator;
        }

        /** At least one not is still visible to the user, so 'Hide All' should be enabled */
        if(atLeastOneVisible)
        {
            iHideAllNotes->setEnabled(true);
            if(atLeastOneHidden)
                iShowAllNotes->setEnabled(true);
            else
                iShowAllNotes->setEnabled(false);
        }
        else
        {
            iHideAllNotes->setEnabled(false);
            if(atLeastOneHidden)
                iShowAllNotes->setEnabled(true);
            else
                iShowAllNotes->setEnabled(false);
        }
    }
    else
    {
        iShowAllNotes->setEnabled(true);
        iHideAllNotes->setEnabled(false);
    }
}

/** Called when the text in the "Search" text box is changed */
void CStickyNotesUIMgr::HandleSearchTextChange(QString currentText)
{
    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();

    vector<CStickyNote*>::iterator notesBeginIterator = dataMgr->GetNotesBeginIterator();
    vector<CStickyNote*>::iterator notesEndIterator = dataMgr->GetNotesEndIterator();

    if(notesBeginIterator != notesEndIterator)
    {
        while(notesBeginIterator != notesEndIterator)
        {
            QString noteTitle = (*notesBeginIterator)->GetTitle();

            if(!noteTitle.contains(currentText, Qt::CaseInsensitive))
            {
                (*notesBeginIterator)->GetAction()->setVisible(false);
            }
            else
            {
                (*notesBeginIterator)->GetAction()->setVisible(true);
            }
            ++notesBeginIterator;
        }
    }
}

/** Called when the "Search" text box receives focus */
void CStickyNotesUIMgr::HandleSearchTextFocus(int unused1, int unused2)
{
    if(iSearchLineEdit->text().contains(tr("Search..."), Qt::CaseInsensitive))
        iSearchLineEdit->setText(tr(""));
}

/** Show the pop-up to start "time tracking" */
void CStickyNotesUIMgr::ShowTimeTrackingPopup()
{
    QMessageBox::information(this, tr("Sticky Notes Plus :: Time Tracking"),
                             tr("This is a reminder to start your time tracking for the day.\n Have a good one!"));
}

/** Called when the user hovers over the "All Notes" menu */
void CStickyNotesUIMgr::AllNotesHovered()
{
    iCurrentlyHoveredMenu = ALL_NOTES_MENU;
}

/** Called when the user hovers over the "Date Notes" menu */
void CStickyNotesUIMgr::DateNotesHovered()
{
    iCurrentlyHoveredMenu = DATE_MENU;
}

/** Called when the user hovers over the "Hour Notes" menu */
void CStickyNotesUIMgr::HourNotesHovered()
{
    iCurrentlyHoveredMenu = HOUR_MENU;
}

/** Called when the user hovers over the "Generic Notes" menu */
void CStickyNotesUIMgr::GenericNotesHovered()
{
    iCurrentlyHoveredMenu = GENERIC_NOTES_MENU;
}

/** Called when the user hovers over the "Reminder Notes" menu */
void CStickyNotesUIMgr::ReminderNotesHovered()
{
    iCurrentlyHoveredMenu = REMINDERS_MENU;
}

/** Returns the member variable iCurrentlyHoveredMenu */
MyNotesMenus CStickyNotesUIMgr::GetCurrentlyHoveredMenu()
{
    return iCurrentlyHoveredMenu;
}

/** Shows the "All Notes" menu */
void CStickyNotesUIMgr::ShowCurrentlyHoveredNotesMenu()
{
    switch(iCurrentlyHoveredMenu)
    {
    case ALL_NOTES_MENU:
        iMyNotesAllNotesMenu->show();
        break;
    case GENERIC_NOTES_MENU:
        iMyNotesGenericNotesMenu->show();
        break;
    case REMINDERS_MENU:
        iMyNotesReminderNotesMenu->show();
        break;
    case HOUR_MENU:
        iMyNotesHourNotesMenu->show();
        break;
    case DATE_MENU:
        iMyNotesDateNotesMenu->show();
        break;
    default:
        break;
    }
    iCurrentlyHoveredMenu = FAKE_MENU;
}

/** CStickySystemTrayIcon definition */

/** Constructor */
CStickySystemTrayIcon::CStickySystemTrayIcon(QWidget* aParent):QSystemTrayIcon(aParent)
{
}

/** This function catches all the events sent to this widget */
bool CStickySystemTrayIcon::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) /** When the tool tip is to be displayed */
    {
       CStickyToDoList* stickyTodoList = CStickyNotesPlus::GetInstance()->GetToDoList();
       stickyTodoList->show();
       stickyTodoList->StartTimer();
    }
    return true;
}
