#include "taskselectdialog.h"
#include "ui_taskselectdialog.h"

TaskSelectDialog::TaskSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskSelectDialog)
{
    ui->setupUi(this);

    ui->project->addItems(client->get_projects_list());
    QCompleter* project_completer = new QCompleter(client->get_projects_list(), this);
    project_completer->setCompletionMode(QCompleter::PopupCompletion);
    ui->project->setCompleter(project_completer);

    QCompleter* task_completer = new QCompleter(client->get_tasks_list(), this);
    task_completer->setCompletionMode(QCompleter::PopupCompletion);
    ui->task->setCompleter(task_completer);

    if (client->get_current_project().length() > 0)
        ui->project->setCurrentText(client->get_current_project());

    if (client->get_current_task().length() > 0)
        ui->task->setText(client->get_current_task());
}

TaskSelectDialog::~TaskSelectDialog()
{
    delete ui;
}

QString TaskSelectDialog::get_project()
{
    return ui->project->currentText();
}

QString TaskSelectDialog::get_task()
{
    return ui->task->text();
}
