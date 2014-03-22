#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QObject(parent)
{
    this->icon_menu = new QMenu();
    this->suspend_resume_action = new QAction(tr("Suspend"), this);
    connect(this->suspend_resume_action, SIGNAL(triggered()), this, SLOT(suspend_resume()));
    this->icon_menu->addAction(this->suspend_resume_action);

    QAction* task_select_action = new QAction(tr("Select task"), this);
    connect(task_select_action, SIGNAL(triggered()), this, SLOT(task_select()));
    this->icon_menu->addAction(task_select_action);

    this->icon_menu->addSeparator();

    QAction* quit_action = new QAction(tr("Quit"), this);
    connect(quit_action, SIGNAL(triggered()), this, SLOT(quit()));
    this->icon_menu->addAction(quit_action);

    this->icon = new QSystemTrayIcon();
    this->icon->setContextMenu(this->icon_menu);

    QIcon icon_image("heart.svg");
    this->icon->setIcon(icon_image);
    this->icon->show();

    this->task_dialog = NULL;
}

void MainWindow::suspend_resume()
{
    client->suspend();
    if (client->is_active())
        this->suspend_resume_action->setText("Suspend");
    else
        this->suspend_resume_action->setText("Resume");
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
    QApplication::exit();
}
