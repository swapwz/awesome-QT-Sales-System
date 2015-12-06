#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMap>

class DataBaseImpl
{
public:
    static QSqlQuery * open(QString);
    static QSqlQuery * open();
    static void close(QString db_name);
    static void close();
    static bool isConnected();
    static bool isConnected(QString db_name);
    static QSqlDatabase connection(void);
private:
    DataBaseImpl() {}
};

#endif // DATABASEIMPL_H
