#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QObject(parent)
{
    this->icon_menu = new QMenu();
    QAction* suspend_resume_action = new QAction(tr("Suspend / Resume"), this);
    connect(suspend_resume_action, SIGNAL(triggered()), this, SLOT(suspend_resume()));
    this->icon_menu->addAction(suspend_resume_action);

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
}

void MainWindow::suspend_resume()
{

}

void MainWindow::task_select()
{

}

void MainWindow::quit()
{

}
