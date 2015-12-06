#include "goodsinfomodel.h"
#include "tool/databaseimpl.h"
#include <QSqlError>

GoodsInfoModel::GoodsInfoModel()
{
    m_query = DataBaseImpl::open();
}

GoodsInfoModel::~GoodsInfoModel()
{
    m_query->clear();
    delete m_query;
    DataBaseImpl::close();
}

bool GoodsInfoModel::batchInsertGoodsInfo(QVector<GoodsInfo *> info_list)
{
    bool execOk = true;
    int size = info_list.size();

    if ( size == 0 )
    {
        return execOk;
    }


    QVariantList nameList;
    QVariantList codeList;
    QVariantList specList;
    QVariantList origList;
    QVariantList mainSNList;
    QVariantList subSNList;
    QVariantList manfList;
    QVariantList mainUnitList;
    QVariantList subUnitList;
    QVariantList rateList;
    QVariantList typeList;

    for ( int i = 0; i < size; i++ )
    {
        typeList << info_list[i]->Type();
        nameList << info_list[i]->Name();
        codeList << info_list[i]->Code();
        origList << info_list[i]->OriginPlace();
        specList << info_list[i]->Spec();
        mainSNList << info_list[i]->MainSerialNum();
        subSNList << info_list[i]->SubSerialNum();
        manfList << info_list[i]->Manufacturer();
        mainUnitList << info_list[i]->MainUnit();
        subUnitList << info_list[i]->SubUnit();
        rateList << info_list[i]->ExchangeRate();
    }
    m_query->prepare(QObject::tr("INSERT INTO GOODS_INFO (GOODS_NAME, GOODS_CODE, GOODS_SPEC, "
                                 "GOODS_ORIGIN_PLACE, GOODS_TYPE, GOODS_MAINSN, GOODS_SUBSN, GOODS_MANUFACTURER, "
                                 "GOODS_MAINUNIT, GOODS_SUBUNIT, GOODS_EXCHANGE_RATE)"
                                 " VALUES (:name, :code, :spec, :orgin, :type, :mainsn, :subsn,"
                                 ":manuf, :mainunit, :subunit, :rate)"));
    m_query->bindValue(":name", nameList);
    m_query->bindValue(":code", codeList);
    m_query->bindValue(":spec", specList);
    m_query->bindValue(":orgin", origList);
    m_query->bindValue(":type", typeList);
    m_query->bindValue(":mainsn", mainSNList);
    m_query->bindValue(":subsn", subSNList);
    m_query->bindValue(":manuf", manfList);
    m_query->bindValue(":mainunit", mainUnitList);
    m_query->bindValue(":subunit", subUnitList);
    m_query->bindValue(":rate", rateList);
    if (true != m_query->execBatch())
    {
        execOk = false;
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();

    return execOk;
}

GoodsInfo* GoodsInfoModel::fillOneGoodsInfo(void)
{
    GoodsInfo *info = new GoodsInfo();

    if ( NULL != info )
    {
        info->Code(m_query->value("GOODS_CODE").toString());
        info->Name(m_query->value("GOODS_NAME").toString());
        info->ExchangeRate(m_query->value("GOODS_EXCHANGE_RATE").toInt());
        info->ID(m_query->value("GOODS_ID").toInt());
        info->Manufacturer(m_query->value("GOODS_MANUFACTURER").toString());
        info->MainSerialNum(m_query->value("GOODS_MAINSN").toString());
        info->SubSerialNum(m_query->value("GOODS_SUBSN").toString());
        info->MainUnit(m_query->value("GOODS_MAINUNIT").toInt());
        info->SubUnit(m_query->value("GOODS_SUBUNIT").toInt());
        info->Spec(m_query->value("GOODS_SPEC").toString());
        info->Type(m_query->value("GOODS_TYPE").toInt());
        info->OriginPlace(m_query->value("GOODS_ORIGIN_PLACE").toString());
    }

    return info;
}

GoodsInfo* GoodsInfoModel::getGoodsInfoByName(QString goods_name)
{
    GoodsInfo *info = NULL;

    m_query->prepare(QObject::tr("SELECT * FROM GOODS_INFO WHERE GOODS_NAME = :GOODS_NAME"));
    m_query->bindValue(":GOODS_NAME", goods_name);
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            info = fillOneGoodsInfo();
        }
    }
    m_query->clear();
    m_query->finish();
    return info;
}

GoodsInfo* GoodsInfoModel::getGoodsInfoBySN(QString sn)
{
    GoodsInfo *info = NULL;

    m_query->prepare(QObject::tr("SELECT * FROM GOODS_INFO WHERE GOODS_MAINSN = :SN OR GOODS_SUBSN = :SN"));
    m_query->bindValue(":SN", sn);
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            info = fillOneGoodsInfo();
        }
    }
    m_query->clear();
    m_query->finish();
    return info;
}

int GoodsInfoModel::getMainUnitBySN(QString sn)
{
    int main_unit = 0;
    GoodsInfo *info = getGoodsInfoBySN(sn);
    if ( NULL != info )
    {
        main_unit = info->MainUnit();
        delete info;
    }
    return main_unit;
}

int GoodsInfoModel::getSubUnitBySN(QString sn)
{
    int sub_unit = 0;
    GoodsInfo *info = getGoodsInfoBySN(sn);
    if ( NULL != info )
    {
        sub_unit = info->SubUnit();
        delete info;
    }
    return sub_unit;
}

int GoodsInfoModel::getRateBySN(QString sn)
{
    int rate = 1;
    m_query->prepare("SELECT GOODS_EXCHANGE_RATE FROM GOODS_INFO WHERE GOODS_SUBSN = :SN OR GOODS_MAINSN = :SN");
    m_query->bindValue(":SN", sn);
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            rate = m_query->value(0).toInt();
            printf("rate %d\r\n", rate);
        }
    }
    m_query->clear();
    m_query->finish();
    return rate;
}

