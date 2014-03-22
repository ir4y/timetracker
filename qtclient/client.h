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


    QString get_current_project() { return this->projects[this->current_project]; }
    QString get_current_task() { return this->tasks[this->current_task]; }

    QStringList get_projects_list() { return this->projects.values(); }
    QStringList get_tasks_list() { return this->tasks.values(); }

signals:
    
private slots:
    void timer_slot(QPrivateSignal s);

private:
    QByteArray request_get(QString url);
    QByteArray request_post(QString url, QByteArray& data, QString content_type_header="application/json");

    void get_tasks();
    void get_projects();

    QString get_task_id_by_name(QString task_name, QString project_id);
    QString get_project_id_by_name(QString name);

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


extern Client* client;

#endif // CLIENT_H
