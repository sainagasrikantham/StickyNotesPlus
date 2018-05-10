/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNote.cpp */
/** Purpose : This file contains the definition for the class CStickyNote which is basic type of note the user
              can create. This class also is the base for other types of notes. */

#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <string>
#include <QString>
#include <QLabel>
#include <QCloseEvent>
#include <QPushButton>
#include <QPalette>
#include <QSpinBox>
#include <QCheckBox>
#include <QDate>
#include <QCalendarWidget>

#include "StickyNote.h"
#include "StickyNotesPlus.h"
#include "StickyNotesDataMgr.h"
#include "StickyNotesUIMgr.h"

using std::string;

const int CStickyNote::iNoteLength = 140;
const int CStickyNote::iTitleLength = 25;

/** Constructor of the class */
CStickyNote::CStickyNote(int aNoteId, bool aBeingReloaded, NoteType aNoteType)
{
    QString temp;
    temp.setNum(aNoteId);

    iExpired = false;
    iTitle   = "Untitled Note ";
    iTitle.append(temp);
    setWindowTitle(iTitle);

    iNoteId   = aNoteId;
    iNoteType = aNoteType;

    iTextEdit     = new QTextEdit();
    iTitleTextBox = new QLineEdit();
    iTitleTextBox->setMaxLength(iTitleLength);

    iHideButton     = new QPushButton(tr("Hide Note"));
    iDeleteButton   = new QPushButton(tr("Delete Note"));
    iSetTitleButton = new QPushButton(tr("Set Title"));
    iHideAllButThis = new QPushButton(tr("Hide All But This"));

    iCharsRem = new QLabel(tr("140 Character(s) Remaining"));

    iRemindCheckBox = new QCheckBox();
    iRemindCheckBox->setText(tr("Make this a reminder in every Sticky Notes session"));
    iRemindCheckBox->setToolTip(tr("Checking the box will make this note appear on every startup"));

    iMainLayout           = new QVBoxLayout();
    iButtonsLayout        = new QHBoxLayout();
    iTitleWidgetsLayout   = new QHBoxLayout();
    iHideAllButThisLayout = new QHBoxLayout();
    iRemindCheckLayout    = new QHBoxLayout();

    QPalette notePalette = iTextEdit->palette();
    notePalette.setColor(QPalette::Base, Qt::yellow);
    iTextEdit->setPalette(notePalette);

    QPalette windowPalette = this->palette();
    windowPalette.setColor(QPalette::Window, Qt::yellow);
    this->setPalette(windowPalette);

    iTitleTextBox->setVisible(false);
    iTitleWidgetsLayout->addWidget(iTitleTextBox);
    iTitleWidgetsLayout->addWidget(iSetTitleButton);
    iMainLayout->addLayout(iTitleWidgetsLayout);

    iRemindCheckLayout->addWidget(iRemindCheckBox);

    if(aNoteType != STICKY_RECURRING_DOW)
        iMainLayout->addLayout(iRemindCheckLayout);

    switch(aNoteType)
    {
    case STICKY_EXPIRY_IN_HOURS:
        {
            iHoursSpinBox   = new QSpinBox();
            iMinutesSpinBox = new QSpinBox();

            iHoursSpinBox->setRange(0, 23);
            iMinutesSpinBox->setRange(0, 60);

            iHoursLabel   = new QLabel(tr("Hours : "));
            iMinutesLabel = new QLabel(tr("Minutes : "));

            iSpinBoxLayout = new QHBoxLayout();
            iSpinBoxLayout->addWidget(iHoursLabel);
            iSpinBoxLayout->addWidget(iHoursSpinBox);
            iSpinBoxLayout->addWidget(iMinutesLabel);
            iSpinBoxLayout->addWidget(iMinutesSpinBox);

            iMainLayout->addLayout(iSpinBoxLayout);
        }
        break;
    case STICKY_RECURRING_DOW:
        {
            iMonCheckBox = new QCheckBox();
            iTueCheckBox = new QCheckBox();
            iWedCheckBox = new QCheckBox();
            iThuCheckBox = new QCheckBox();
            iFriCheckBox = new QCheckBox();
            iSatCheckBox = new QCheckBox();
            iSunCheckBox = new QCheckBox();

            iMonCheckBox->setText(tr("Monday"));
            iMonCheckBox->setToolTip(tr("Remind me every Monday"));

            iTueCheckBox->setText(tr("Tuesday"));
            iTueCheckBox->setToolTip(tr("Remind me every Tuesday"));

            iWedCheckBox->setText(tr("Wednesday"));
            iWedCheckBox->setToolTip(tr("Remind me every Wednesday"));

            iThuCheckBox->setText(tr("Thursday"));
            iThuCheckBox->setToolTip(tr("Remind me every Thursday"));

            iFriCheckBox->setText(tr("Friday"));
            iFriCheckBox->setToolTip(tr("Remind me every Friday"));

            iSatCheckBox->setText(tr("Saturday"));
            iSatCheckBox->setToolTip(tr("Remind me every Saturday"));

            iSunCheckBox->setText(tr("Sunday"));
            iSunCheckBox->setToolTip(tr("Remind me every Sunday"));

            iDOWLayout       = new QHBoxLayout();
            iDOWLayoutLeft   = new QVBoxLayout();
            iDOWLayoutRight  = new QVBoxLayout();
            iDOWLayoutCentre = new QVBoxLayout();

            iDOWLayoutLeft->addWidget(iMonCheckBox);
            iDOWLayoutLeft->addWidget(iTueCheckBox);
            iDOWLayoutLeft->addWidget(iWedCheckBox);

            iDOWLayoutCentre->addWidget(iThuCheckBox);
            iDOWLayoutCentre->addWidget(iFriCheckBox);
            iDOWLayoutCentre->addWidget(iSatCheckBox);

            iDOWLayoutRight->addWidget(iSunCheckBox);

            iDOWLayout->addLayout(iDOWLayoutLeft);
            iDOWLayout->addLayout(iDOWLayoutCentre);
            iDOWLayout->addLayout(iDOWLayoutRight);

            iMainLayout->addLayout(iDOWLayout);
        }
        break;
    case STICKY_EXPIRY_ON_DATE:
        {
            iDateExpiryLabel    = new QLabel(tr("No Expiry Date Selected Yet"));
            iShowCalendarButton = new QPushButton(tr("Change Expiry Date"));
            iCalendarWidget     = new QCalendarWidget(this);
            iCalendarLayout     = new QVBoxLayout();

            iCalendarWidget->setVisible(false);
            iCalendarLayout->addWidget(iDateExpiryLabel);
            iCalendarLayout->addWidget(iCalendarWidget);
            iCalendarLayout->addWidget(iShowCalendarButton);

            iMainLayout->addLayout(iCalendarLayout);
            connect( iShowCalendarButton, SIGNAL(clicked()),           this, SLOT(ChangeExpiryDateClicked()) );
        }
        break;
    default:
        break;
    }

    // Trying to apply colour to the buttons. Doesn't work!

    /*QPalette hideButtonPalette = iHideButton->palette();
    hideButtonPalette.setColor(QPalette::Base, Qt::yellow);
    iHideButton->setPalette(hideButtonPalette);
    iHideButton->setAutoFillBackground(true);

    QPalette deleteButtonPalette = iDeleteButton->palette();
    deleteButtonPalette.setColor(QPalette::Base, Qt::yellow);
    iDeleteButton->setPalette(notePalette);
    iDeleteButton->setAutoFillBackground(true);*/

    iMainLayout->addWidget(iTextEdit);
    iMainLayout->addWidget(iCharsRem);

    iButtonsLayout->addWidget(iHideButton);
    iButtonsLayout->addWidget(iDeleteButton);

    iMainLayout->addLayout(iButtonsLayout);

    iHideAllButThisLayout->addWidget(iHideAllButThis);
    iMainLayout->addLayout(iHideAllButThisLayout);

    setLayout(iMainLayout);

    setWindowIcon(CStickyNotesPlus::GetStickyNotesIcon());

    /** Making connections */
    connect( iTextEdit,           SIGNAL(textChanged()),       this, SLOT(CheckNoteWidth())        );
    connect( iHideButton,         SIGNAL(clicked()),           this, SLOT(HideClicked())           );
    connect( iDeleteButton,       SIGNAL(clicked()),           this, SLOT(DeleteClicked())         );
    connect( iTitleTextBox,       SIGNAL(textEdited(QString)), this, SLOT(ChangeTitleText())       );
    connect( iSetTitleButton,     SIGNAL(clicked()),           this, SLOT(SetTitleClicked())       );
    connect( iHideAllButThis,     SIGNAL(clicked()),           this, SLOT(HideAllButThisClicked()) );


    this->setFixedSize(sizeHint());

    if(!aBeingReloaded)
        show();
}

