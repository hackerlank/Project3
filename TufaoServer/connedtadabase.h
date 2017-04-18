#ifndef CONNEDTADABASE_H
#define CONNEDTADABASE_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class ConneDtadabase
{
public:
    ConneDtadabase();
    bool databaseDel(QString sql);
    bool databaseInsert(QString sql);
    bool databaseSelect(QString sql);
//private:
    QSqlDatabase    mdbconn;
};

#endif // CONNEDTADABASE_H
