/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesLoginMgr.cpp */
/** Purpose : This file contains the definition for the class CStickyNotesLoginMgr which is responsible to deal
              with all the "login information" module of the project */

#include "StickyNotesLoginMgr.h"
#include <QTime>
#include <string>
#include <QString>

using std::string;

CStickyNotesLoginMgr* CStickyNotesLoginMgr::self = NULL;

/** Returns the sole instance of the class */
CStickyNotesLoginMgr* CStickyNotesLoginMgr::GetInstance()
{
    if(self == NULL)
    {
        self = new CStickyNotesLoginMgr();
    }
    return self;
}

/** Returns the duration passed since logon */
QString CStickyNotesLoginMgr::GetLoginDuration()
{
    iLoginDuration.clear();

    int milliSeconds = iTimeKeeper.elapsed();

    int minutesElapsed = milliSeconds / 60000;
    int hoursElapsed = minutesElapsed / 60;

    minutesElapsed -= (hoursElapsed * 60);

    QString temp;
    temp.setNum(minutesElapsed);
    iLoginDuration.setNum(hoursElapsed);
    iLoginDuration.append(" hour(s) and ");
    iLoginDuration.append(temp);
    iLoginDuration.append(" minute(s) have passed since you logged in");

    return iLoginDuration;
}

/** Constructor of the class */
CStickyNotesLoginMgr::CStickyNotesLoginMgr()
{
    iTimeKeeper.start();
}

/** Returns the hour when the user logged on */
int CStickyNotesLoginMgr::GetLoginHour()
{
    return iLoginHour;
}

/** Returns the minut when the user logged on */
int CStickyNotesLoginMgr::GetLoginMinute()
{
    return iLoginMinute;
}



/** Saves the loggd on HOUR and MINUTE to a file */
void CStickyNotesLoginMgr::SaveLoginInfo()
{
    // Do Nothing for now!
}
