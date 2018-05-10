/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyToDoListItem.h */
/** Purpose : This file contains the definition for the class CStickyToDoListItem which is an instance of
              a "to do" task */

#ifndef STICKYTODOLISTITEM_H
#define STICKYTODOLISTITEM_H

#include<QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;
class QCheckBox;
class QLabel;
QT_END_NAMESPACE

class CStickyToDoListItem : public QWidget
{
    Q_OBJECT
public:
    /** Constructor of the class */
    CStickyToDoListItem(int aTodoTaskId, bool aBeingReloaded);

    /** Destructor of the class */
    ~CStickyToDoListItem();

    /** This function returns if the task is completed */
    bool IsTaskCompleted();

    /** This function sets the task completed checkbox status */
    void SetTaskCompleted(bool aTaskCompleted);

    /** This function returns the task ID */
    int GetTaskId();

    /** This function sets the task ID */
    void SetTaskId(int aTaskId);

    /** This function returns the task detail */
    QString GetTaskDetail();

    /** This function sets the task detail */
    void SetTaskDetail(QString aTaskDetail);

private:
    /** UI components */
    QPushButton* iDeleteButton;
    QPushButton* iEditButton;
    QTextEdit* iTextEdit;
    QCheckBox* iTaskDone;
    QLabel* iTodoText;

    /** Layouts */
    QHBoxLayout* iMainLayout;
    QHBoxLayout* iCheckBoxLayout;
    QHBoxLayout* iLineEditLayout;
    QHBoxLayout* iButtonLayout;

    /** Other variables */
    bool iEditMode;
    int  iTodoTaskId;

    static const int iTodoTextLength;

private slots:
    /** This function is called when the "checkbox" is checked / unchecked */
    void OnCheckBoxStatusChange();

    /** This function is called when the "-" button is pressed */
    void OnDeletePressed();

    /** This function is called when the "edit" button is pressed */
    void OnEditPressed();

    /** Function called on the "text modified" signal to check that the user doesn't surpass 140 characters */
    void CheckTextWidth();
};

#endif // STICKYTODOLISTITEM_H
