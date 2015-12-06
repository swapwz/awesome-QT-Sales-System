#ifndef GOODSSTOCKMODEL_H
#define GOODSSTOCKMODEL_H
#include "goodsstock.h"

class QSqlQuery;
class GoodsStockModel
{
public:
    GoodsStockModel();
    ~GoodsStockModel();
    GoodsStock * getStockBySN(QString sn);
    bool batchInsertGoodsStock(QVector<GoodsStock *> stock_list);
    bool batchUpdateGoodsStock(QVector<GoodsStock *> stock_list);
    bool addStock(GoodsStock &stock);
    bool hasGoods(QString sn);
    bool deleteStockBySN(QString sn);
    int  getStockNumberBySN(QString sn);
    bool deleteStockByID(int id);
    bool updateStockSNBySN(QString old, QString sn);
private:
    QSqlQuery *m_query;
};

#endif // GOODSSTOCKMODEL_H
