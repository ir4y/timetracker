#include "client.h"

Client::Client(QString server_address, QObject *parent) :
    QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);
    this->server_address = server_address;

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout(QPrivateSignal)),
            this, SLOT(timer_slot(QPrivateSignal)));
    this->timer->start(10 * 60 * 1000);

    this->get_projects();
    this->get_tasks();

    this->sendScreen();
}

void Client::timer_slot(QPrivateSignal)
{
    this->sendScreen();
}


void Client::get_tasks()
{
    QString url = this->server_address + "/api/task/";
    QByteArray response = this->request_get(url);

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject object = document.object();
    QJsonArray data = object["data"].toArray();

    foreach (QJsonValue field, data)
    {
        QJsonObject obj = field.toObject();
        this->tasks[obj["id"].toString()] = obj["title"].toString();
        this->tasks_to_projects[obj["id"].toString()] = obj["project"].toString();
    }

    this->current_task = this->tasks.keys()[0];
}

void Client::get_projects()
{
    QString url = this->server_address + "/api/project/";
    QByteArray response = this->request_get(url);

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject object = document.object();
    QJsonArray data = object["data"].toArray();

    foreach (QJsonValue field, data)
    {
        QJsonObject obj = field.toObject();
        this->projects[obj["id"].toString()] = obj["title"].toString();
    }

    this->current_project = this->projects.keys()[0];
}

QByteArray Client::request_get(QString url)
{
    QNetworkRequest request(url);
    QNetworkReply* reply = this->manager->get(request);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QByteArray data = reply->readAll();
    return data;
}

QByteArray Client::request_post(QString url, QByteArray &data, QString content_type_header)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, content_type_header);
    QNetworkReply* reply = this->manager->post(request, data);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QByteArray result = reply->readAll();
    return result;

}

// first, change status
void Client::change_status(QString action)
{
    QJsonObject document;
    document.insert("task", QJsonValue::fromVariant(this->current_task));
    document.insert("user", QJsonValue::fromVariant("532d1d7172905d55e266f494"));
    document.insert("action", QJsonValue::fromVariant(action));
    QByteArray data = QJsonDocument(document).toJson();
    QString url = this->server_address + "/api/action/";

    this->request_post(url, data);
}

// second, send photo
void Client::sendScreen()
{
    QPixmap image = QPixmap::grabWindow(QApplication::desktop()->winId());

    QByteArray buffer_data;
    QBuffer buffer(&buffer_data);
    buffer.open(QIODevice::ReadWrite);
    image.save(&buffer, "PNG");

    QString url = this->server_address + "/api/upload/";
    this->request_post(url, buffer_data, "image/png");

    // GET IMAGE URL

    QString image_url;
    QJsonObject document;
    document.insert("task", QJsonValue::fromVariant(this->current_task));
    document.insert("user", QJsonValue::fromVariant("532d1d7172905d55e266f494"));
    document.insert("image", QJsonValue::fromVariant(image_url));
    QByteArray data = QJsonDocument(document).toJson();
    url = this->server_address + "/api/screen/";

    this->request_post(url, data);
}
