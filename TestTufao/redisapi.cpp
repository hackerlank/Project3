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
    r = NULL;
}

RedisApi::~RedisApi()
{
   //freeReplyObject(this->r);
   redisFree(this->c);
   this->r = NULL;
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

void RedisApi::addRedisString(QString key, QString value)
{
    QString tmp = QString("set %1 %2").arg(key,value);

    string ba = tmp.toStdString();
    const char *cmd = ba.c_str();

    r = (redisReply*)redisCommand(c, cmd);
    if(NULL == r)
    {
        qDebug()<<"Execut command failure";
        return;
    }
    if(!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK")==0))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return;
    }
    qDebug()<<"Succeed to execute command " <<cmd;
}

bool RedisApi::getRedisString(QString key,char* dataout)
{
    r = (redisReply*)redisCommand(c, "GET %s", key.toUtf8().data());
    strcpy(dataout,r->str);

    return true;
}

void RedisApi::addRedisHmset(QString hmobj, QString key, QString value)
{
    QString tmp = QString("hmset %1 %2 %3").arg(hmobj,key,value);
    string ba = tmp.toStdString();
    const char *cmd = ba.c_str();

    r = (redisReply*)redisCommand(c, cmd);
    if(NULL == r)
    {
        qDebug()<<"Execut command failure";
        return;
    }
    if(!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK")==0))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return;
    }
    qDebug()<<"Succeed to execute command " <<cmd;
}

bool RedisApi::getRedisHmset(QString hmobj, QString key,char* dataout)
{
    QString tmp = QString("hmget %1 %2").arg(hmobj,key);
    string ba = tmp.toStdString();
    const char *cmd = ba.data();

    r = (redisReply*)redisCommand(c, cmd);
    if(NULL == r)
    {
        return false;
    }
    strcpy(dataout,(*r->element)->str);

    return true;
}

void RedisApi::addRedisList(QString listobj, QString value)
{
    QString tmp = QString("lpush %1 %2").arg(listobj, value);
    string ba = tmp.toStdString();
    const char *cmd = ba.c_str();

    r = (redisReply*)redisCommand(c, cmd);
    if(NULL == r)
    {
        qDebug()<<"Execut command failure";
        return;
    }
    if(!(r->type == REDIS_REPLY_INTEGER))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return;
    }
    qDebug()<<"Succeed to execute command " <<cmd;
//...
}

bool RedisApi::getRedisList(QString listobj, int to, int to1,char* dataout)
{
    r = (redisReply*)redisCommand(c, "LRANGE %s %d %d", listobj.toUtf8().data(), to, to1);
    if(NULL == r)
    {
        return false;
    }
    strcpy(dataout,(*r->element)->str);

    return true;
}

void RedisApi::addRedisSet(QString setobj, QString value)
{
    QString tmp = QString("SADD %1 %2").arg(setobj, value);
    string ba = tmp.toStdString();
    const char *cmd = ba.c_str();

    r = (redisReply*)redisCommand(c, cmd);
    if(NULL == r)
    {
        qDebug()<<"Execut command failure";
        return;
    }
    if(!(r->type == REDIS_REPLY_INTEGER))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return;
    }
    qDebug()<<"Succeed to execute command " <<cmd;
}

bool RedisApi::getRedisSet(QString setobj,char* dataout)
{
    r = (redisReply*)redisCommand(c, "SMEMBERS %s",setobj.toUtf8().data());
    if(NULL == r)
    {
        return false;
    }
    strcpy(dataout,(*r->element)->str);

    return true;
}

void RedisApi::RedisMulti()//kai qi shi wu
{
    const char *cmd = "MULTI";

    r = (redisReply*)redisCommand(c, cmd);
    if(!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK")==0))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return;
    }
    qDebug()<<"Succeed to execute command " <<cmd;
}

void RedisApi::RedisExec()//guan bi shi wu
{
    const char *cmd = "EXEC";

    r = (redisReply*)redisCommand(c, cmd);
    if(!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK")==0))
    {
        qDebug()<<"Failed to execute command " <<cmd;
        return;
    }
    qDebug()<<"Succeed to execute command " <<cmd;
}
