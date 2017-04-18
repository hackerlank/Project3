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

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <argp.h>

#include <QDateTime>
#include <stdio.h>
#include <QUuid>

#include <QSsl>
#include <QSslKey>
#include <QSslCertificate>

#include <string.h>
#include <list>

using namespace std;
using namespace Tufao;

//redis测试
void testfunc()
{
    QString uuid = QUuid::createUuid().toString();
    qDebug() << "uuid = "<< uuid;
    
    char buf[1024];
    memset(buf, 0,sizeof(buf));
    RedisApi red;
    red._connectRidis("127.0.0.1", 6379);
    red.addRedisString("studen","cangxiaojie");
    red.getRedisString("studen",buf);
    qDebug()<<buf;

    red.addRedisHmset("myhm","name sb","status 0 lat 123 lng 456");
    red.getRedisHmget("myhm","name",buf);
    qDebug()<<buf;

    QDateTime time = QDateTime::currentDateTime();
    //QString str = time.toString("yyyyMMddhhmmssdd");
    QString session = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    qDebug()<<session;

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
    //测试redis
    //testfunc();

    server = new HttpServer(this);
#if 0
    /*@***************************************zhen shu**************************************@*/
    /*@*/    QFile key(":/key.pem");                                                      /*@*/
    /*@*/    key.open(QIODevice::ReadOnly);                                               /*@*/
    /*@*/    server->setPrivateKey(QSslKey(&key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey,  /*@*/
    /*@*/                                 "123456"));                                     /*@*/
    /*@*/                                                                                 /*@*/
    /*@*/    QFile cert(":/cert.pem");                                                    /*@*/
    /*@*/    cert.open(QIODevice::ReadOnly);                                              /*@*/
    /*@*/    server->setLocalCertificate(QSslCertificate(&cert));                         /*@*/
    /*@*************************************************************************************@*/
#endif
    //乘客更新位置信号函数和槽函数绑定
    connect(this, &App::sigPassengerUpdatepos,this, &App::slotPassengerUpdatepos);
    //给司机分配订单信号和槽
    connect(this, &App::sigPassengerOrderrequest,this, &App::slotPassengerOrderrequest);

    //Tufao服务器消息信号和槽
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
    db = ConneDtadabase();

    red._connectRidis("127.0.0.1", 6379);

    QDateTime time = QDateTime::currentDateTime();
    orderid = time.toString("yyyyMMddhhmmssdd");//order ID

    waitUsername = QString("");//wait driver queue

}
App::~App()
{
}

//用户注册
bool App::handleRegister(QJsonObject& obj, QString& username, QString& password)
{

    if(connDatabase("reg",username,password))
    {
        // {result: "ok"}
        obj.insert("cmd",QString("reg"));
        obj.insert("result", QString("ok"));
    }
    else
    {
        // {result: "err"}
        obj.insert("cmd",QString("reg"));
        obj.insert("result", QString("err"));
        obj.insert("reason", QString("username or password error"));
        return false;
    }
    return true;
}

//连接数据库验证登录信息和注册信息
bool App::connDatabase(QString cmd, QString& username, QString& password)
{
    if(cmd == "login")
    {
        QString yourSql = QString("select * from user_table where username = '%1' and password ='%2'")
                .arg(username,password);
        if(!db.databaseSelect(yourSql))
        {
            qDebug()<<"select database failure";
            return false;
        }
        qDebug()<<"select database success";
    }
    else if(cmd == "reg")
    {
        QString yourSql = QString("insert into user_table(username, password) values('%1','%2')")
                .arg(username,password);
        if(!db.databaseInsert(yourSql))
        {
            qDebug()<<"insert database failure";
            return false;
        }
        qDebug()<<"insert database success";
    }

    return true;
}

