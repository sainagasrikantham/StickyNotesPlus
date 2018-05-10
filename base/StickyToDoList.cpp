/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyToDoList.cpp */
/** Purpose : This file contains the definition for the class CStickyToDoList which is the list of
              "to do" tasks */

#include "StickyToDoList.h"
#include "StickyNotesPlus.h"
#include <fstream>
#include <string>
#include <QMessageBox>

using std::fstream;
using std::ios;
using std::string;

#define TODO_LIST_INITIAL_WIDTH    320
#define TODO_LIST_INITIAL_HEIGHT   450
#define TODO_LIST_HEIGHT_INCREMENT 50
#define TODO_ADD_BUTTON_HEIGHT     20
#define TODO_ADD_BUTTON_WIDTH      270
#define TODO_AUTO_HIDE_TIMEOUT     3000

/** Constructor of the class */
CStickyToDoList::CStickyToDoList()
{
    // Creating the "yellow" palette
    QPalette windowPalette = this->palette();
    windowPalette.setColor(QPalette::Window, Qt::yellow);

    // This is a dummy widget
    QWidget* mainWidget = new QWidget(this);

    // The main layout
    iMainLayout = new QVBoxLayout();
    iScrollArea = new QScrollArea(mainWidget);
    iScrollArea->setWidgetResizable(false);
    iScrollArea->setPalette(windowPalette);

    // The "Add a new task" button
    iAddButton = new QPushButton(tr("Add a new task"));
    iAddButton->setFixedSize(TODO_ADD_BUTTON_WIDTH, TODO_ADD_BUTTON_HEIGHT);
    iButtonLayout = new QVBoxLayout(this);
    iButtonLayout->setSizeConstraint(QLayout::SetFixedSize);
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(iAddButton);

    QVBoxLayout* vLayout = new QVBoxLayout(mainWidget); // This a dummy layout
    vLayout->addLayout(hLayout);
    vLayout->addWidget(iScrollArea);
    QWidget* scrollAreaWidgetContents = new QWidget(); // Yet another dummy widget
    scrollAreaWidgetContents->setLayout(iMainLayout);

    iMainLayout->setSizeConstraint(QLayout::SetFixedSize);
    iScrollArea->setWidget(scrollAreaWidgetContents);
    this->setCentralWidget(mainWidget);

    setWindowTitle(QObject::tr("Sticky Todo List"));
    setWindowIcon(CStickyNotesPlus::GetStickyNotesIcon());

    // Add contents to the main layout
    iMainLayout->addLayout(iButtonLayout);
    this->setFixedSize(TODO_LIST_INITIAL_WIDTH, TODO_LIST_INITIAL_HEIGHT);

    iTimer = new QTimer(this);
    connect(iAddButton, SIGNAL(clicked()), this, SLOT(OnAddButtonPressed()));
    connect(iTimer, SIGNAL(timeout()), this, SLOT(OnTimerExpiry()));

    setAnimated(true);
    setDockOptions(AnimatedDocks);
    iTimerActive = false;
}

/** Destructor of the class */
CStickyToDoList::~CStickyToDoList()
{
    if(iMainLayout)
    {
        delete iMainLayout;
        iMainLayout = NULL;
    }
    if(iButtonLayout)
    {
        delete iButtonLayout;
        iButtonLayout = NULL;
    }
    if(iAddButton)
    {
        delete iAddButton;
        iAddButton = NULL;
    }
    if(iScrollArea)
    {
        delete iScrollArea;
        iScrollArea = NULL;
    }
}

/** This function is called when the "Add task" button is pressed */
void CStickyToDoList::OnAddButtonPressed()
{
    int nextId = iToDoList.size();
    CStickyToDoListItem* newTodoItem = new CStickyToDoListItem(nextId, false);
    iToDoList.push_back(newTodoItem);
    iMainLayout->addWidget(newTodoItem, Qt::AlignLeft);
}

