/** Project : Sticky Notes Plus */
/** Author  : Sainagakishore Srikantham */
/** Email   : ssn.kishore@gmail.com */

/** File    : main.cpp */
/** Purpose : This file is from where the execution of the project starts */

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>

#include "StickyNotesPlus.h"

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(stickynotesplus);

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    CStickyNotesPlus* stickyNotesPlus = CStickyNotesPlus::GetInstance();
    stickyNotesPlus->StartApp();

    return app.exec();
}
