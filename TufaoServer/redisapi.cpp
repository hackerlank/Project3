#include "redisapi.h"

#include <iostream>
#include <QDebug>
#include <QString>
#include <argp.h>
#include <hiredis/async.h>

using namespace std;

RedisApi::RedisApi()
{
    c = new redisContext;
    //r = new redisReply;
    reply = NULL;
}

RedisApi::~RedisApi()
{
   //freeReplyObject(this->r);
   redisFree(this->c);
   this->reply = NULL;
   this->c = NULL;
}

bool RedisApi::_connectRidis(QString host, int port)
{
    c = redisConnect(host.toUtf8().data(), port);
    if(c->err)
    {
        cout << "Connect to redisServer faile"<< endl;
        return false;
    }
    qDebug()<<"Connect to redisServer Success";

    return true;
}

bool RedisApi::addRedisString(QString key, QString value)
{
    QString tmp = QString("set %1 %2").arg(key,value);

    string ba = tmp.toStdString();
    const char *cmd = ba.c_str();

    reply = (redisReply*)redisCommand(c, cmd);
    if(NULL == reply)
    {
        qDebug()<<"Execut command failure";
        return false;
    }
    if(!(reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK")==0))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return false;
    }
    qDebug()<<"Succeed to execute command " <<cmd;

    return true;
}

bool RedisApi::getRedisString(QString key,char* dataout)
{
    reply = (redisReply*)redisCommand(c, "GET %s", key.toUtf8().data());
    strcpy(dataout,reply->str);

    return true;
}

bool RedisApi::addRedisHmset(QString hmobj, QString key, QString value)
{
    QString tmp = QString("hmset %1 %2 %3").arg(hmobj,key,value);
    string ba = tmp.toStdString();
    const char *cmd = ba.c_str();

     reply = (redisReply*)redisCommand(c, cmd);
    if(NULL == reply)
    {
        qDebug()<<"Execut command failure";
        return false;
    }
    if(!(reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK")==0))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return false;
    }
    qDebug()<<"Succeed to execute command " <<cmd;

    return true;
}

bool RedisApi::getRedisHmget(QString hmobj, QString key,char* dataout)
{
    QString tmp = QString("hmget %1 %2").arg(hmobj,key);
    string ba = tmp.toStdString();
    const char *cmd = ba.data();
    qDebug()<<cmd;
    reply = (redisReply*)redisCommand(c, cmd);
    if(NULL == reply)
    {
        return false;
    }
    if(!(*reply->element)->str)
    {
       return false;
    }
    strcpy(dataout,(*reply->element)->str);

    return true;
}

bool RedisApi::addRedisList(QString listobj, QString value)
{
    QString tmp = QString("lpush %1 %2").arg(listobj, value);
    string ba = tmp.toStdString();
    const char *cmd = ba.c_str();

    reply = (redisReply*)redisCommand(c, cmd);
    if(NULL == reply)
    {
        qDebug()<<"Execut command failure";
        return false;
    }
    if(!(reply->type == REDIS_REPLY_INTEGER))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return false;
    }
    qDebug()<<"Succeed to execute command " <<cmd;

    return true;
}

bool RedisApi::getRedisList(QString listobj, int to, int to1,char* dataout)
{
    reply = (redisReply*)redisCommand(c, "LRANGE %s %d %d", listobj.toUtf8().data(), to, to1);
    if(NULL == reply)
    {
        return false;
    }
    strcpy(dataout,(*reply->element)->str);

    return true;
}

bool RedisApi::addRedisSet(QString setobj, QString value)
{
    QString tmp = QString("SADD %1 %2").arg(setobj, value);
    string ba = tmp.toStdString();
    const char *cmd = ba.c_str();

    reply = (redisReply*)redisCommand(c, cmd);
    if(NULL == reply)
    {
        qDebug()<<"Execut command failure";
        return false;
    }
    if(!(reply->type == REDIS_REPLY_INTEGER))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return false;
    }
    qDebug()<<"Succeed to execute command " <<cmd;
    return true;
}

bool RedisApi::getRedisSet(QString setobj,char* dataout)
{
    reply = (redisReply*)redisCommand(c, "SMEMBERS %s",setobj.toUtf8().data());
    if(NULL == reply)
    {
        return false;
    }
    strcpy(dataout,(*reply->element)->str);

    return true;
}

bool RedisApi::RedisMulti()//kai qi shi wu
{
    const char *cmd = "MULTI";

    reply = (redisReply*)redisCommand(c, cmd);
    if(!(reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK")==0))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return false;
    }
    qDebug()<<"Succeed to execute command " <<cmd;
    return true;
}

bool RedisApi::RedisExec()//guan bi shi wu
{
    const char *cmd = "EXEC";

    reply = (redisReply*)redisCommand(c, cmd);
    if(!(reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK")==0))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return false;
    }
    qDebug()<<"Succeed to execute command " <<cmd;

    return true;
}