/** Destructor of the class */
CStickyNote::~CStickyNote()
{
    if(iTextEdit)
    {
        delete iTextEdit;
        iTextEdit = NULL;
    }

    if(iMainLayout)
    {
        delete iMainLayout;
        iMainLayout = NULL;
    }

    if(iCharsRem)
    {
        delete iCharsRem;
        iCharsRem = NULL;
    }

    if(iHideButton)
    {
        delete iHideButton;
        iHideButton = NULL;
    }

    if(iDeleteButton)
    {
        delete iDeleteButton;
        iDeleteButton = NULL;
    }
}

/** Overriding the base class function to "ignore" the close event */
void CStickyNote::closeEvent(QCloseEvent *event)
{
    QMessageBox::information(this, tr("Sticky Notes Plus"),
                             tr(" Click the 'Hide Note' button \n\tor\n Choose 'Hide All Notes' from systray menu"));
    event->ignore();
}

/** Function called when the 'Delete Note' button is clicked */
void CStickyNote::DeleteClicked()
{
    int keepIt = 0;

    keepIt = QMessageBox::question(this, tr("Sticky Notes Plus"),
                                   tr("This action cannot be reverted. Continue?"),
                                   tr("Yes, go ahead"),
                                   tr("No, hold on!!"), 0, 0, 1);

    if(!keepIt) /** Wishes to delete the note */
    {
        CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();
        dataMgr->DeleteNote(iNoteId);
        dataMgr->SaveAllNotes();

        CStickyNotesUIMgr* uiMgr = CStickyNotesPlus::GetInstance()->GetUIMgr();
        uiMgr->EvaluateShowNHideAll();
        uiMgr->CreateMyNotes(dataMgr->GetNumberOfNotes());
    }
}

