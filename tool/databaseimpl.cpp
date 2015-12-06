#include "databaseimpl.h"

#define DEFAULT_DATABASE_NAME "shop_user.db"

static QSqlDatabase m_dbConnection;
static QMap<QString, int> m_conn_ref_map;

QSqlDatabase DataBaseImpl::connection(void)
{
    return m_dbConnection;
}

QSqlQuery* DataBaseImpl::open(QString db_name)
{
    QSqlQuery *result = NULL;
    if ( m_dbConnection.isOpen() != true )
    {
        m_dbConnection = QSqlDatabase::addDatabase("QSQLITE", db_name);
        m_dbConnection.setDatabaseName(db_name);
        if ( true == m_dbConnection.open())
        {
            m_conn_ref_map[db_name] = 1;
            qDebug("Database %s open success\r\n", db_name.toStdString().c_str());
        }
        else
        {
            qDebug("Database %s open failed\r\n", db_name.toStdString().c_str());
            return result;
        }
    }

    if ( true == m_conn_ref_map.contains(db_name) )
    {
        m_conn_ref_map[db_name]++;
        qDebug("Database %s already opend, reference count %d\r\n", db_name.toStdString().c_str(), m_conn_ref_map[db_name]);
    }

    result = new QSqlQuery(m_dbConnection);
    return result;
}

QSqlQuery* DataBaseImpl::open()
{
    return open(DEFAULT_DATABASE_NAME);
}

void DataBaseImpl::close(QString db_name)
{
    if ( true == m_conn_ref_map.contains(db_name) )
    {
        if ( 0 < m_conn_ref_map[db_name] )
        {
            m_conn_ref_map[db_name]--;
            qDebug("Database %s dereferenced, count %d \r\n", db_name.toStdString().c_str(), m_conn_ref_map[db_name]);
        }

        if ( 0 == m_conn_ref_map[db_name])
        {
            m_dbConnection.removeDatabase(db_name);
            m_conn_ref_map.remove(db_name);
            qDebug("Database %s closed\r\n", db_name.toStdString().c_str());
        }

        if ( true == m_conn_ref_map.isEmpty() )
        {
            m_dbConnection.close();
            qDebug("All database removed, close the connection\r\n");
        }
    }

    return ;
}

void DataBaseImpl::close()
{
    close(DEFAULT_DATABASE_NAME);
    return;
}

bool DataBaseImpl::isConnected()
{
    return m_conn_ref_map.contains(DEFAULT_DATABASE_NAME);
}

bool DataBaseImpl::isConnected(QString db_name)
{
    return m_conn_ref_map.contains(db_name);
}
