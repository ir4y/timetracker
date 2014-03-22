#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client("http://127.0.0.1:5000");
    return 0;
}
