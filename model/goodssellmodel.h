#ifndef GOODSSELLMODEL_H
#define GOODSSELLMODEL_H
#include "goodssell.h"
class QSqlQuery;
class GoodsSellModel
{
public:
    GoodsSellModel();
    ~GoodsSellModel();
    bool deleteSellByID(int id);
    bool batchInsertSell(QVector<GoodsSell *> sell_list);
private:
    QSqlQuery *m_query;
};

#endif // GOODSSELLMODEL_H
