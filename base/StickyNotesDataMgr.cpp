/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNotesDataMgr.cpp */
/** Purpose : This file contains the definition for the class CStickyNotesDataMgr which is responsible to deal
              with saving the "sticky notes" created by the user on to the disk */

#include "StickyNotesDataMgr.h"

#include <fstream>
#include <QTimer>

using std::fstream;
using std::ios;
using std::string;

#define ONE_MINUTE 60000

/** Constructor of the class */
CStickyNotesDataMgr::CStickyNotesDataMgr()
{
    iNotesTimer = new QTimer(this);
    connect(iNotesTimer, SIGNAL(timeout()), this, SLOT(UpdateNotes()));
}

/** Destructor of the class */
CStickyNotesDataMgr::~CStickyNotesDataMgr()
{
    iNotesTimer->stop();
    if(iNotesTimer)
    {
        delete iNotesTimer;
        iNotesTimer = NULL;
    }
}

/** Creates a new note of the specified type */
void CStickyNotesDataMgr::AddNote(NoteType aNoteType, bool aBeingReloaded)
{
    switch(aNoteType)
    {
    case STICKY_EXPIRY_IN_HOURS:
    case STICKY_GENERIC_NOTE:
    case STICKY_RECURRING_DOW:
    case STICKY_EXPIRY_ON_DATE:
        {
            int vectorSize = iStickyNotes.size();
            iStickyNotes.push_back(new CStickyNote(vectorSize + 1, aBeingReloaded, aNoteType));
            break;
        }
    default:
        break;
    }
}

/** Saves all notes to a file */
void CStickyNotesDataMgr::SaveAllNotes()
{
    fstream binaryFile("StickyNotesPlus.dat",ios::out|ios::binary);

    for(unsigned int index=0; index < iStickyNotes.size(); ++index)
    {
        StickyNoteStruct noteStruct;
        CStickyNote* stickyNote = iStickyNotes.at(index);

        noteStruct.noteId = stickyNote->GetNoteId();
        string temp = stickyNote->GetTitle().toStdString();
        temp.copy(noteStruct.title, temp.length(), 0);
        noteStruct.title[temp.length()] = '\0';

        temp = "";
        temp = stickyNote->GetMsg().toStdString();

        temp.copy(noteStruct.message, temp.length(), 0);
        noteStruct.message[temp.length()] = '\0';

        noteStruct.type = stickyNote->GetNoteType();

        noteStruct.reminder = stickyNote->IsReminder();

        switch(stickyNote->GetNoteType())
        {
        case STICKY_EXPIRY_IN_HOURS:
            {
                noteStruct.expiryInHours   = stickyNote->GetExpiryInHours();
                noteStruct.expiryInMinutes = stickyNote->GetExpiryInMinutes();
                break;
            }
        case STICKY_RECURRING_DOW:
            {
                noteStruct.reminder_mon = stickyNote->IsReminderMonday();
                noteStruct.reminder_tue = stickyNote->IsReminderTuesday();
                noteStruct.reminder_wed = stickyNote->IsReminderWednesday();
                noteStruct.reminder_thu = stickyNote->IsReminderThursday();
                noteStruct.reminder_fri = stickyNote->IsReminderFriday();
                noteStruct.reminder_sat = stickyNote->IsReminderSaturday();
                noteStruct.reminder_sun = stickyNote->IsReminderSunday();
            }
        case STICKY_EXPIRY_ON_DATE:
            {
                noteStruct.day   = stickyNote->GetExpiryDay();
                noteStruct.month = stickyNote->GetExpiryMonth();
                noteStruct.year  = stickyNote->GetExpiryYear();
                break;
            }
        default:
            break;
        }

        binaryFile.write(reinterpret_cast<char*>(&noteStruct),sizeof(StickyNoteStruct));
    }

    binaryFile.close();
}

/** Saves the specified note to the file */
void CStickyNotesDataMgr::SaveNote(int aNoteId)
{
    int temp = aNoteId;
    temp++;
    // TBD
}

/** Deletes the specified note */
void CStickyNotesDataMgr::DeleteNote(int aNoteId)
{
    int position = aNoteId - 1;

    CStickyNote* stickyNote = iStickyNotes.at(position);

    if(stickyNote)
    {
        delete stickyNote;
        stickyNote = NULL;
    }

    iStickyNotes.erase(iStickyNotes.begin() + position);

    for(unsigned int index=0; index<iStickyNotes.size(); ++index)
    {
        stickyNote = iStickyNotes.at(index);
        stickyNote->SetNoteId(index+1);
    }
}

/** Deletes all the notes */
void CStickyNotesDataMgr::DeleteAllNotes()
{
    iStickyNotes.clear();
    SaveAllNotes(); // Clearing the file out
}

