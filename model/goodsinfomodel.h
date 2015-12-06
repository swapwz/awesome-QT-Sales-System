#ifndef GOODSINFOMODEL_H
#define GOODSINFOMODEL_H
#include "goodsinfo.h"
class QString;
class QSqlQuery;
class GoodsInfoModel
{
public:
    GoodsInfoModel();
    ~GoodsInfoModel();
    bool hasGoods(QString sn);
    GoodsInfo* getGoodsInfoBySN(QString sn);
    GoodsInfo* getGoodsInfoByName(QString goods_name);
    GoodsInfo* getGoodsInfoByMainSN(QString main_sn);
    QString getSubSerialNumByMainSN(QString main_sn);
    QString getMainSerialNumBySubSN(QString sub_sn);
    QStringList getGoodsInfoNameListByName(QString goods_name);
    bool batchInsertGoodsInfo(QVector<GoodsInfo *>);
    bool addGoodsInfo(GoodsInfo &goods);
    bool updateGoodsNameByID(QString name, int goods_id);
    bool updateSubSNByID(QString name, int goods_id);
    bool deleteGoodsByName(QString goods_name);
    bool deleteGoodsByID(int id);
    int getRateBySN(QString sn);
    int getMainUnitBySN(QString sn);
    int getSubUnitBySN(QString sn);
private:
    QSqlQuery *m_query;
    GoodsInfo* fillOneGoodsInfo(void);
};

#endif // GOODSINFOMODEL_H
