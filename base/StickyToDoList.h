/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyToDoList.h */
/** Purpose : This file contains the definition for the class CStickyToDoList which is the list of
              "to do" tasks */

#ifndef STICKYTODOLIST_H
#define STICKYTODOLIST_H

#include "StickyToDoListItem.h"
#include<QWidget>
#include<QPushButton>
#include<QVBoxLayout>
#include<QScrollArea>
#include<QScrollBar>
#include<QMainWindow>
#include<QCloseEvent>
#include<QTimer>
#include<vector>

typedef struct StickyTodDoTaskStruct_t
{
    /** Generic Members */
    char        taskDetail[140];
    int         taskId;
    bool        isCompleted;
}StickyTodDoTaskStruct;

using std::vector;

class CStickyToDoList : public QMainWindow
{
    Q_OBJECT
public:
    /** Constructor of the class */
    CStickyToDoList();

    /** Destructor of the class */
    ~CStickyToDoList();

    /** This function deletes a particular task */
    void DeleteTodoTask(int aTodoTaskId);

    /** This function reloads saved "to do" tasks from a file */
    void ReloadTasks();

    /** This function saves all "to do" tasks to a file */
    void SaveAllTasks(bool aCalledFromDelete=false);

    /** This function catches all the events sent to this widget */
    bool event(QEvent *event);

    /** This function starts the timer for 3 seconds */
    void StartTimer();

    /** This function is used to disable the "timed" disappearance of the "to do" list. It is called when the
    'middle button' has been used to launch the "to do" list */
    void SetTimerInactive();

protected:
    /** Overriding the "close" event */
    void closeEvent(QCloseEvent* event);

private:
    /** UI Components */
    QPushButton* iAddButton;
    QVBoxLayout* iButtonLayout;
    QVBoxLayout* iMainLayout;
    QScrollArea* iScrollArea;
    QScrollBar*  iScrollBar;

    /** Data Components */
    vector <CStickyToDoListItem*> iToDoList;

    /** Timer Components */
    QTimer* iTimer;
    bool iTimerActive;

private slots:
    /** This function is called when the "Add task" button is pressed */
    void OnAddButtonPressed();

    /** This function is called when the timer expires */
    void OnTimerExpiry();
};

#endif // STICKYTODOLIST_H
