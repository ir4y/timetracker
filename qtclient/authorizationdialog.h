#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include <QDialog>

namespace Ui {
class AuthorizationDialog;
}

class AuthorizationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AuthorizationDialog(QWidget *parent = 0);
    ~AuthorizationDialog();

    QString get_login();
    QString get_password();
    
private:
    Ui::AuthorizationDialog *ui;
};

#endif // AUTHORIZATIONDIALOG_H