GoodsInfo* GoodsInfoModel::getGoodsInfoByMainSN(QString main_sn)
{
    GoodsInfo *info = NULL;

    m_query->prepare(QObject::tr("SELECT * FROM GOODS_INFO WHERE GOODS_MAINSN = :GOODS_MAINSN"));
    m_query->bindValue(":GOODS_MAINSN", main_sn);
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            info = fillOneGoodsInfo();
        }
    }
    m_query->finish();
    m_query->clear();

    return info;
}


QString GoodsInfoModel::getSubSerialNumByMainSN(QString main_sn)
{
    QString sub_sn = "";
    GoodsInfo *info = getGoodsInfoByMainSN(main_sn);

    if ( NULL != info )
    {
        sub_sn = info->SubSerialNum();
        delete info;
    }

    return sub_sn;
}

QString GoodsInfoModel::getMainSerialNumBySubSN(QString sub_sn)
{
    m_query->prepare(QObject::tr("SELECT GOODS_MAINSN FROM GOODS_INFO WHERE GOODS_SUBSN = :GOODS_SUBSN"));
    m_query->bindValue(":GOODS_SUBSN", sub_sn);
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            return m_query->value(0).toString();
        }
    }
    m_query->clear();
    m_query->finish();
    return "";
}
bool GoodsInfoModel::addGoodsInfo(GoodsInfo &goods)
{
    bool ret;
    m_query->prepare(QObject::tr("INSERT INTO GOODS_INFO (GOODS_NAME, GOODS_CODE, GOODS_SPEC, "
                                 "GOODS_ORIGIN_PLACE, GOODS_TYPE, GOODS_MAINSN, GOODS_SUBSN, GOODS_MANUFACTURER, "
                                 "GOODS_MAINUNIT, GOODS_SUBUNIT, GOODS_EXCHANGE_RATE)"
                                 " VALUES (:name, :code, :spec, :origin_place, :type, :mainsn, :subsn, "
                                 ":manu, :mainunit, :subunit, :rate)"));
    m_query->bindValue(":name", goods.Name());
    m_query->bindValue(":code", goods.Code());
    m_query->bindValue(":spec", goods.Spec());
    m_query->bindValue(":origin_place", goods.OriginPlace());
    m_query->bindValue(":type", goods.Type());
    m_query->bindValue(":mainsn", goods.MainSerialNum());
    m_query->bindValue(":subsn", goods.SubSerialNum());
    m_query->bindValue(":manu", goods.Manufacturer());
    m_query->bindValue(":mainunit", goods.MainUnit());
    m_query->bindValue(":subunit", goods.SubUnit());
    m_query->bindValue(":rate", goods.ExchangeRate());
    ret = m_query->exec();
    if ( ret != true )
    {
        qDebug() <<"Insert error";
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();

    return ret;
}
bool GoodsInfoModel::updateSubSNByID(QString name, int goods_id)
{
    bool ret;
    m_query->prepare(QObject::tr("UPDATE goods_info set GOODS_SUBSN = :subsn WHERE GOODS_ID = :ID"));
    m_query->bindValue(":subsn", name);
    m_query->bindValue(":ID", goods_id);
    ret = m_query->exec();
    if ( ret != true )
    {
        qDebug() <<"updateSubSNByID error";
        qDebug() << m_query->lastError();
    }

    m_query->clear();
    return ret;
}

bool GoodsInfoModel::updateGoodsNameByID(QString name, int goods_id)
{
    bool ret;
    m_query->prepare(QObject::tr("UPDATE goods_info set GOODS_NAME = :name WHERE GOODS_ID = :ID"));
    m_query->bindValue(":name", name);
    m_query->bindValue(":ID", goods_id);
    ret = m_query->exec();
    if ( ret != true )
    {
        qDebug() <<"updateGoodsNameByID error";
        qDebug() << m_query->lastError();
    }

    m_query->clear();
    m_query->finish();
    return ret;
}

bool GoodsInfoModel::deleteGoodsByID(int id)
{
    bool ret;

    m_query->prepare("DELETE FROM GOODS_INFO WHERE GOODS_ID = :id");
    m_query->bindValue(":id", id);
    ret = m_query->exec();
    if (ret != true )
    {
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();

    return ret;
}
bool GoodsInfoModel::deleteGoodsByName(QString goods_name)
{
    bool ret;
    m_query->prepare("DELETE FORM GOODS_INFO WHERE GOODS_NAME = :NAME");
    m_query->bindValue(":NAME", goods_name);
    ret = m_query->exec();
    m_query->clear();
    m_query->finish();

    return ret;
}

bool GoodsInfoModel::hasGoods(QString sn)
{
    bool hasGoods = false;

    m_query->prepare("SELECT * FROM GOODS_INFO WHERE GOODS_MAINSN = :sn OR GOODS_SUBSN = :sn");
    m_query->bindValue(":sn", sn);
    if ( true == m_query->exec() )
    {
        hasGoods = m_query->first();
    }
    m_query->clear();
    m_query->finish();
    return hasGoods;
}

QStringList GoodsInfoModel::getGoodsInfoNameListByName(QString goods_name)
{
    QStringList list;
    QString queryStr = QString("select goods_name from goods_info where goods_name like %1%2%3").arg("'%", goods_name, "%'");

    m_query->prepare(queryStr);
    m_query->exec();

    while (m_query->next())
    {
        list << m_query->value(0).toString();
    }

    m_query->clear();
    m_query->finish();
    return list;
}

