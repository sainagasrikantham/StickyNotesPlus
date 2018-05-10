/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesPlus.cpp */
/** Purpose : This file contains the definition for the class CStickyNotesPlus which is the main class of the project */

#include "StickyNotesPlus.h"
#include "StickyNotesUIMgr.h"
#include "StickyNotesLoginMgr.h"
#include "StickyNotesDataMgr.h"
#include "StickyToDoList.h"

CStickyNotesPlus* CStickyNotesPlus::self = NULL;
QIcon CStickyNotesPlus::iStickyNotesIcon ( ":/images/stickynote.svg"  );
QIcon CStickyNotesPlus::iPreferencesIcon ( ":/images/preferences.svg" );
QIcon CStickyNotesPlus::iInfoIcon        ( ":/images/info.svg"        );
QIcon CStickyNotesPlus::iDeleteIcon      ( ":/images/delete.svg"      );
QIcon CStickyNotesPlus::iDonateIcon      ( ":/images/dollar.svg"      );
QIcon CStickyNotesPlus::iNewNoteIcon     ( ":/images/plus.svg"        );
QIcon CStickyNotesPlus::iQuitIcon        ( ":/images/power.svg"       );
QIcon CStickyNotesPlus::iShowNotesIcon   ( ":/images/show.svg"        );
QIcon CStickyNotesPlus::iSearchIcon      ( ":/images/search.svg"      );

/** Returns the singleton instance of the class */
CStickyNotesPlus* CStickyNotesPlus::GetInstance()
{
    if(self == NULL)
    {
        self = new CStickyNotesPlus();
    }
    return self;
}

/** Returns the Sticky Notes Icon */
QIcon CStickyNotesPlus::GetStickyNotesIcon()
{
    return iStickyNotesIcon;
}

/** Returns the Quit Icon */
QIcon CStickyNotesPlus::GetQuitIcon()
{
    return iQuitIcon;
}

/** Returns the Preferences Icon */
QIcon CStickyNotesPlus::GetPreferencesIcon()
{
    return iPreferencesIcon;
}

/** Returns the Info Icon */
QIcon CStickyNotesPlus::GetInfoIcon()
{
    return iInfoIcon;
}

/** Returns the Donate Icon */
QIcon CStickyNotesPlus::GetDonateIcon()
{
    return iDonateIcon;
}

/** Returns the New Note Icon */
QIcon CStickyNotesPlus::GetNewNoteIcon()
{
    return iNewNoteIcon;
}

/** Returns the Delete Notes Icon */
QIcon CStickyNotesPlus::GetDeleteIcon()
{
    return iDeleteIcon;
}

/** Returns the Show Notes Icon */
QIcon CStickyNotesPlus::GetShowNotesIcon()
{
    return iShowNotesIcon;
}

/** Returns the Search Icon */
QIcon CStickyNotesPlus::GetSearchIcon()
{
    return iSearchIcon;
}

/** Constructor of the class */
CStickyNotesPlus::CStickyNotesPlus()
{
    iStickyLoginMgr = CStickyNotesLoginMgr::GetInstance();
    iStickyUIMgr    = new CStickyNotesUIMgr();
    iStickyDataMgr  = new CStickyNotesDataMgr();
    iStickyToDoList = new CStickyToDoList();
}

/** Destructor of the class */
CStickyNotesPlus::~CStickyNotesPlus()
{
    StopApp();

    if(iStickyDataMgr)
    {
        delete iStickyDataMgr;
        iStickyDataMgr = NULL;
    }
    if(iStickyLoginMgr)
    {
        delete iStickyLoginMgr;
        iStickyLoginMgr = NULL;
    }
    if(iStickyUIMgr)
    {
        delete iStickyUIMgr;
        iStickyUIMgr = NULL;
    }
    if(iStickyToDoList)
    {
        delete iStickyToDoList;
        iStickyToDoList = NULL;
    }
}

/** Starts the application */
void CStickyNotesPlus::StartApp()
{
    iStickyUIMgr->RegisterActions();
    iStickyUIMgr->hide();
    iStickyDataMgr->ReloadNotes();
    iStickyUIMgr->CreateMyNotes(iStickyDataMgr->GetNumberOfNotes());
    iStickyDataMgr->StartNotesTimer();
    iStickyToDoList->ReloadTasks();
    //iStickyUIMgr->ShowTimeTrackingPopup(); // This isn't required any more. We've implemented 'reminder' notes.
}

/** Stop the application */
void CStickyNotesPlus::StopApp()
{
    iStickyUIMgr->close();
}

/** Returns the data manager instance */
CStickyNotesDataMgr* CStickyNotesPlus::GetDataMgr()
{
    return iStickyDataMgr;
}

/** Returns the login manager instance */
CStickyNotesLoginMgr* CStickyNotesPlus::GetLoginMgr()
{
    return iStickyLoginMgr;
}

/** Returns the UI manager instance */
CStickyNotesUIMgr* CStickyNotesPlus::GetUIMgr()
{
    return iStickyUIMgr;
}

/** Returns the TODO list instance */
CStickyToDoList* CStickyNotesPlus::GetToDoList()
{
    return iStickyToDoList;
}
