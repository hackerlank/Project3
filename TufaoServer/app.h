#ifndef APP_H
#define APP_H

#include <QObject>

#include "connedtadabase.h"
#include "redisapi.h"

#include <tufao-1/Tufao/HttpsServer>
#include <tufao-1/Tufao/HttpServer>
#include <tufao-1/Tufao/HttpServerRequest>
#include <tufao-1/Tufao/HttpServerResponse>
#include <tufao-1/Tufao/Headers>

#include <list>
#include <stack>

using namespace Tufao;
using namespace std;

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QObject *parent = 0);
    ~App();

    void handlePostData(Tufao::HttpServerRequest& request,Tufao::HttpServerResponse& response);
    bool handleRegister(QJsonObject& obj,QString& username, QString& password);//register
    bool handleLogin(QJsonObject& obj, QString& username, QString& password);//login
    bool connDatabase(QString cmd,QString& username, QString& password);
    bool driverSaveRedis(QJsonObject root);
    bool passengerSevaRedis(QJsonObject root);
    bool driverUpdatepos(QJsonObject root);
    bool orderSaveRedis(QJsonObject root);
    bool ppSaveRedis(QJsonObject root);
    bool updateStatusSevaRedis(QJsonObject root);
    void todriverOrder(QString& username, QJsonObject& obj);

private:
    HttpServer *server;
    ConneDtadabase db;
    RedisApi red;
    list<QString> name;
    QString orderid;
    QString waitUsername;
    App(const App&);
    App& operator =(const App&);

signals:
    void sigPassengerUpdatepos(QJsonObject&);
    void sigPassengerOrderrequest(QJsonObject&, QJsonObject&);

public slots:
    void slotRequestReady(Tufao::HttpServerRequest& request,Tufao::HttpServerResponse& response);
    void slotPassengerUpdatepos(QJsonObject&);
    void slotPassengerOrderrequest(QJsonObject&, QJsonObject&);
};

#endif // APP_H
