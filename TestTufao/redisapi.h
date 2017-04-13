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

    void addRedisString(QString key, QString value);
    bool getRedisString(QString key,char* dataout);

    void addRedisHmset( QString hmobj, QString key, QString value);
    bool getRedisHmset(QString hmobj, QString key, char* dataout);

    void addRedisList(QString listobj, QString value);
    bool getRedisList(QString listobj, int to, int to1, char* dataout);

    void addRedisSet(QString setobj, QString value);
    bool getRedisSet(QString setobj, char* dataout);

    void RedisMulti();//kai qi shi wu
    void RedisExec();//guan bi shi wu

private:
    redisContext *c;
    redisReply *r;
    RedisApi(const RedisApi&);
    RedisApi& operator=(const RedisApi&);
};

#endif // REDISAPI_H
