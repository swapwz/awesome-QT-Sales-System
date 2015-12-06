#include "editablequerymodel.h"
#include <QtSql>

EditableQueryModel::EditableQueryModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

enum GOODS_STOCK_TABLE{
    TABLE_STOCK_ID,
    TABLE_STOCK_DATE,
    TABLE_STOCK_SN,
    TABLE_STOCK_NAME,
    TABLE_STOCK_NUMBER,
    TABLE_STOCK_UNIT_ID,
    TABLE_STOCK_PURCHASE_PRICE,
    TABLE_STOCK_SELL_PRICE
};
Qt::ItemFlags EditableQueryModel::flags(
        const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if ( index.column() == TABLE_STOCK_NUMBER ||
         index.column() == TABLE_STOCK_PURCHASE_PRICE ||
         index.column() == TABLE_STOCK_SELL_PRICE)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool EditableQueryModel::setStockPurchasePrice(int stockID, double stockPurchasePrice)
{
    QSqlQuery query(m_db);
    query.prepare("update goods_stock set stock_purchase_price = ? where stock_id = ?");
    query.addBindValue(stockPurchasePrice);
    query.addBindValue(stockID);
    return query.exec();
}

bool EditableQueryModel::setStockSellPrice(int stockID, double stockSellPrice)
{
    QSqlQuery query(m_db);
    query.prepare("update goods_stock set stock_sell_price = ? where stock_id = ?");
    query.addBindValue(stockSellPrice);
    query.addBindValue(stockID);

    if ( true != query.exec() )
    {
        qDebug() << query.lastError();
    }
    return query.exec();
}

void EditableQueryModel::setMyQuery(const QString &query, const QSqlDatabase &db)
{
    m_querystr = query;
    m_db = db;
    setQuery(query, db);
}

bool EditableQueryModel::setStockNumber(int stockID, int stockNum)
{
    QSqlQuery query(m_db);
    query.prepare("update goods_stock set stock_number = ? where stock_id = ?");
    query.addBindValue(stockNum);
    query.addBindValue(stockID);
    return query.exec();
}

void EditableQueryModel::refresh()
{
    setQuery(m_querystr, m_db);
    setHeaderData(TABLE_STOCK_ID, Qt::Horizontal, QObject::tr("库存号"));
    setHeaderData(TABLE_STOCK_DATE, Qt::Horizontal, QObject::tr("日期"));
    setHeaderData(TABLE_STOCK_SN, Qt::Horizontal, QObject::tr("商品条码"));
    setHeaderData(TABLE_STOCK_NAME, Qt::Horizontal, QObject::tr("名称"));
    setHeaderData(TABLE_STOCK_NUMBER, Qt::Horizontal, QObject::tr("库存数量"));
    setHeaderData(TABLE_STOCK_UNIT_ID, Qt::Horizontal, QObject::tr("单位"));
    setHeaderData(TABLE_STOCK_PURCHASE_PRICE, Qt::Horizontal, QObject::tr("采购价"));
    setHeaderData(TABLE_STOCK_SELL_PRICE, Qt::Horizontal, QObject::tr("销售价"));
}

bool EditableQueryModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
    int column = index.column();

    if ( (column != TABLE_STOCK_NUMBER) && (column != TABLE_STOCK_SELL_PRICE) && (column != TABLE_STOCK_PURCHASE_PRICE) )
    {
        return false;
    }

    QModelIndex stockIDIndex = QSqlQueryModel::index(index.row(), TABLE_STOCK_ID);
    int id = data(stockIDIndex).toInt();
    clear();

    bool ok;
    if (column == TABLE_STOCK_NUMBER) {
        ok = setStockNumber(id, value.toInt());
    }
    else if (column == TABLE_STOCK_SELL_PRICE)
    {
        ok = setStockSellPrice(id, value.toDouble());
    }
    else if (column == TABLE_STOCK_PURCHASE_PRICE)
    {
        ok = setStockPurchasePrice(id, value.toDouble());
    }
    else
    {
        ok = false;
    }
    refresh();
    return ok;
}