/** This function deletes a particular task */
void CStickyToDoList::DeleteTodoTask(int aTodoTaskId)
{
    CStickyToDoListItem* task = iToDoList.at(aTodoTaskId);
    if(task)
    {
        iMainLayout->removeWidget(task);
        iToDoList.erase(iToDoList.begin() + aTodoTaskId);
        delete task;
    }
    SaveAllTasks(true); // Just in case the stupid user exits the application entirely...
}

/** Overriding the "close" event */
void CStickyToDoList::closeEvent(QCloseEvent* event)
{
    QMessageBox::information(this, tr("Sticky Notes Plus"),
                             tr(" Click the middle mouse button \n on the systray icon to hide window"));
    event->ignore();
}

/** This function saves all "to do" tasks to a file */
void CStickyToDoList::SaveAllTasks(bool aCalledFromDelete)
{
    if(iToDoList.size() > 0)
    {
        fstream binaryFile("StickyToDoList.dat",ios::out|ios::binary);

        for(unsigned int index=0; index < iToDoList.size(); ++index)
        {
            StickyTodDoTaskStruct taskStruct;
            CStickyToDoListItem* todoTask = iToDoList.at(index);

            if(aCalledFromDelete)
            {
                todoTask->SetTaskId(index); // Since a task was just deleted, we need to adjust the ids of the tasks we are left with
            }

            taskStruct.isCompleted = todoTask->IsTaskCompleted();
            taskStruct.taskId      = todoTask->GetTaskId();

            string temp = "";
            temp = todoTask->GetTaskDetail().toStdString();

            temp.copy(taskStruct.taskDetail, temp.length(), 0);
            taskStruct.taskDetail[temp.length()] = '\0';

            binaryFile.write(reinterpret_cast<char*>(&taskStruct), sizeof(StickyTodDoTaskStruct));
        }
        binaryFile.close();
    }
}

/** This function reloads the saved to do task list */
void CStickyToDoList::ReloadTasks()
{
    fstream binaryFile("StickyToDoList.dat",ios::in|ios::binary);

    if(!binaryFile.fail())
    {
        do
        {
            StickyTodDoTaskStruct taskStruct;
            binaryFile.read(reinterpret_cast<char*>(&taskStruct), sizeof(StickyTodDoTaskStruct));
            if(binaryFile.eof())
                break;

            CStickyToDoListItem* newTodoTask = new CStickyToDoListItem(taskStruct.taskId, true);
            iToDoList.push_back(newTodoTask);

            QString taskDetail = taskStruct.taskDetail;
            newTodoTask->SetTaskDetail(taskDetail);

            bool taskCompleted = taskStruct.isCompleted;
            newTodoTask->SetTaskCompleted(taskCompleted);

            iMainLayout->addWidget(newTodoTask, Qt::AlignLeft);
        }while(!binaryFile.eof());
    }
    binaryFile.close();
}

/** This function catches all the events sent to this widget */
bool CStickyToDoList::event(QEvent *event)
{
    if(iTimerActive)
    {
        if (event->type() == QEvent::Enter) /** Mouse pointer entering the widget */
        {
            iTimer->stop();
        }
        else if(event->type() == QEvent::Leave) /** Mouse pointer leaving the widget */
        {
            hide();
        }
    }
    return QWidget::event(event);
}

/** This function is used to disable the "timed" disappearance of the "to do" list. It is called when the
'middle button' has been used to launch the "to do" list */
void CStickyToDoList::SetTimerInactive()
{
    iTimerActive = false;
    iTimer->stop();
}

/** This function starts the timer for TODO_AUTO_HIDE_TIMEOUT seconds */
void CStickyToDoList::StartTimer()
{
    iTimer->start(TODO_AUTO_HIDE_TIMEOUT);
    iTimer->setSingleShot(true);
    iTimerActive = true;
}

/** This function is called when the timer expires */
void CStickyToDoList::OnTimerExpiry()
{
    iTimerActive = false;
    hide();
}
