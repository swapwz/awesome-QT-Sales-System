#include "purchaseinfomodel.h"
#include "tool/databaseimpl.h"
#include <QSqlError>

PurchaseInfoModel::PurchaseInfoModel()
{
    m_query = DataBaseImpl::open();
}

PurchaseInfoModel::~PurchaseInfoModel()
{
    m_query->clear();
    delete m_query;
    DataBaseImpl::close();
}

bool PurchaseInfoModel::deletePurchaseByID(int id)
{
    bool isExecOK;

    m_query->prepare("DELETE FROM GOODS_PURCHASE WHERE id = :id");
    m_query->bindValue(":id", id);

    isExecOK = m_query->exec();
    if ( true != isExecOK )
    {
        qDebug() << m_query->lastError();
    }

    m_query->clear();
    m_query->finish();
    return isExecOK;
}

bool PurchaseInfoModel::batchInsertPurchaseInfo(QVector<PurchaseInfo *> purchase_list)
{

    int size = purchase_list.size();

    if ( size == 0 )
    {
        return true;
    }

    bool isExecOK;
    QVariantList number_list;
    QVariantList total_list;
    QVariantList time_list;
    QVariantList date_list;
    QVariantList sn_list;
    QVariantList price_list;
    QVariantList unit_list;
    QVariantList supplier_list;
    QVariantList remark_list;
    for (int i = 0; i < size; i++)
    {
        number_list << purchase_list[i]->Number();
        total_list << purchase_list[i]->Total();
        time_list << purchase_list[i]->Time();
        date_list << purchase_list[i]->Date();
        sn_list << purchase_list[i]->SN();
        price_list << purchase_list[i]->Price();
        unit_list << purchase_list[i]->UnitID();
        supplier_list << purchase_list[i]->Supplier();
        remark_list << purchase_list[i]->Remark();
    }
    m_query->prepare(QObject::tr("INSERT INTO GOODS_PURCHASE (PURCHASE_SN, PURCHASE_UNIT_ID, PURCHASE_SUPPLIER, "
                                 "PURCHASE_PRICE, PURCHASE_NUMBER, PURCHASE_DATE, PURCHASE_TIME, PURCHASE_REMARK, PURCHASE_TOTAL) "
                                 " VALUES(:sn, :unit_id, :supplier, :price, :number, :date, :time, :remark, :total)"));
    m_query->bindValue(":sn", sn_list);
    m_query->bindValue(":unit_id", unit_list);
    m_query->bindValue(":supplier", supplier_list);
    m_query->bindValue(":price", price_list);
    m_query->bindValue(":number", number_list);
    m_query->bindValue(":date", date_list);
    m_query->bindValue(":time", time_list);
    m_query->bindValue(":remark", remark_list);
    m_query->bindValue(":total", total_list);
    isExecOK = m_query->execBatch();
    if ( true != isExecOK )
    {
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();
    return isExecOK;
}

