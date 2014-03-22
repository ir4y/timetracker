#include <QApplication>

#include "client.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("allsol");
    QCoreApplication::setApplicationName("timetracker");

    MainWindow::authorization_loop();
    MainWindow window;
    return a.exec();
}
