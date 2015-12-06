#include "goodsstockmodel.h"
#include "tool/databaseimpl.h"
#include <QSqlError>

#define STOCK_DATE "STOCK_DATE"
#define STOCK_ID  "STOCK_ID"
#define STOCK_SN "STOCK_SN"
#define STOCK_NUMBER "STOCK_NUMBER"
#define STOCK_UNIT_ID "STOCK_UNIT_ID"
#define STOCK_PURCHASE_PRICE "STOCK_PURCHASE_PRICE"
#define STOCK_IS_DISCOUNT "STOCK_IS_DISCOUNT"
#define STOCK_IS_SMALL "STOCK_IS_SMALL"
#define STOCK_SELL_PRICE "STOCK_SELL_PRICE"

GoodsStockModel::GoodsStockModel()
{
    m_query = DataBaseImpl::open();
}

GoodsStockModel::~GoodsStockModel()
{
    m_query->clear();
    delete m_query;
    DataBaseImpl::close();
}

bool GoodsStockModel::updateStockSNBySN(QString old, QString sn)
{
    m_query->prepare(QObject::tr("UPDATE GOODS_STOCK SET STOCK_SN = :sn1 "
                                 " WHERE STOCK_SN = :sn2"));
    m_query->bindValue(":sn1", sn);
    m_query->bindValue(":sn2", old);
    bool execOK = m_query->exec();
    if ( true != execOK )
    {
        qDebug("update stocks\r\n");
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();
    return execOK;
}

bool GoodsStockModel::batchUpdateGoodsStock(QVector<GoodsStock *> stock_list)
{
    int size = stock_list.size();

    if ( 0 == size )
    {
        return true;
    }

    QVariantList sn_list;
    QVariantList num_list;
    QVariantList purchase_list;
    QVariantList date_list;
    QVariantList sell_list;
    for ( int i = 0; i < size; i++ )
    {
        sn_list << stock_list[i]->SN();
        num_list << stock_list[i]->Number();
        date_list << stock_list[i]->Date();
        sell_list << stock_list[i]->SellPrice();
        purchase_list << stock_list[i]->PurchasePrice();
    }

    m_query->prepare(QObject::tr("UPDATE GOODS_STOCK SET STOCK_NUMBER = :number, STOCK_PURCHASE_PRICE = :purchase,"
                                 " STOCK_SELL_PRICE = :sell, STOCK_DATE = :date "
                                 " WHERE STOCK_SN = :sn "));
    m_query->bindValue(":number", num_list);
    m_query->bindValue(":purchase", purchase_list);
    m_query->bindValue(":sell", sell_list);
    m_query->bindValue(":date", date_list);
    m_query->bindValue(":sn", sn_list);
    bool execOK = m_query->execBatch();
    if ( true != execOK )
    {
        qDebug("update stocks\r\n");
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();
    return execOK;
}

int GoodsStockModel::getStockNumberBySN(QString sn)
{
    int num = 0;
    m_query->prepare("SELECT STOCK_NUMBER FROM GOODS_STOCK WHERE STOCK_SN = :SN");
    m_query->bindValue(":SN", sn);
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            num = m_query->value(0).toInt();
        }
    }
    else
    {
        qDebug("get stock number\r\n");
        qDebug() << m_query->lastError();
    }

    m_query->clear();
    m_query->finish();
    return num;
}

