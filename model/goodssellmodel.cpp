#include "goodssellmodel.h"
#include "tool/databaseimpl.h"
#include <QSqlError>

GoodsSellModel::GoodsSellModel()
{
    m_query = DataBaseImpl::open();
}

GoodsSellModel::~GoodsSellModel()
{
    m_query->clear();
    delete m_query;
    DataBaseImpl::close();
}

bool GoodsSellModel::deleteSellByID(int id)
{
    m_query->prepare("delete from goods_sell where sell_id = :id");
    m_query->bindValue(":id", id);

    bool isOK = m_query->exec();
    if ( true != isOK )
    {
        qDebug() << "delete sell";
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();
    return isOK;
}

bool GoodsSellModel::batchInsertSell(QVector<GoodsSell *> sell_list)
{
    int size = sell_list.size();

    if ( size == 0 )
    {
        return true;
    }

    QVariantList sn_list;
    QVariantList num_list;
    QVariantList unit_list;
    QVariantList price_list;
    QVariantList total_list;
    QVariantList profit_list;
    QVariantList discount_list;
    QVariantList date_list;
    QVariantList time_list;
    for ( int i = 0; i < size; i++ )
    {
        sn_list << sell_list[i]->SN();
        num_list << sell_list[i]->Number();

        price_list <<  sell_list[i]->Price();
        total_list << sell_list[i]->Total();
        discount_list << sell_list[i]->DiscountRate();
        profit_list << sell_list[i]->Profit();
        unit_list << sell_list[i]->UnitID();
        date_list << sell_list[i]->Date();
        time_list << sell_list[i]->Time();
    }

    m_query->prepare(QObject::tr("INSERT INTO GOODS_SELL (sell_sn, sell_number, sell_unit_id, "
                                 "sell_price, sell_total, sell_profit, sell_date, sell_time, sell_discount_rate) "
                                 " VALUES (:sn, :number, :unit_id, :price, :total, :profit, :date, :time, :discount)"));
    m_query->bindValue(":sn", sn_list);
    m_query->bindValue(":number", num_list);
    m_query->bindValue(":unit_id", unit_list);
    m_query->bindValue(":price", price_list);
    m_query->bindValue(":total", total_list);
    m_query->bindValue(":profit", profit_list);
    m_query->bindValue(":date", date_list);
    m_query->bindValue(":time", time_list);
    m_query->bindValue(":discount", discount_list);

    bool isOK = m_query->execBatch();
    if ( true != isOK )
    {
        qDebug("add sell\r\n");
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();

    return isOK;
}