/** Function called when the 'Hide Note' button is clicked */
void CStickyNote::HideClicked()
{
    int deleteIt = 0;
    if(iExpired)
    {
        deleteIt = QMessageBox::question(this, tr("Sticky Notes Plus"),
                                         tr("This note has expired. Do you still wish to keep it?"),
                                         tr("Yeah, I want to"),
                                         tr("Nope, thrash it"), 0, 0, 1);

        if(deleteIt) /** Wishes to delete the expired note */
        {
            CStickyNotesPlus::GetInstance()->GetDataMgr()->DeleteNote(iNoteId);
            CStickyNotesPlus::GetInstance()->GetUIMgr()->EvaluateShowNHideAll();
        }
    }
    if(!deleteIt) /** User wishes to keep the expired note OR the normal case of
                    a general NOTE being want to be hidden */
    {
        hide();
        CStickyNotesPlus::GetInstance()->GetUIMgr()->EvaluateShowNHideAll();
    }

    /** Save the notes, just in case anything was modified */
    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();
    dataMgr->SaveAllNotes();
}

/** Function called on the "text modified" signal to check that the user doesn't surpass 140 characters */
void CStickyNote::CheckNoteWidth()
{
    static int allowedLength = iNoteLength;

    QString currentText = GetMsg();
    string stringText = currentText.toStdString();

    int strLen = stringText.length();
    int charsRem = (iNoteLength - strLen); /** -1 because the actual limit is 140 chars only
                                                but we need one extra for the backslash n character */

    if(charsRem >= 0)
    {
        QString tempStr;

        tempStr.setNum(charsRem);
        tempStr.append(" Character(s) Remaining");
        iCharsRem->setText(tempStr);
    }

    if( strLen > allowedLength)
    {
        allowedLength = iNoteLength + 1;
        stringText = stringText.substr(0, iNoteLength-1); /** Trimming back to 140 characters */
        currentText = stringText.c_str();
        currentText.append("\n");
        SetMsg(currentText);
    }
}

/** Function called on the "text modified" signal of the title text box */
void CStickyNote::ChangeTitleText()
{
    QString currentText = iTitleTextBox->text();
    this->setWindowTitle(currentText);
}

/** Function called when the 'Set Tile' button is clicked */
void CStickyNote::SetTitleClicked()
{
    bool titleTextBoxVisible = iTitleTextBox->isVisible();
    
    if(!titleTextBoxVisible)
    {
        iTitleTextBox->setVisible(true);
        iTitleTextBox->clear();
        iSetTitleButton->setText(tr("Done"));
    }
    else
    {
        QString currentText = iTitleTextBox->text();
        string stringText = currentText.toStdString();
        int strLen = stringText.length();
        if(strLen == 0) /** The user did not enter anything (the text box is blank) */
        {
            this->setWindowTitle(iTitle);
        }
        else /** User entered a new title, saving the current title in the title member variable */
        {
            iTitle = currentText;
            // Refresh the list of "My Notes"
            CStickyNotesPlus* stickyNotesPlus = CStickyNotesPlus::GetInstance();
            stickyNotesPlus->GetUIMgr()->CreateMyNotes(stickyNotesPlus->GetDataMgr()->GetNumberOfNotes());
        }
        iTitleTextBox->setVisible(false);
        iSetTitleButton->setText(tr("Set Title"));
    }
}

