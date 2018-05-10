/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesPlus.h */
/** Purpose : This file contains the definition for the class CStickyNotesPlus which is the main class of the project */

#ifndef STICKYNOTESPLUS_H
#define STICKYNOTESPLUS_H

/** Forward declarations */

class CStickyNotesUIMgr;
class CStickyNotesLoginMgr;
class CStickyNotesDataMgr;
class CStickyToDoList;

#include <QIcon>

/** End of forward declarations*/

class CStickyNotesPlus
{
public:
    /** Destructor of the class */
    ~CStickyNotesPlus();

    /** Starts the application */
    void StartApp();

    /** Stop the application */
    void StopApp();

    /** Returns the data manager instance */
    CStickyNotesDataMgr* GetDataMgr();

    /** Returns the login manager instance */
    CStickyNotesLoginMgr* GetLoginMgr();

    /** Returns the UI manager instance */
    CStickyNotesUIMgr* GetUIMgr();

    /** Returns the TODO list instance */
    CStickyToDoList* GetToDoList();

    /** Returns the singleton instance of the class */
    static CStickyNotesPlus* GetInstance();

    /** Returns the Sticky Notes Icon */
    static QIcon GetStickyNotesIcon();

    /** Returns the Quit Icon */
    static QIcon GetQuitIcon();

    /** Returns the Preferences Icon */
    static QIcon GetPreferencesIcon();

    /** Returns the Info Icon */
    static QIcon GetInfoIcon();

    /** Returns the Donate Icon */
    static QIcon GetDonateIcon();

    /** Returns the New Note Icon */
    static QIcon GetNewNoteIcon();

    /** Returns the Delete Notes Icon */
    static QIcon GetDeleteIcon();

    /** Returns the Show Notes Icon */
    static QIcon GetShowNotesIcon();

    /** Returns the Search Icon */
    static QIcon GetSearchIcon();

private:
    /** Constructor of the class */
    CStickyNotesPlus();

private:
    /** Member instance of the UI manager class */
    CStickyNotesUIMgr* iStickyUIMgr;

    /** Member instance of the login manager class */
    CStickyNotesLoginMgr* iStickyLoginMgr;

    /** Member instance of the data manager class */
    CStickyNotesDataMgr* iStickyDataMgr;

    /** Member instance of the To Do List class */
    CStickyToDoList* iStickyToDoList;

    static CStickyNotesPlus* self;

    static QIcon iStickyNotesIcon;
    static QIcon iPreferencesIcon;
    static QIcon iInfoIcon;
    static QIcon iDeleteIcon;
    static QIcon iDonateIcon;
    static QIcon iNewNoteIcon;
    static QIcon iQuitIcon;
    static QIcon iShowNotesIcon;
    static QIcon iSearchIcon;
};

#endif // STICKYNOTESPLUS_H
