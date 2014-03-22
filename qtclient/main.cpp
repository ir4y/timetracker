#include <QApplication>

#include "client.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow::authorization_loop();
    MainWindow window;
    return a.exec();
}
