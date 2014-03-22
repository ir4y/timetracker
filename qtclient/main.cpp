#include <QApplication>
#include <QMessageBox>

#include "client.h"
#include "mainwindow.h"
#include "authorizationdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    for (;;)
    {
        AuthorizationDialog* dialog = new AuthorizationDialog();
        dialog->exec();

        client = new Client("http://127.0.0.1:5000", dialog->get_login(), dialog->get_password());
        if (client->is_authenticated())
        {
            break;
        }
        else
        {
            QMessageBox::warning(NULL, QObject::tr("Ошибка авторизации"), QObject::tr("Неверный логин или пароль"));
            delete client;
            delete dialog;
        }
    }

    MainWindow window;
    return a.exec();
}
