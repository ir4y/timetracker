#include "mainwindow.h"

void onQuit(GtkMenu *menu, gpointer data)
{
    Q_UNUSED(menu);
    MainWindow *self = static_cast<MainWindow *>(data);
    self->quit();
}

void onSuspendResume(GtkMenu *menu, gpointer data)
{
    Q_UNUSED(menu);
    MainWindow *self = static_cast<MainWindow *>(data);
    self->suspend_resume();
}

void onTaskSelect(GtkMenu *menu, gpointer data)
{
    Q_UNUSED(menu);
    MainWindow *self = static_cast<MainWindow *>(data);
    self->task_select();
}

void onUserSelect(GtkMenu *menu, gpointer data)
{
    Q_UNUSED(menu);
    MainWindow *self = static_cast<MainWindow *>(data);
    self->change_user();
}


MainWindow::MainWindow(QWidget *parent) :
    QObject(parent)
{

    QString desktop;

   desktop = getenv("XDG_CURRENT_DESKTOP");
   is_unity = (desktop.toLower() == "unity");

   if(is_unity) //only use this in unity
   {
        AppIndicator *indicator;
        GtkWidget *menu;
        GtkWidget *selectItem, *changeUserItem ,*quitItem;

        menu = gtk_menu_new();

        // suspend_resume item
        suspend_resume_item = gtk_menu_item_new_with_label("Приостановить");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), suspend_resume_item);
        g_signal_connect(suspend_resume_item, "activate", G_CALLBACK(onSuspendResume), this);
        gtk_widget_show(suspend_resume_item);

        //select task item
        selectItem = gtk_menu_item_new_with_label("Выбрать задачу");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), selectItem);
        g_signal_connect(selectItem, "activate", G_CALLBACK(onTaskSelect), this);
        gtk_widget_show(selectItem);

        //cahnge user item
        changeUserItem = gtk_menu_item_new_with_label("Смена пользователя");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), changeUserItem);
        g_signal_connect(changeUserItem, "activate", G_CALLBACK(onUserSelect), this);
        gtk_widget_show(changeUserItem);

        //quit item
        quitItem = gtk_menu_item_new_with_label("Выход");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), quitItem);
        g_signal_connect(quitItem, "activate", G_CALLBACK(onQuit), this);
        gtk_widget_show(quitItem);

        indicator = app_indicator_new_with_path("timetracker", "icon.svg", APP_INDICATOR_CATEGORY_OTHER,":/icons");

        app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
        app_indicator_set_menu(indicator, GTK_MENU(menu));
   } else {

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
       }

    this->task_dialog = NULL;
}

void MainWindow::suspend_resume()
{
    client->suspend();
    if(is_unity){
        if (client->is_active()){
            GtkWidget *menu_label = gtk_bin_get_child(GTK_BIN(this->suspend_resume_item));
            gtk_label_set_text(GTK_LABEL(menu_label), "Приостановить");
        } else {
            GtkWidget *menu_label = gtk_bin_get_child(GTK_BIN(this->suspend_resume_item));
            gtk_label_set_text(GTK_LABEL(menu_label), "Возобновить");
        }

    }else{
        if (client->is_active())
            this->suspend_resume_action->setText(tr("Приостановить"));
        else
            this->suspend_resume_action->setText(tr("Возобновить"));
    }
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
