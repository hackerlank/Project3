#include "connedtadabase.h"
#include <QDebug>

ConneDtadabase::ConneDtadabase()
{
    mdbconn = QSqlDatabase::addDatabase("QMYSQL");
    mdbconn.setHostName("127.0.0.1");
    mdbconn.setDatabaseName("team_three");
    mdbconn.setUserName("root");
    mdbconn.setPassword("Aa12345678");
    if(!mdbconn.open())
    {
        qDebug()<< "Warning open database failure";
    }
    else
    {
        qDebug()<< "open database success!";
    }
}
bool ConneDtadabase::databaseDel(QString sql)
{
    QSqlQuery query = (QSqlQuery)mdbconn;
    if(!query.exec(sql))
    {
        qDebug()<< "delete database failure";
        return false;
    }
        return true;
}

bool ConneDtadabase::databaseInsert(QString sql)
{
    QSqlQuery query = (QSqlQuery)mdbconn;
    if(!query.exec(sql))
    {
        return false;
    }
    return true;
}

bool ConneDtadabase::databaseSelect(QString sql)
{
    QSqlQuery query = (QSqlQuery)mdbconn;

    // QString yourSql = QString("select * from user_table where username = '%1' and password ='%2'")
    //         .arg(username,password);
    if(!query.exec(sql))
    {
        return false;
    }
        return query.next();
}