//司机更新位置存储redis
bool App::driverUpdatepos(QJsonObject root)
{
    QString username = root.value("username").toString();
    QString lat = root.value("lat").toString();
    QString lng = root.value("lng").toString();
    QString geohash = root.value("geohash").toString();

    QDateTime time = QDateTime::currentDateTime();
    QString session = time.toString("yyyy-MM-dd hh:mm:ss ddd");

    bool ret = red.addRedisHmset(username,"session", session);
    ret = red.addRedisHmset(username,"lat", lat);
    ret = red.addRedisHmset(username, "lng",lng);
    ret = red.addRedisHmset(username, "geohash", geohash);

    return ret;
}

//乘客订单保存redis
bool App::orderSaveRedis(QJsonObject root)
{
    QString username = root.value("username").toString();
    QString lat1 = root.value("lat1").toString();
    QString lng1 = root.value("lng1").toString();
    QString lat2 = root.value("lat2").toString();
    QString lng2 = root.value("lng2").toString();
    QString geohash = root.value("geohash").toString();
    QString tel = root.value("tel").toString();

    bool ret = red.addRedisHmset(username+"order","tel",tel);
    ret = red.addRedisHmset(username+"order","lat1",lat1);
    ret = red.addRedisHmset(username+"order","lng1",lng1);
    ret = red.addRedisHmset(username+"order","lat1",lat2);
    ret = red.addRedisHmset(username+"order","lng1",lng2);
    ret = red.addRedisHmset(username+"order","geohash",geohash);
    ret = red.addRedisHmset(username+"order", "orderid",orderid);
    ret = red.addRedisHmset(username+"order","username",username);

    return ret;
}

bool App::ppSaveRedis(QJsonObject root)
{
    QString username = root.value("username").toString();
    QString lat = root.value("lat").toString();
    QString lng = root.value("lng").toString();
    QString cmd = root.value("cmd").toString();

    bool ret = red.addRedisHmset(username+cmd,"lat",lat);
    ret = red.addRedisHmset(username+cmd,"lng",lng);

    return ret;
}

//司机更新状态
bool App::updateStatusSevaRedis(QJsonObject root)
{
    QString username = root.value("username").toString();
    QString status = root.value("status").toString();

    bool ret = red.addRedisHmset(username,"status",status);

    return ret;
}

//给司机分配订单
void App::todriverOrder(QString& username, QJsonObject& obj)
{
    char buf[1024]={0};
    red.getRedisHmget(username+"order","status",buf);
    QString status = QString(buf);

    //如果更新位置的username==成员变量保存的等待接客的司机的username
    if(username == waitUsername)
    {
        memset(buf,0,sizeof(buf));

        red.getRedisHmget(username+"order","tel",buf);
        QString tel = QString(buf);
        red.getRedisHmget(username+"order","lat1",buf);
        QString lat1 = QString(buf);
        red.getRedisHmget(username+"order","lng1",buf);
        QString lng1 = QString(buf);
        red.getRedisHmget(username+"order","lat2",buf);
        QString lat2 = QString(buf);
        red.getRedisHmget(username+"order","lng2",buf);
        QString lng2 = QString(buf);
        red.getRedisHmget(username+"order","orderid",buf);
        QString id = QString(buf);
        red.getRedisHmget(username+"order","username",buf);
        QString pusername = QString(buf);

        //给司机回order报文
        obj.insert("cmd", QString("order"));
        obj.insert("tel", tel);
        obj.insert("lat",lat1);
        obj.insert("lng",lng1);
        obj.insert("orderid",id);
        obj.insert("username",pusername);

        //司机订单分配成功后更改司机状态为"2"
        red.addRedisHmset(username+"order","status",QString("2"));

        //写mysq数据库
        db.databaseInsert(QString("insert into oder_form(PassengerID DriverID initial_latitude initial_longitude destination_latitude destination_longitude cost state)values(%1 %2 '%3' '%4' '%5' '%6' '%6' %7 %8)")
                          .arg(pusername, username, lat1, lng1, lat2, lng2,"35","2"));
    }
    //如果司机订单状态为“2”，给司机发"order"报文
    else if(status == "2")
    {
        memset(buf,0,sizeof(buf));

        obj.insert("cmd", QString("order"));

        red.getRedisHmget(username+"order","tel",buf);
        obj.insert("tel", QString(buf));

        red.getRedisHmget(username+"order","lat1",buf);
        obj.insert("lat",QString(buf));

        red.getRedisHmget(username+"order","lng1",buf);
        obj.insert("lng",QString(buf));

        obj.insert("orderid",orderid);

        red.getRedisHmget(username+"order","username",buf);
        obj.insert("username",QString(buf));
    }
    //如果司机订单状态为“3”，给司机发"pp"报文
    else if(status =="3")
    {
        red.getRedisHmget(username+"order","lat1",buf);
        obj.insert("lat",QString(buf));

        red.getRedisHmget(username+"order","lng1",buf);
        obj.insert("lng",QString(buf));

        obj.insert("cmd", QString("pp"));
    }
    else
    {
        obj.insert("cmd", QString("updatepos"));
        obj.insert("result", QString("ok"));
    }
}

