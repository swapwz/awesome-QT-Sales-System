#include "goodstypemodel.h"
#include "tool/databaseimpl.h"
#include <QSqlError>
GoodsTypeModel::GoodsTypeModel()
{
    m_query = DataBaseImpl::open();
}

GoodsTypeModel::~GoodsTypeModel()
{
    m_query->clear();
    delete m_query;
    DataBaseImpl::close();
}

bool GoodsTypeModel::deleteTypeByID(int id)
{
    bool ret;
    m_query->prepare("DELETE FROM GOODS_TYPE WHERE TYPE_ID = :ID");
    m_query->bindValue(":ID", id);
    ret = m_query->exec();
    if ( ret != true )
    {
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();

    return ret;
}

bool GoodsTypeModel::addNewType(GoodsType &type)
{
    bool ret;
    m_query->prepare(QObject::tr("INSERT INTO GOODS_TYPE (TYPE_ID, TYPE_NAME)"
                                 " VALUES (:ID, :NAME)"));
    m_query->bindValue(":ID", type.ID());
    m_query->bindValue(":NAME", type.Name());
    ret = m_query->exec();
    if ( ret != true )
    {
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();

    return ret;
}

QString GoodsTypeModel::getTypeNameByID(int id)
{
    QString type;

    m_query->prepare("SELECT TYPE_NAME FROM GOODS_TYPE WHERE TYPE_ID = :ID");
    m_query->bindValue(":ID", id);
    m_query->exec();

    type = m_query->value(0).toString();

    m_query->clear();
    m_query->finish();
    return type;
}

QStringList GoodsTypeModel::getAllTypeNames(void)
{
    QStringList list;

    m_query->prepare("SELECT TYPE_NAME FROM GOODS_TYPE");
    m_query->exec();

    while (m_query->next())
    {
        list << m_query->value(0).toString();
    }

    m_query->clear();
    m_query->finish();
    return list;
}
