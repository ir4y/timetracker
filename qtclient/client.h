#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QEventLoop>
#include <QDebug>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QBuffer>
#include <QScreen>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString server_address, QObject *parent = 0);

    void authenticate(QString login, QString password);
    void sendScreen(); // 1 - upload image; 2 - send data
    void updateTask(QString project, QString task);
    void suspend();
    void change_status(QString action);


    QString get_project() { return this->projects[this->current_project]; }
    QString get_task() { return this->tasks[this->current_task]; }

signals:
    
private slots:
    void timer_slot(QPrivateSignal s);

private:
    QByteArray request_get(QString url);
    QByteArray request_post(QString url, QByteArray& data, QString content_type_header="application/json");

    void sendScreen2();
    void get_tasks();
    void get_projects();

    typedef void (Client::* HandlerPointer)(QJsonDocument&);
    QHash<QString, HandlerPointer> handlers;

    void set_handlers();

    // handlers
    void handler_authenticate(QJsonDocument& data);
    void handler_sendScreen(QJsonDocument& data);
    void handler_sendScreen2(QJsonDocument& data);
    void handler_updateTask(QJsonDocument& data);
    void handler_suspend(QJsonDocument& data);
    void handler_resume(QJsonDocument& data);


    QTimer* timer;
    QNetworkAccessManager* manager;
    QString sessionId;
    QString server_address;

    // ids
    QString current_project;
    QString current_task;

    // ids -- names
    QHash<QString, QString> projects;
    QHash<QString, QString> tasks;

    // id -- id
    QHash<QString, QString> tasks_to_projects;
};

#endif // CLIENT_H
