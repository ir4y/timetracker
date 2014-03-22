#ifndef TASKSELECTDIALOG_H
#define TASKSELECTDIALOG_H

#include <QDialog>
#include <QCompleter>
#include "client.h"

namespace Ui {
class TaskSelectDialog;
}

class TaskSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TaskSelectDialog(QWidget *parent = 0);
    ~TaskSelectDialog();

    QString get_project();
    QString get_task();
    
private:
    Ui::TaskSelectDialog *ui;
};

#endif // TASKSELECTDIALOG_H
