#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QObject(parent)
{
    this->icon_menu = new QMenu();
    this->suspend_resume_action = new QAction(tr("Приостановить"), this);
    connect(this->suspend_resume_action, SIGNAL(triggered()), this, SLOT(suspend_resume()));
    this->icon_menu->addAction(this->suspend_resume_action);

    QAction* task_select_action = new QAction(tr("Выбрать задачу"), this);
    connect(task_select_action, SIGNAL(triggered()), this, SLOT(task_select()));
    this->icon_menu->addAction(task_select_action);

    QAction* change_user_action = new QAction(tr("Смена пользователя"), this);
    connect(change_user_action, SIGNAL(triggered()), this, SLOT(change_user()));
    this->icon_menu->addAction(change_user_action);

    this->icon_menu->addSeparator();

    QAction* quit_action = new QAction(tr("Выход"), this);
    connect(quit_action, SIGNAL(triggered()), this, SLOT(quit()));
    this->icon_menu->addAction(quit_action);

    this->icon = new QSystemTrayIcon();
    this->icon->setContextMenu(this->icon_menu);

    QIcon icon_image(":/icons/icon.svg");
    this->icon->setIcon(icon_image);
    this->icon->show();

    this->task_dialog = NULL;
}

void MainWindow::suspend_resume()
{
    client->suspend();
    if (client->is_active())
        this->suspend_resume_action->setText(tr("Приостановить"));
    else
        this->suspend_resume_action->setText(tr("Возобновить"));
}

void MainWindow::task_select()
{
    if (this->task_dialog)
    {
        this->task_dialog->close();
        this->task_dialog->deleteLater();
    }

    this->task_dialog = new TaskSelectDialog();
    this->task_dialog->show();
    connect(this->task_dialog, SIGNAL(accepted()), this, SLOT(dialog_accepted()));
}

void MainWindow::dialog_accepted()
{
    client->updateTask(this->task_dialog->get_project(), this->task_dialog->get_task());
}

void MainWindow::quit()
{
    if (client->is_active())
        client->send_action("suspend");

    QApplication::exit();
}

void MainWindow::change_user()
{
    if (client->is_active())
        client->send_action("suspend");

    QSettings settings;
    settings.remove("login");
    settings.remove("password");
    MainWindow::authorization_loop();
}


bool MainWindow::authorization_loop()
{
    QSettings settings;
    for (;;)
    {
        QString login = settings.value("login", "").toString();
        QString password = settings.value("password", "").toString();

        if (login.length() == 0 || password.length() == 0)
        {
            AuthorizationDialog* dialog = new AuthorizationDialog();
            int status = dialog->exec();

            if (status == 0)
            {
                QApplication::exit();
                return false;
            }

            login = dialog->get_login();
            password = dialog->get_password();
            delete dialog;
        }

        if (client)
            delete client;

        QString server_address = settings.value("server", "http://127.0.0.1:5000").toString();
        client = new Client(server_address, login, password);
        if (client->is_authenticated())
        {
            settings.setValue("login", login);
            settings.setValue("password", password);
            break;
        }
        else
        {
            settings.remove("login");
            settings.remove("password");
            QMessageBox::warning(NULL, QObject::tr("Ошибка авторизации"), QObject::tr("Неверный логин или пароль"));
            delete client;
            client = NULL;
        }
    }

    return true;
}
