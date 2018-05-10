/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesEnums.h */
/** Purpose : This file contains the definition of all the enumerations used in the project */

#ifndef STICKY_NOTES_ENUMS_H
#define STICKY_NOTES_ENUMS_H

enum NoteType
{
    STICKY_EXPIRY_IN_HOURS, /** Reminder in specified number of hours */
    STICKY_EXPIRY_ON_DATE,  /** Reminder on a particular day of the month */
    STICKY_GENERIC_NOTE,    /** Generic note, no bells or whistels attached */
    STICKY_RECURRING_DOW,   /** Recurring note, on a particular day of the week */
};

enum DaysOfTheWeek
{
    MONDAY = 1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

enum MyNotesMenus
{
    ALL_NOTES_MENU,
    GENERIC_NOTES_MENU,
    REMINDERS_MENU,
    HOUR_MENU,
    DATE_MENU,
    FAKE_MENU
};

#endif // STICKY_NOTES_ENUMS_H
