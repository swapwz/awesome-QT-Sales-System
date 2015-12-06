#include "unitinfomodel.h"
#include "tool/databaseimpl.h"

UnitInfoModel::UnitInfoModel()
{
    m_query = DataBaseImpl::open();
}

UnitInfoModel::~UnitInfoModel()
{
    m_query->clear();
    delete m_query;
    DataBaseImpl::close();
}

QStringList UnitInfoModel::getAllUnitName(void)
{
    QStringList unit_list;
    m_query->prepare("SELECT UNIT_NAME FROM UNIT_INFO");
    m_query->exec();

    while (m_query->next())
    {
        unit_list << m_query->value(0).toString();
    }

    m_query->clear();
    m_query->finish();
    return unit_list;
}

QString UnitInfoModel::getUnitNameByID(int id)
{
    QString unit_name = "";
    m_query->prepare("SELECT UNIT_NAME FROM UNIT_INFO WHERE UNIT_ID = :ID");
    m_query->bindValue(":ID", QVariant(id).toInt());
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            unit_name = m_query->value(0).toString();
        }
    }
    m_query->clear();
    m_query->finish();
    return unit_name;
}
