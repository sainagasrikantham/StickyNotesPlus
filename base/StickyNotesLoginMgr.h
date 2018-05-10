/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesLoginMgr.h */
/** Purpose : This file contains the definition for the class CStickyNotesLoginMgr which is responsible to deal
              with all the "login information" module of the project */

#ifndef STICKYNOTESLOGINMGR_H
#define STICKYNOTESLOGINMGR_H

#include <QString>
#include <QTime>

class CStickyNotesLoginMgr
{
public:
    /** Returns the sole instance of the class */
    static CStickyNotesLoginMgr* GetInstance();

    /** Returns the duration passed since logon */
    QString GetLoginDuration();

    /** Returns the hour when the user logged on */
    int GetLoginHour();

    /** Returns the minut when the user logged on */
    int GetLoginMinute();

    /** Saves the loggd on HOUR and MINUTE to a file */
    void SaveLoginInfo();

private:
    /** Constructor of the class */
    CStickyNotesLoginMgr();
private:
    /** Variable which holds the hour the user logged on */
    int iLoginHour;

    /** Variable which holds the minute the user logged on */
    int iLoginMinute;

    /** Variable which holds the duration since user login */
    QString iLoginDuration;

    /** The sole instance of the class */
    static CStickyNotesLoginMgr* self;

    /** The variable which keeps track of time elapsed */
    QTime iTimeKeeper;
};

#endif // STICKYNOTESLOGINMGR_H
