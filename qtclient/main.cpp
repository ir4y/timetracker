#include <QApplication>

#include "client.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("allsol");
    QCoreApplication::setApplicationName("timetracker");

    if (!MainWindow::authorization_loop())
        return 0;

    MainWindow window;
    return a.exec();
}