/** Function called when the 'Change Expiry Date' button is clicked */
void CStickyNote::ChangeExpiryDateClicked()
{
    if(iCalendarWidget->isVisible())
    {
       iCalendarWidget->setVisible(false);
       iShowCalendarButton->setText(tr("Change Expiry Date"));

       // Date has been selected
       iNoteExpiryDate = iCalendarWidget->selectedDate();
       QString dateString(tr("This note is scheduled to expire on "));
       dateString.append(iNoteExpiryDate.toString());
       iDateExpiryLabel->setText(dateString);
    }
    else
    {
        iCalendarWidget->setMinimumDate(QDate::currentDate());
        iCalendarWidget->setVisible(true);
        iShowCalendarButton->setText(tr("I'm Done"));
    }
}

/** Function called when the 'Hide All But This' button is clicked */
void CStickyNote::HideAllButThisClicked()
{
    CStickyNotesDataMgr* dataMgr = CStickyNotesPlus::GetInstance()->GetDataMgr();

    vector<CStickyNote*>::iterator notesBeginIterator = dataMgr->GetNotesBeginIterator();
    vector<CStickyNote*>::iterator notesEndIterator = dataMgr->GetNotesEndIterator();

    while(notesBeginIterator != notesEndIterator)
    {
        if((*notesBeginIterator)->GetNoteId() == iNoteId) /** Skip THIS NOTE */
        {
            ++notesBeginIterator;
            continue;
        }

        (*notesBeginIterator)->hide();
        ++notesBeginIterator;
    }
    CStickyNotesPlus::GetInstance()->GetUIMgr()->EvaluateShowNHideAll();
}

/** Returns the title of the note */
QString CStickyNote::GetTitle()
{
    return iTitle;
}

/** Sets the note title */
void CStickyNote::SetTitle(QString aTitle)
{
    iTitle = aTitle;
    setWindowTitle(iTitle);
}

/** Returns the message of the note */
QString CStickyNote::GetMsg()
{
    iMsg = iTextEdit->toPlainText();
    return iMsg;
}

/** Sets the note message */
void CStickyNote::SetMsg(QString aMsg)
{
    iMsg = aMsg;
    iTextEdit->setText(iMsg);
}

/** Returns the type of the note */
NoteType CStickyNote::GetNoteType()
{
    return iNoteType;
}

/** Sets the note type */
void CStickyNote::SetNoteType(NoteType aNoteType)
{
    iNoteType = aNoteType;
}

/** Returns the note id */
int CStickyNote::GetNoteId()
{
    return iNoteId;
}

/** Sets the note id */
void CStickyNote::SetNoteId(int aNoteId)
{
    iNoteId = aNoteId;
}

/** Gets the number of hours in which the note expires */
int CStickyNote::GetExpiryInHours()
{
    return iHoursSpinBox->value();
}

/** Gets the number of minutes in which the note expires */
int CStickyNote::GetExpiryInMinutes()
{
    return iMinutesSpinBox->value();
}

/** Assigns the associated StickyAction object */
void CStickyNote::SetAction(CStickyAction* stickyAction)
{
    iAction = stickyAction;
}

/** Returns the associated StickAction object */
CStickyAction* CStickyNote::GetAction()
{
    return iAction;
}

/** Sets the number of hours in which the note expires */
void CStickyNote::SetExpiryInHours(int aHours)
{
    iHoursSpinBox->setValue(aHours);
}

/** Sets the number of minutes in which the note expires */
void CStickyNote::SetExpiryInMinutes(int aMinutes)
{
    iMinutesSpinBox->setValue(aMinutes);
}

/** Returns TRUE if the note has expired */
bool CStickyNote::IsExpired()
{
    return iExpired;
}

/** Sets the iExpired bool variable */
void CStickyNote::SetExpired(bool aExpired)
{
    iExpired = aExpired;
}

/** Returns true if the 'reminder' check box is checked */
bool CStickyNote::IsReminder()
{
    return iRemindCheckBox->isChecked();
}

/** Sets the 'checked' state of the 'reminder' checkbox */
void CStickyNote::SetReminder(bool aChecked)
{
    iRemindCheckBox->setChecked(aChecked);
}

/** Returns true if the 'Monday' check box is checked */
bool CStickyNote::IsReminderMonday()
{
    return iMonCheckBox->isChecked();
}