/** Reloads the saved notes */
void CStickyNotesDataMgr::ReloadNotes()
{
    fstream binaryFile("StickyNotesPlus.dat",ios::in|ios::binary);

    if(!binaryFile.fail())
    {
        do
        {
            StickyNoteStruct noteStruct;
            binaryFile.read(reinterpret_cast<char*>(&noteStruct),sizeof(StickyNoteStruct));
            if(binaryFile.eof())
                break;

            AddNote(noteStruct.type, true);
            CStickyNote* stickyNote = iStickyNotes.at(iStickyNotes.size() - 1);

            QString tempTitle = noteStruct.title;
            stickyNote->SetTitle(tempTitle);

            QString tempMsg = noteStruct.message;
            stickyNote->SetMsg(tempMsg);

            stickyNote->SetNoteType(noteStruct.type);
            stickyNote->SetReminder(noteStruct.reminder);

            switch(noteStruct.type)
            {
            case STICKY_EXPIRY_IN_HOURS:
                {
                    stickyNote->SetExpiryInHours(noteStruct.expiryInHours);
                    stickyNote->SetExpiryInMinutes(noteStruct.expiryInMinutes);
                }
                break;
            case STICKY_RECURRING_DOW:
                {
                    stickyNote->SetReminderMonday(noteStruct.reminder_mon);
                    stickyNote->SetReminderTuesday(noteStruct.reminder_tue);
                    stickyNote->SetReminderWednesday(noteStruct.reminder_wed);
                    stickyNote->SetReminderThursday(noteStruct.reminder_thu);
                    stickyNote->SetReminderFriday(noteStruct.reminder_fri);
                    stickyNote->SetReminderSaturday(noteStruct.reminder_sat);
                    stickyNote->SetReminderSunday(noteStruct.reminder_sun);
                }
                break;
            case STICKY_EXPIRY_ON_DATE:
                {
                    stickyNote->SetExpiryDate(noteStruct.day, noteStruct.month, noteStruct.year);
                }
                break;
            default:
                break;
            }

            if(noteStruct.type != STICKY_RECURRING_DOW && noteStruct.reminder)
                stickyNote->show();

        }while(!binaryFile.eof());
    }
    binaryFile.close();
}

/** Returns an iterator for the Sticky Notes Vector */
vector<CStickyNote*>::iterator CStickyNotesDataMgr::GetNotesBeginIterator()
{
    return iStickyNotes.begin();
}

/** Returns an iterator for the Sticky Notes Vector end */
vector<CStickyNote*>::iterator CStickyNotesDataMgr::GetNotesEndIterator()
{
    return iStickyNotes.end();
}

/** Returns number of notes currently held */
int CStickyNotesDataMgr::GetNumberOfNotes()
{
    return iStickyNotes.size();
}

/** Starts the QT One Minute Timer */
void CStickyNotesDataMgr::StartNotesTimer()
{
    iNotesTimer->start(ONE_MINUTE);
}

/** Updates the notes' information after every minute */
void CStickyNotesDataMgr::UpdateNotes()
{
    for(unsigned int index=0; index < iStickyNotes.size(); ++index)
    {
        CStickyNote* stickyNote = iStickyNotes.at(index);
        if(!stickyNote->IsExpired()) /** Processing only those notes which HAVEN'T yet expired */
        {
            switch(stickyNote->GetNoteType())
            {
            case STICKY_EXPIRY_IN_HOURS:
                {

                    int expiryInHours = stickyNote->GetExpiryInHours();
                    int expiryInMinutes = stickyNote->GetExpiryInMinutes();

                    if(expiryInMinutes > 0)
                        --expiryInMinutes;

                    if(expiryInMinutes == 0 && expiryInHours == 0) /** Gradually the minutes and hours have become ZERO */
                    {
                        stickyNote->SetExpired(true);
                        stickyNote->SetExpiryInMinutes(0);
                        stickyNote->show();
                        stickyNote->setFocus(Qt::ActiveWindowFocusReason);
                    }
                    else if(expiryInMinutes == 0 && expiryInHours > 0) /** 60 minutes have passed, so reduce ONE hour */
                    {
                        --expiryInHours;
                        expiryInMinutes = 60;
                    }

                    if(expiryInHours >= 0)
                        stickyNote->SetExpiryInHours(expiryInHours);

                    if(expiryInMinutes > 0)
                        stickyNote->SetExpiryInMinutes(expiryInMinutes);

                    break;
                }
            case STICKY_EXPIRY_ON_DATE:
                {
                    break;
                }
            default:
                break;
            }
        }
    }
}