//司机登录信息保存redis
bool App::driverSaveRedis(QJsonObject root)
{
    QString username = root.value("username").toString();
    QString lat = root.value("lat").toString();
    QString lng = root.value("lng").toString();
    //QString geohash = root.value("geohash").toString();

    QDateTime time = QDateTime::currentDateTime();
    QString session = time.toString("yyyy-MM-dd hh:mm:ss ddd");

    bool ret = red.addRedisHmset(username,"session",session);
    ret = red.addRedisHmset(username,"lat",lat);
    ret = red.addRedisHmset(username,"lng",lng);
    //ret = red.addRedisHmset(username, "geohash",geohash);

    return ret;
}

//乘客登录信息保存redis
bool App::passengerSevaRedis(QJsonObject root)
{
    QString username = root.value("username").toString();
    QString lat = root.value("lat").toString();
    QString lng = root.value("lng").toString();
    //QString geohash = root.value("geohash").toString();

    QDateTime time = QDateTime::currentDateTime();
    QString session = time.toString("yyyy-MM-dd hh:mm:ss ddd");

    bool ret = red.addRedisHmset(username+"passenger","session",session);
    ret = red.addRedisHmset(username+"passenger","lat",lat);
    ret = red.addRedisHmset(username+"passenger","lng",lng);
    ret = red.addRedisHmset(username+"passenger","status","passenger");
    //ret = red.addRedisHmset(username, "geohash",geohash);

    return ret;
}

//登录验证
bool App::handleLogin(QJsonObject& obj, QString& username, QString& password)
{
    if(connDatabase("login",username,password))
    {
        // {result: "ok"}
        obj.insert("cmd", QString("login"));
        obj.insert("result", QString("ok"));
    }
    else
    {
        // {result: "err"}
        obj.insert("cmd", QString("login"));
        obj.insert("result", QString("err"));
        obj.insert("reason", QString("username or password error"));
        return false;
    }
    return true;
}

