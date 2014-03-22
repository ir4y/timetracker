#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QIcon>
#include <QAction>

class MainWindow : public QObject
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void suspend_resume();
    void task_select();
    void quit();
    
private:
    QSystemTrayIcon* icon;
    QMenu* icon_menu;
};

#endif // MAINWINDOW_H
