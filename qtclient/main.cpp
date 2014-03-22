#include "client.h"
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    client = new Client("http://127.0.0.1:5000");
    MainWindow window;

    return a.exec();
}
