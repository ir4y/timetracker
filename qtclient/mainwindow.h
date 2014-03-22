#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QIcon>
#include <QAction>
#include <QMessageBox>
#include <QSettings>

#include "taskselectdialog.h"
#include "authorizationdialog.h"

class MainWindow : public QObject
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    static bool authorization_loop();

public slots:
    void suspend_resume();
    void task_select();
    void quit();
    void dialog_accepted();
    void change_user();
    
private:
    QSystemTrayIcon* icon;
    QMenu* icon_menu;
    QAction* suspend_resume_action;

    TaskSelectDialog* task_dialog;
};

#endif // MAINWINDOW_H
