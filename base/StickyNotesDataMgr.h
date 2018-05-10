/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesDataMgr.h */
/** Purpose : This file contains the definition for the class CStickyNotesDataMgr which is responsible to deal
              with saving the "sticky notes" created by the user on to the disk */

#ifndef STICKYNOTESDATAMGR_H
#define STICKYNOTESDATAMGR_H

#include <vector>
#include <string>

using std::string;

#include "StickyNote.h"
#include <QObject>

typedef struct StickyNoteStruct_t
{
    /** Generic Members */
    char        title[25];
    char        message[140];
    NoteType    type;
    int         noteId;
    bool        reminder;

    /** Date Expiry Members */
    int         day;
    int         month;
    int         year;
    int         hour;
    int         minute;

    /** Hour Expiry Members */
    int         expiryInHours;
    int         expiryInMinutes;

    /** DOW Reminder Members */
    bool        reminder_mon;
    bool        reminder_tue;
    bool        reminder_wed;
    bool        reminder_thu;
    bool        reminder_fri;
    bool        reminder_sat;
    bool        reminder_sun;
}StickyNoteStruct;

using std::vector;

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

class CStickyNotesDataMgr : public QObject
{
    Q_OBJECT
public:
    /** Constructor of the class */
    CStickyNotesDataMgr();

    /** Destructor of the class */
    virtual ~CStickyNotesDataMgr();

    /** Creates a new note of the specified type */
    void AddNote(NoteType aNoteType, bool aBeingReloaded=false);

    /** Saves all notes to a file */
    void SaveAllNotes();

    /** Saves the specified note to the file */
    void SaveNote(int aNoteId);

    /** Deletes the specified note */
    void DeleteNote(int aNoteId);

    /** Deletes all the notes */
    void DeleteAllNotes();

    /** Reloads the saved notes */
    void ReloadNotes();

    /** Returns an iterator for the Sticky Notes Vector begin */
    vector<CStickyNote*>::iterator GetNotesBeginIterator();

    /** Returns an iterator for the Sticky Notes Vector end */
    vector<CStickyNote*>::iterator GetNotesEndIterator();

    /** Returns number of notes currently held */
    int GetNumberOfNotes();

    /** Starts the QT One Minute Timer */
    void StartNotesTimer();

public slots:
    /** Updates the notes' information after every minute */
    void UpdateNotes();

private:
    /** Holds the instances of the sticky notes created by the user */
    vector <CStickyNote*> iStickyNotes;

    QTimer* iNotesTimer;
};

#endif // STICKYNOTESDATAMGR_H
