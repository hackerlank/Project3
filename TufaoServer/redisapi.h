#ifndef REDISAPI_H
#define REDISAPI_H

#include <hiredis/hiredis.h>
#include <QString>
#include <string>

class RedisApi
{
public:
    RedisApi();
    ~RedisApi();
    bool _connectRidis(QString host, int port);

    bool addRedisString(QString key, QString value);
    bool getRedisString(QString key,char* dataout);

    bool addRedisHmset( QString hmobj, QString key, QString value);
    bool getRedisHmget(QString hmobj, QString key,char* dataout);

    bool addRedisList(QString listobj, QString value);
    bool getRedisList(QString listobj, int to, int to1, char* dataout);

    bool addRedisSet(QString setobj, QString value);
    bool getRedisSet(QString setobj, char* dataout);

    bool RedisMulti();//kai qi shi wu
    bool RedisExec();//guan bi shi wu

private:
    redisContext *c;
    redisReply *reply;
    RedisApi(const RedisApi&);
    RedisApi& operator=(const RedisApi&);
};

#endif // REDISAPI_H
