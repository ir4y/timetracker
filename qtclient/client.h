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


#define PERIODIC_TIME_DEFAULT (10 * 60 * 1000)


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString server_address, QString name, QString password, QObject *parent = 0);

    void authenticate();
    void updateTask(QString project, QString task);
    void suspend();
    void change_status(QString action);
    bool is_authenticated();
    bool is_active();


    QString get_current_project() { return this->projects[this->current_project]; }
    QString get_current_task() { return this->tasks[this->current_task]; }

    QStringList get_projects_list() { return this->projects.values(); }
    QStringList get_tasks_list() { return this->tasks.values(); }

    void send_action(QString action);

signals:
    
private slots:
    void timer_slot();

private:
    QByteArray request_get(QString url);
    QByteArray request_post(QString url, QByteArray& data, QString content_type_header="application/json");

    void get_tasks();
    void get_projects();

    QString get_task_id_by_name(QString task_name, QString project_id);
    QString get_project_id_by_name(QString name);

    QTimer* timer;
    QNetworkAccessManager* manager;
    QString name;
    QString password;
    QString session_id;
    QString server_address;
    bool active;
    int timer_remaining_time;

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
