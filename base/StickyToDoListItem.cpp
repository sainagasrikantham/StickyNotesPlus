/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : StickyToDoListItem.h */
/** Purpose : This file contains the definition for the class CStickyToDoListItem which is an instance of
              a "to do" task */

#include "StickyToDoListItem.h"
#include "StickyNotesPlus.h"
#include "StickyToDoList.h"
#include<QTextEdit>
#include<QHBoxLayout>
#include<QPushButton>
#include<QCheckBox>
#include<QMessageBox>
#include<QLabel>
#include<QString>
#include<string>

#define TEXT_EDIT_WIDTH  150
#define TEXT_EDIT_HEIGHT 50
using std::string;

const int CStickyToDoListItem::iTodoTextLength = 140;

/** Constructor of the class */
CStickyToDoListItem::CStickyToDoListItem(int aTodoTaskId, bool aBeingReloaded):iTodoTaskId(aTodoTaskId)
{
    iEditMode = false;

    iMainLayout = new QHBoxLayout();

    iButtonLayout = new QHBoxLayout();
    iDeleteButton = new QPushButton(tr("Delete"));
    iButtonLayout->addWidget(iDeleteButton);
    iDeleteButton->setVisible(false);

    iEditButton = new QPushButton(tr("Edit"));
    iButtonLayout->addWidget(iEditButton);

    iCheckBoxLayout = new QHBoxLayout();
    iTaskDone = new QCheckBox();
    iCheckBoxLayout->addWidget(iTaskDone);

    iLineEditLayout = new QHBoxLayout();
    iTextEdit = new QTextEdit();
    iTextEdit->setFixedSize(TEXT_EDIT_WIDTH, TEXT_EDIT_HEIGHT);
    iTextEdit->setVisible(false);
    iLineEditLayout->addWidget(iTextEdit);

    iTodoText = new QLabel();
    iTodoText->setFixedSize(TEXT_EDIT_WIDTH, TEXT_EDIT_HEIGHT);
    iTodoText->setWordWrap(true);
    iLineEditLayout->addWidget(iTodoText);

    iMainLayout->addLayout(iCheckBoxLayout);
    iMainLayout->addLayout(iLineEditLayout);
    iMainLayout->addLayout(iButtonLayout);

    QPalette notePalette = iTextEdit->palette();
    notePalette.setColor(QPalette::Base, Qt::yellow);
    iTextEdit->setPalette(notePalette);

    setLayout(iMainLayout);

    if(!aBeingReloaded) // When a new task is being created
    {
        iTodoText->setVisible(false);
        iTextEdit->setVisible(true);
        iTextEdit->setFocus(Qt::ActiveWindowFocusReason);
        iTextEdit->moveCursor(QTextCursor::End);
        iEditMode = true;
        iEditButton->setText("Done");
    }

    QObject::connect(iTaskDone, SIGNAL(stateChanged(int)), this, SLOT(OnCheckBoxStatusChange()));
    QObject::connect(iDeleteButton,SIGNAL(clicked()), this, SLOT(OnDeletePressed()));
    QObject::connect(iEditButton, SIGNAL(clicked()), this, SLOT(OnEditPressed()));
    QObject::connect(iTextEdit, SIGNAL(textChanged()), this, SLOT(CheckTextWidth()));
}

/** Destructor of the class */
CStickyToDoListItem::~CStickyToDoListItem()
{
    if(iTodoText)
    {
        delete iTodoText;
        iTodoText = NULL;
    }
    if(iTextEdit)
    {
        delete iTextEdit;
        iTextEdit = NULL;
    }
    if(iLineEditLayout)
    {
        delete iLineEditLayout;
        iLineEditLayout = NULL;
    }
    if(iTaskDone)
    {
        delete iTaskDone;
        iTaskDone = NULL;
    }
    if(iCheckBoxLayout)
    {
        delete iCheckBoxLayout;
        iCheckBoxLayout = NULL;
    }
    if(iEditButton)
    {
        delete iEditButton;
        iEditButton = NULL;
    }
    if(iDeleteButton)
    {
        delete iDeleteButton;
        iDeleteButton = NULL;
    }
    if(iButtonLayout)
    {
        delete iButtonLayout;
        iButtonLayout = NULL;
    }
    if(iMainLayout)
    {
        delete iMainLayout;
        iMainLayout = NULL;
    }  
}

/** This function is called when the "checkbox" is checked / unchecked */
void CStickyToDoListItem::OnCheckBoxStatusChange()
{
    if(iTaskDone->isChecked())
    {
        iDeleteButton->setVisible(true);
        iEditButton->setVisible(false);
    }
    else
    {
        iDeleteButton->setVisible(false);
        iEditButton->setVisible(true);
    }
}

/** This function is called when the "-" button is pressed */
void CStickyToDoListItem::OnDeletePressed()
{
    CStickyToDoList* todoList = CStickyNotesPlus::GetInstance()->GetToDoList();
    todoList->DeleteTodoTask(iTodoTaskId);
}

/** This function is called when the "to do" textr is clicked */
void CStickyToDoListItem::OnEditPressed()
{
    if(!iEditMode)
    {
        iTodoText->setVisible(false);
        iTextEdit->setVisible(true);
        iEditButton->setText(tr("Done"));
        iEditMode = true;
    }
    else
    {
        QString currentText = iTextEdit->toPlainText();
        iTodoText->setText(currentText);
        iTextEdit->setVisible(false);
        iTodoText->setVisible(true);
        iEditButton->setText(tr("Edit"));
        iEditMode = false;
    }
}

/** Function called on the "text modified" signal to check that the user doesn't surpass 140 characters */
void CStickyToDoListItem::CheckTextWidth()
{
    static int allowedLength = iTodoTextLength;

    QString currentText = iTextEdit->toPlainText();
    string stringText = currentText.toStdString();

    int strLen = stringText.length();

    if( strLen > allowedLength)
    {
        allowedLength = iTodoTextLength + 1;
        stringText = stringText.substr(0, iTodoTextLength-1); /** Trimming back to 140 characters */
        currentText = stringText.c_str();
        currentText.append("\n");
        iTextEdit->setText(currentText);
    }
}

/** This function returns if the task is completed */
bool CStickyToDoListItem::IsTaskCompleted()
{
    return (iTaskDone->isChecked()) ? true : false;
}

/** This function sets the task completed checkbox status */
void CStickyToDoListItem::SetTaskCompleted(bool aTaskCompleted)
{
    iTaskDone->setChecked(aTaskCompleted);
}

/** This function returns the task ID */
int CStickyToDoListItem::GetTaskId()
{
    return iTodoTaskId;
}

/** This function sets the task ID */
void CStickyToDoListItem::SetTaskId(int aTaskId)
{
    iTodoTaskId = aTaskId;
}

/** This function returns the task detail */
QString CStickyToDoListItem::GetTaskDetail()
{
    return iTextEdit->toPlainText();
}

/** This function sets the task detail */
void CStickyToDoListItem::SetTaskDetail(QString aTaskDetail)
{
    iTextEdit->setText(aTaskDetail);
    iTodoText->setText(aTaskDetail);
}