//处理POST数据报文
void App::handlePostData(Tufao::HttpServerRequest& request,Tufao::HttpServerResponse& response)
{
    QByteArray data = request.readBody();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QString cmd = root.value("cmd").toString();
    QString username = root.value("username").toString();
    QString password = root.value("password").toString();
    QString type = root.value("type").toString();

    qDebug()<<"**********************************************************"<<cmd;

    //给用户回报文对象obj
    QJsonDocument dc;
    QJsonObject obj = dc.object();

    if(cmd == "reg")//用户注册
    {
        //register
        handleRegister(obj,username, password);
    }
    else if(cmd == "login")//用户登录
    {
        //login
        if(!handleLogin(obj, username, password)){
            qDebug()<<"login err";
        }

        if(type == "driver")//如果是司机登录
        {
            driverSaveRedis(root);//保存司机登录信息
            name.push_back(username);
            red.addRedisHmset(username,"status","0");//司机登录状态初始化置"0"
            red.addRedisHmset(username+"passenger","status","0");//将此用户乘客登录状态置"0"
            red.addRedisHmset(username+"order","status",QString("0"));//将司机订单状态置"0"
        }
        else//乘客登录
        {
            passengerSevaRedis(root);//保存乘客信息
            red.addRedisHmset(username,"status","0");//将此用户司机登录状态置"0"
            red.addRedisHmset(username+"passenger","status","passenger");//将此用户乘客登录状态置"passenger"
            red.addRedisHmset(username+"passengerorder","status","0");//将乘客订单状态置为"0"
        }
    }
    else if(cmd == "updatepos")//客户端更新位置
    {
        /*************************************************/
        QString lat = root.value("lat").toString();
        QString lng = root.value("lng").toString();
        qDebug()<<username;
        qDebug()<<lat;
        qDebug()<<lng;
        /*************************************************/

        char buf[1024] = {0};
        red.getRedisHmget(username+"passenger","status",buf);
        QString pStatus = buf;
        qDebug()<<"------------------------------------------------------"<<pStatus;
        if(pStatus == "passenger")//乘客更新位置状态是"passenger"
        {
            red.getRedisHmget(username+"passengerorder","status",buf);
            QString porderStatus = buf;
            if (porderStatus == "3")//如果乘客的订单状态是"3"给乘客回"arrive"报文
            {
                obj.insert("cmd",QString("updatepos"));
                obj.insert("result" ,QString("arrive"));
//                red.addRedisHmset(username,"orderstatus","0");//将乘客状态置为初始值
            }
            else//处理乘客更新位置并给乘客返回附近司机的位置
                emit sigPassengerUpdatepos(obj);
        }
        else//司机更新位置
        {
            if(!driverUpdatepos(root))//如果司机更新位置存储redis失败给司机回更新超时报文
            {
                obj.insert("cmd", QString("updatepos"));
                obj.insert("result", QString("err"));
                obj.insert("reason",QString("session time out"));
            }

            else//司机更新位置成功
            {
                todriverOrder(username, obj);//给司机分配订单
            }
        }
    }

    else if(cmd == "updatestatus")//司机更新状态
    {
        QString username = root.value("username").toString();
        QString status = root.value("status").toString();
        qDebug()<<username;
        qDebug()<<status;

        if(!updateStatusSevaRedis(root))//成功
        {
            obj.insert("cmd", QString("updatepos"));
            obj.insert("result", QString("err"));
            obj.insert("reason",QString("session time out"));
        }
        else//失败
        {
            obj.insert("cmd", QString("updatestatus"));
            obj.insert("result", QString("ok"));
        }
    }
    else if(cmd == "updateorder")//司机发乘客已到达报文
    {
        
        obj.insert("cmd", QString("updateorder"));
        obj.insert("result",QString("ok"));
        QString status = root.value("status").toString();
        red.addRedisHmset(username,"status",status);//更新司机订单状态
        if(status == "3")//如果订单状态是"3"
        {
            char buf[1024]={0};
            red.getRedisHmget(username+"order","username",buf);
            QString pusername = QString(buf);
            red.addRedisHmset(pusername+"passengerorder","status","3");//将乘客订单状态置为"3"
        }
    }
    //乘客约车
    else if(cmd == "orderrequest")
    {
        obj.insert("cmd",QString("orderrequest"));
        emit sigPassengerOrderrequest(root, obj);//给乘客分配订单
    }

    //给用户回报文
    {
        QJsonDocument dc(obj);
        QByteArray buf = dc.toJson();
        response.writeHead(HttpResponseStatus::OK);
        response.end(buf);
    }
}

//处理GET和POST报文槽函数
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

