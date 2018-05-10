/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyNote.h */
/** Purpose : This file contains the definition for the class CStickyNote which is basic type of note the user
              can create. This class also is the base for other types of notes. */

#ifndef STICKYNOTE_H
#define STICKYNOTE_H

#include <QWidget>
#include <QIcon>
#include <QDate>
#include "StickyNotesEnums.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QTextDocument;
class QCloseEvent;
class QPushButton;
class QSpinBox;
class QLineEdit;
class QCheckBox;
class QCalendarWidget;
QT_END_NAMESPACE

class CStickyAction;

class CStickyNote : public QWidget
{
    Q_OBJECT
public:
    /** Constructor of the class */
    CStickyNote(int aNoteId, bool aBeingReloaded, NoteType aNoteType);

    /** Destructor of the class */
    ~CStickyNote();

    /** Returns the title of the note */
    QString GetTitle();

    /** Sets the note title */
    void SetTitle(QString aTitle);

    /** Returns the message of the note */
    QString GetMsg();

    /** Sets the note message */
    void SetMsg(QString aMsg);

    /** Returns the type of the note */
    NoteType GetNoteType();

    /** Sets the note type */
    void SetNoteType(NoteType aNoteType);

    /** Returns the note id */
    int GetNoteId();

    /** Sets the note id */
    void SetNoteId(int aNoteId);

    /** Gets the number of hours in which the note expires */
    int GetExpiryInHours();

    /** Gets the number of minutes in which the note expires */
    int GetExpiryInMinutes();

    /** Returns the associated StickAction object */
    CStickyAction* GetAction();

    /** Sets the number of hours in which the note expires */
    void SetExpiryInHours(int aHours);

    /** Sets the number of minutes in which the note expires */
    void SetExpiryInMinutes(int aMinutes);

    /** Assigns the associated StickyAction object */
    void SetAction(CStickyAction* stickyAction);

    /** Returns TRUE if the note has expired */
    bool IsExpired();

    /** Sets the iExpired bool variable */
    void SetExpired(bool aExpired);

    /** Returns true if the 'reminder' check box is checked */
    bool IsReminder();

    /** Sets the 'checked' state of the 'reminder' checkbox */
    void SetReminder(bool aChecked);

    /** Returns true if the 'Monday' check box is checked */
    bool IsReminderMonday();

    /** Returns true if the 'Tuesday' check box is checked */
    bool IsReminderTuesday();

    /** Returns true if the 'Wednesday' check box is checked */
    bool IsReminderWednesday();

    /** Returns true if the 'Thursday' check box is checked */
    bool IsReminderThursday();

    /** Returns true if the 'Friday' check box is checked */
    bool IsReminderFriday();

    /** Returns true if the 'Saturday' check box is checked */
    bool IsReminderSaturday();

    /** Returns true if the 'Sunday' check box is checked */
    bool IsReminderSunday();

    /** Sets the 'checked' state of the 'Monday' check box */
    void SetReminderMonday(bool aChecked);

    /** Sets the 'checked' state of the 'Tuesday' check box */
    void SetReminderTuesday(bool aChecked);

    /** Sets the 'checked' state of the 'Wednesday' check box */
    void SetReminderWednesday(bool aChecked);

    /** Sets the 'checked' state of the 'Thursday' check box */
    void SetReminderThursday(bool aChecked);

    /** Sets the 'checked' state of the 'Friday' check box */
    void SetReminderFriday(bool aChecked);

    /** Sets the 'checked' state of the 'Saturday' check box */
    void SetReminderSaturday(bool aChecked);

    /** Sets the 'checked' state of the 'Sunday' check box */
    void SetReminderSunday(bool aChecked);

    /** Returns the "date note" day of expiry */
    int GetExpiryDay();

    /** Returns the "date note" month of expiry */
    int GetExpiryMonth();

    /** Returns the "date note" year of expiry */
    int GetExpiryYear();

    /** Sets the "date note" date of expiry */
    void SetExpiryDate(int aDay, int aMonth, int aYear);

protected:
    /** Overriding the base class function to "ignore" the close event */
    void closeEvent(QCloseEvent *event);

private slots:
    /** Function called on the "text modified" signal to check that the user doesn't surpass 140 characters */
    void CheckNoteWidth();

    /** Function called when the 'Delete Note' button is clicked */
    void DeleteClicked();

    /** Function called when the 'Hide Note' button is clicked */
    void HideClicked();

    /** Function called on the "text modified" signal of the title text box */
    void ChangeTitleText();

    /** Function called when the 'Set Tile' button is clicked */
    void SetTitleClicked();

    /** Function called when the 'Hide All But This' button is clicked */
    void HideAllButThisClicked();

    /** Function called when the 'Change Expiry Date' button is clicked */
    void ChangeExpiryDateClicked();

private:
    /** Data Components */
    QString         iTitle;
    QString         iMsg;
    NoteType        iNoteType;
    int             iNoteId;
    bool            iExpired;

    /** Generic Note UI Components */
    QTextEdit*      iTextEdit;

    QLabel*         iCharsRem;

    QVBoxLayout*    iMainLayout;
    QHBoxLayout*    iButtonsLayout;
    QHBoxLayout*    iTitleWidgetsLayout;
    QHBoxLayout*    iHideAllButThisLayout;
    QHBoxLayout*    iRemindCheckLayout;

    QPushButton*    iDeleteButton;
    QPushButton*    iHideButton;
    QPushButton*    iSetTitleButton;
    QPushButton*    iHideAllButThis;

    QLineEdit*      iTitleTextBox;
    QCheckBox*      iRemindCheckBox;

    /** Recurring Note DOW Components */
    QCheckBox*      iMonCheckBox;
    QCheckBox*      iTueCheckBox;
    QCheckBox*      iWedCheckBox;
    QCheckBox*      iThuCheckBox;
    QCheckBox*      iFriCheckBox;
    QCheckBox*      iSatCheckBox;
    QCheckBox*      iSunCheckBox;

    QHBoxLayout*    iDOWLayout;
    QVBoxLayout*    iDOWLayoutLeft;
    QVBoxLayout*    iDOWLayoutCentre;
    QVBoxLayout*    iDOWLayoutRight;


    /** Hour Note UI Components */
    QLabel*         iHoursLabel;
    QLabel*         iMinutesLabel;
    QSpinBox*       iHoursSpinBox;
    QSpinBox*       iMinutesSpinBox;
    QHBoxLayout*    iSpinBoxLayout;

    /** Date Note UI Components */
    QLabel*          iDateExpiryLabel;
    QVBoxLayout*     iCalendarLayout;
    QCalendarWidget* iCalendarWidget;
    QPushButton*     iShowCalendarButton;
    QDate            iNoteExpiryDate;

    static const int iNoteLength;
    static const int iTitleLength;

    CStickyAction* iAction;
};

#endif // STICKYNOTE_H