/** Returns true if the 'Tuesday' check box is checked */
bool CStickyNote::IsReminderTuesday()
{
    return iTueCheckBox->isChecked();
}

/** Returns true if the 'Wednesday' check box is checked */
bool CStickyNote::IsReminderWednesday()
{
    return iWedCheckBox->isChecked();
}

/** Returns true if the 'Thursday' check box is checked */
bool CStickyNote::IsReminderThursday()
{
    return iThuCheckBox->isChecked();
}

/** Returns true if the 'Friday' check box is checked */
bool CStickyNote::IsReminderFriday()
{
    return iFriCheckBox->isChecked();
}

/** Returns true if the 'Saturday' check box is checked */
bool CStickyNote::IsReminderSaturday()
{
    return iSatCheckBox->isChecked();
}

/** Returns true if the 'Sunday' check box is checked */
bool CStickyNote::IsReminderSunday()
{
    return iSunCheckBox->isChecked();
}

/** Sets the 'checked' state of the 'Monday' check box */
void CStickyNote::SetReminderMonday(bool aChecked)
{
    iMonCheckBox->setChecked(aChecked);

    if(aChecked)
    {
        QDate todaysDate = QDate::currentDate();
        if(todaysDate.dayOfWeek() == MONDAY)
            show();
    }
}

/** Sets the 'checked' state of the 'Tuesday' check box */
void CStickyNote::SetReminderTuesday(bool aChecked)
{
    iTueCheckBox->setChecked(aChecked);

    if(aChecked)
    {
        QDate todaysDate = QDate::currentDate();
        if(todaysDate.dayOfWeek() == TUESDAY)
            show();
    }
}

/** Sets the 'checked' state of the 'Wednesday' check box */
void CStickyNote::SetReminderWednesday(bool aChecked)
{
    iWedCheckBox->setChecked(aChecked);

    if(aChecked)
    {
        QDate todaysDate = QDate::currentDate();
        if(todaysDate.dayOfWeek() == WEDNESDAY)
            show();
    }
}

/** Sets the 'checked' state of the 'Thursday' check box */
void CStickyNote::SetReminderThursday(bool aChecked)
{
    iThuCheckBox->setChecked(aChecked);

    if(aChecked)
    {
        QDate todaysDate = QDate::currentDate();
        if(todaysDate.dayOfWeek() == THURSDAY)
            show();
    }
}

/** Sets the 'checked' state of the 'Friday' check box */
void CStickyNote::SetReminderFriday(bool aChecked)
{
    iFriCheckBox->setChecked(aChecked);

    if(aChecked)
    {
        QDate todaysDate = QDate::currentDate();
        if(todaysDate.dayOfWeek() == FRIDAY)
            show();
    }
}

/** Sets the 'checked' state of the 'Saturday' check box */
void CStickyNote::SetReminderSaturday(bool aChecked)
{
    iSatCheckBox->setChecked(aChecked);

    if(aChecked)
    {
        QDate todaysDate = QDate::currentDate();
        if(todaysDate.dayOfWeek() == SATURDAY)
            show();
    }
}

/** Sets the 'checked' state of the 'Sunday' check box */
void CStickyNote::SetReminderSunday(bool aChecked)
{
    iSunCheckBox->setChecked(aChecked);

    if(aChecked)
    {
        QDate todaysDate = QDate::currentDate();
        if(todaysDate.dayOfWeek() == SUNDAY)
            show();
    }
}

/** Returns the "date note" day of expiry */
int CStickyNote::GetExpiryDay()
{
    return iNoteExpiryDate.day();
}

/** Returns the "date note" month of expiry */
int CStickyNote::GetExpiryMonth()
{
    return iNoteExpiryDate.month();
}

/** Returns the "date note" year of expiry */
int CStickyNote::GetExpiryYear()
{
    return iNoteExpiryDate.year();
}

/** Sets the "date note" date of expiry */
void CStickyNote::SetExpiryDate(int aDay, int aMonth, int aYear)
{
    iNoteExpiryDate.setDate(aYear, aMonth, aDay);
    iCalendarWidget->setSelectedDate(iNoteExpiryDate);
    QString dateString(tr(""));

    if( iNoteExpiryDate == QDate::currentDate() )
    {
        dateString.append(tr("This note expires today, "));
        iExpired = true;
        show();
    }
    else if( iNoteExpiryDate < QDate::currentDate() )
    {
        dateString.append(tr("This note expired on "));
    }
    else
    {
        dateString.append(tr("This note is scheduled to expire on "));
    }
    dateString.append(iNoteExpiryDate.toString());
    iDateExpiryLabel->setText(dateString);
}
