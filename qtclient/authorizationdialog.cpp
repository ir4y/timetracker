#include "authorizationdialog.h"
#include "ui_authorizationdialog.h"

AuthorizationDialog::AuthorizationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthorizationDialog)
{
    ui->setupUi(this);
}

AuthorizationDialog::~AuthorizationDialog()
{
    delete ui;
}

QString AuthorizationDialog::get_login()
{
    return ui->login->text();
}

QString AuthorizationDialog::get_password()
{
    return ui->password->text();
}