//处理乘客更新位置槽函数
void App::slotPassengerUpdatepos(QJsonObject& obj)
{
    list<QString>::iterator it;
    char buf[1024] = {0};

    for(it = name.begin(); it != name.end(); ++it)
    {
        QString username = *it;

        if(!red.getRedisHmget(username,"status",buf))
        {
            obj.insert("cmd",QString("updatepos"));
            obj.insert("lat", QString(" "));
            obj.insert("lng", QString(" "));

            break;
        }
        QString status = QString(buf);
        if(status == "wait")
        {
            obj.insert("cmd",QString("updatepos"));

            red.getRedisHmget(username, "lat",buf);
            obj.insert("lat", QString(buf));

            red.getRedisHmget(username, "lng",buf);
            obj.insert("lng", QString(buf));

            return;
        }
    }
    obj.insert("cmd",QString("updatepos"));
    obj.insert("lat", QString(" "));
    obj.insert("lng", QString(" "));
}

//乘客约车处理槽函数
void App::slotPassengerOrderrequest(QJsonObject &root, QJsonObject& obj)
{
    QString lat1 = root.value("lat1").toString();
    QString lng1 = root.value("lng1").toString();
    QString lat2 = root.value("lat2").toString();
    QString lng2 = root.value("lng2").toString();
    QString geohash = root.value("geohash").toString();
    QString tel = root.value("tel").toString();
    QString username = root.value("username").toString();

    //char *str = geohash.toUtf8().data();
    string ba = geohash.toStdString();
    const char *str = ba.c_str();

    list<QString>::iterator it;
    char buf[1024] = {0};

    //在成员list变量遍历判断在线的司机是否匹配乘客订单需求
    for(it = name.begin(); it != name.end(); ++it)
    {
        waitUsername = *it;

        if(!red.getRedisHmget(waitUsername,"status",buf))
        {
            break;
        }
        QString status = QString(buf);
        red.getRedisHmget(waitUsername,"geohash",buf);
        QString geohash = buf;

        //如果司机的状态是"wait"并且
        if(status == "wait" && strncmp(str, buf, 5)==0)
        {
            obj.insert("result", QString("ok"));
            obj.insert("username", waitUsername);
            obj.insert("tel",QString("drever tel"));

            QDateTime time = QDateTime::currentDateTime();
            orderid = time.toString("yyyyMMddhhmmssdd");
            obj.insert("orderid", QString(orderid));

            red.getRedisHmget(waitUsername, "lat",buf);
            obj.insert("lat",QString(buf));

            red.getRedisHmget(waitUsername, "lng",buf);
            obj.insert("lng", QString(buf));

            //保存司机订单
            red.addRedisHmset(waitUsername+"order","tel",tel);
            red.addRedisHmset(waitUsername+"order","lat1",lat1);
            red.addRedisHmset(waitUsername+"order","lng1",lng1);
            red.addRedisHmset(waitUsername+"order","lat1",lat2);
            red.addRedisHmset(waitUsername+"order","lng1",lng2);
            red.addRedisHmset(waitUsername+"order","geohash",geohash);
            red.addRedisHmset(waitUsername+"order", "orderid",orderid);
            red.addRedisHmset(waitUsername+"order","username",username);

            //如果订单分配成功把司机的订单状态置为"2"
            red.addRedisHmset(waitUsername+"order","status","2");

            //orderSaveRedis(root);//保存乘客订单信息
            red.addRedisHmset(username+"passengerorder","tel",tel);
            red.addRedisHmset(username+"passengerorder","lat1",lat1);
            red.addRedisHmset(username+"passengerorder","lng1",lng1);
            red.addRedisHmset(username+"passengerorder","lat1",lat2);
            red.addRedisHmset(username+"passengerorder","lng1",lng2);
            red.addRedisHmset(username+"passengerorder","geohash",str);
            red.addRedisHmset(username+"passengerorder", "orderid",orderid);
            red.addRedisHmset(username+"passengerorder","username",waitUsername);

            return;
        }
    }
    waitUsername = "";
    obj.insert("result", QString("err"));
    obj.insert("reason", QString("No driver pick list"));
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    new App;

    return app.exec();
}
