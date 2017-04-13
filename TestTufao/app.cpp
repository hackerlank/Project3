#include "app.h"
#include "redisapi.h"
#include <QCoreApplication>
#include <QDebug>
#include <QString>

#include <QUrl>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>

#include <QFile>

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <argp.h>

using namespace std;
using namespace Tufao;

void testfunc()
{
    char buf[1024];
    memset(buf, 0,sizeof(buf));
    RedisApi red;
    red._connectRidis("127.0.0.1", 6379);
    red.addRedisString("studen","cangxiaojie");
    red.getRedisString("studen",buf);
    qDebug()<<buf;

    red.addRedisHmset("AVnvyou","name","hello");
    red.getRedisHmset("AVnvyou","name",buf);
    qDebug()<<buf;

    red.addRedisList("mylist","aaa");
    red.getRedisList("mylist",0, -1, buf);
    qDebug()<<buf;

    red.addRedisSet("myset","kkk");
    red.getRedisSet("myset",buf);
    qDebug()<<buf;
}

App::App(QObject *parent) :
    QObject(parent)
{
    testfunc();//test RedsiApi

    server = new HttpServer(this);
    connect(server, SIGNAL(requestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&)),
            this, SLOT(slotRequestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&)));

    if(!server->listen(QHostAddress::Any, 9014))
    {
        qDebug()<<"bind error";
    }
    else
    {
        qDebug()<<"bind ok";
    }

}
App::~App()
{
    free(server);
}

void App::handleRegister(QString& username, QString& password)
{
    QSqlDatabase    dbconn;
    QSqlQuery       query;
    dbconn = QSqlDatabase::addDatabase("QMYSQL");
    dbconn.setHostName("127.0.0.1");
    dbconn.setDatabaseName("team_three");
    dbconn.setUserName("root");
    dbconn.setPassword("Aa123456");
    if(!dbconn.open())
    {
        qDebug()<< "this,warning,failure";
        return;
    }
    else
    {
        qDebug()<< "this,ok,success";
    }

    query = (QSqlQuery)dbconn;
    QString yourSql = QString("insert into user_table(username, password) valuses('%1','%2')")
                       .arg(username,password);
    query.exec(yourSql);
}

void App::selectDatabase()
{
    QSqlDatabase    dbconn;
     QSqlQuery       query;
    query = (QSqlQuery)dbconn;
    QString yourSql = QString("insert into user_table(username, password) valuses('%1','%2')")
                       .arg(username,password);
    QSqlQuery res = query.exec(yourSql);
    res.next();

}

void App::handleLogin(QJsonObject& obj, QString& username, QString& password)
{
    if(username == "aaa" && password== "bbb")
        {
            // {result: "ok"}
            obj.insert("result", QString("login ok!"));
        }
        else
        {
            // {result: "err"}

            obj.insert("result", QString("err"));
            obj.insert("reason", QString("username or password error"));
        }
}

void App::handlePostData(Tufao::HttpServerRequest& request,Tufao::HttpServerResponse& response)
{
    QByteArray data = request.readBody();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject root = doc.object();
        QString comd = root.value("comd").toString();
        QString username = root.value("username").toString();
        QString password = root.value("password").toString();

        QJsonDocument dc;
        QJsonObject obj = dc.object();

        if(comd == "register")
        {
            //register
            handleRegister(username, password);
        }
        else
        {
            //login
            handleLogin(obj, username, password);
        }

        {
            QJsonDocument dc(obj);
            QByteArray buf = dc.toJson();
            response.writeHead(HttpResponseStatus::OK);
            response.end(buf);
        }

}

void App::slotRequestReady(Tufao::HttpServerRequest& request,Tufao::HttpServerResponse& response)
{
    if(request.method() == "POST")
        {
            connect(&request, &HttpServerRequest::end, [&](){
            handlePostData(request, response);
            });
        }
        else
        {
            qDebug()<< "method:" << request.method();
            qDebug()<< "url:" << request.url();
            qDebug()<< "httpVersion:" << (int)request.httpVersion();

            Tufao::Headers &headers = request.headers();
            for(auto it = headers.begin(); it != headers.end(); ++it)
            {
                qDebug()<< it.key()<<"=" << it.value();
            }

            response.writeHead(HttpResponseStatus::OK);
            response.end("i need post method\n");
        }
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    new App;

    return app.exec();
}
