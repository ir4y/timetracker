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
    if (client->get_current_task().length() == 0)
        window.task_select();

    return a.exec();
}
