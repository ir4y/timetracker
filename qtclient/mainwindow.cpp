#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->icon_menu = new QMenu(this);
    this->icon_menu->addAction(minimizeAction);
    this->icon_menu->addAction(maximizeAction);
    this->icon_menu->addAction(restoreAction);
    this->icon_menu->addSeparator();
    this->icon_menu->addAction(quitAction);

    this->icon = new QSystemTrayIcon(this);
    this->icon->setContextMenu(this->icon_menu);

    QIcon icon_image("heart.svg");
    this->icon->setIcon(icon_image);
}