bool GoodsStockModel::batchInsertGoodsStock(QVector<GoodsStock *> stock_list)
{
    bool execOK;
    int size = stock_list.size();

    if ( size == 0 )
    {
        return true;
    }

    QVariantList sn_list;
    QVariantList num_list;
    QVariantList unit_list;
    QVariantList sell_list;
    QVariantList purchase_list;
    QVariantList issmall_list;
    QVariantList isdiscount_list;
    QVariantList date_list;
    for ( int i = 0; i < size; i++ )
    {
        sn_list << stock_list[i]->SN();
        num_list << stock_list[i]->Number();
        sell_list <<  stock_list[i]->SellPrice();
        purchase_list << stock_list[i]->PurchasePrice();
        issmall_list << stock_list[i]->isSmall();
        unit_list << stock_list[i]->UnitID();
        date_list << stock_list[i]->Date();
    }

    m_query->prepare(QObject::tr("INSERT INTO GOODS_STOCK (STOCK_SN, STOCK_NUMBER, STOCK_UNIT_ID, "
                                 "STOCK_PURCHASE_PRICE, STOCK_SELL_PRICE, STOCK_IS_SMALL, STOCK_DATE) "
                                 " VALUES (:SN, :NUMBER, :UNIT_ID, :PURCHASE_PRICE, :SELL_PRICE, :IS_SMALL, :DATE)"));
    m_query->bindValue(":SN", sn_list);
    m_query->bindValue(":NUMBER", num_list);
    m_query->bindValue(":UNIT_ID", unit_list);
    m_query->bindValue(":PURCHASE_PRICE", purchase_list);
    m_query->bindValue(":SELL_PRICE", sell_list);
    m_query->bindValue(":IS_SMALL", issmall_list);
    m_query->bindValue(":DATE", date_list);

    execOK = m_query->execBatch();
    if ( true != execOK )
    {
        qDebug("add stocks\r\n");
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();

    return execOK;
}

bool GoodsStockModel::deleteStockBySN(QString sn)
{
    bool isExecOK;

    m_query->prepare("DELETE FROM GOODS_STOCK WHERE STOCK_SN = :sn ");
    m_query->bindValue(":sn", sn);

    isExecOK = m_query->exec();
    if ( true != isExecOK )
    {
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();
    return isExecOK;
}

bool GoodsStockModel::deleteStockByID(int id)
{
    bool isExecOK;

    m_query->prepare("DELETE FROM GOODS_STOCK WHERE STOCK_ID = :id ");
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

bool GoodsStockModel::hasGoods(QString sn)
{
    bool hasGoods = false;

    m_query->prepare(QObject::tr("SELECT * FROM GOODS_STOCK WHERE STOCK_SN = :sn"));
    m_query->bindValue(":sn", sn);
    if ( true == m_query->exec() )
    {
        hasGoods = m_query->first();
    }
    m_query->clear();
    m_query->finish();
    return hasGoods;
}
GoodsStock * GoodsStockModel::getStockBySN(QString sn)
{
    GoodsStock *stock = NULL;

    m_query->prepare(QObject::tr("SELECT * FROM GOODS_STOCK WHERE STOCK_SN = :sn"));
    m_query->bindValue(":sn", sn);
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            stock = new GoodsStock();
            stock->Date(m_query->value(STOCK_DATE).toDate());
            stock->ID(m_query->value(STOCK_ID).toInt());
            stock->setSmall(m_query->value(STOCK_IS_SMALL).toString() );
            stock->setDiscount(m_query->value(STOCK_IS_DISCOUNT).toString());
            stock->Number(m_query->value(STOCK_NUMBER).toInt());
            stock->PurchasePrice(m_query->value(STOCK_PURCHASE_PRICE).toDouble());
            stock->SellPrice(m_query->value(STOCK_SELL_PRICE).toDouble());
            stock->SN(sn);
            stock->UnitID(m_query->value(STOCK_UNIT_ID).toInt());
        }
    }
    else
    {
        qDebug("getStockBySN\r\n");
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();
    return stock;
}

bool GoodsStockModel::addStock(GoodsStock &stock)
{
    bool ret;
    m_query->prepare(QObject::tr("INSERT INTO GOODS_STOCK (STOCK_SN, STOCK_NUMBER, STOCK_UNIT_ID, "
                                 "STOCK_PURCHASE_PRICE, STOCK_SELL_PRICE, STOCK_DATE, "
                                 "STOCK_IS_DISCOUNT, STOCK_IS_SMALL) "
                                 " VALUES (:SN, :NUMBER, :UNIT_ID, :PURCHASE_PRICE, :SELL_PRICE, "
                                 ":DATE, :IS_DISCOUNT, :IS_SMALL)"));
    m_query->bindValue(":SN", stock.SN());
    m_query->bindValue(":NUMBER", stock.Number());
    m_query->bindValue(":UNIT_ID", stock.UnitID());
    m_query->bindValue(":PURCHASE_PRICE", stock.PurchasePrice());
    m_query->bindValue(":SELL_PRICE", stock.SellPrice());
    m_query->bindValue(":DATE", stock.Date());
    m_query->bindValue(":IS_DISCOUNT", stock.isDisdount());
    m_query->bindValue(":IS_SMALL", stock.isSmall());

    ret = m_query->exec();

    m_query->finish();
    return ret;
}
