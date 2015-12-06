#ifndef GOODSTYPEMODEL_H
#define GOODSTYPEMODEL_H
#include "goodstype.h"

class QSqlQuery;
class GoodsTypeModel
{
public:
    GoodsTypeModel();
    ~GoodsTypeModel();
    bool addNewType(GoodsType &type);
    bool deleteTypeByID(int id);
    QString getTypeNameByID(int id);
    QStringList getAllTypeNames(void);
private:
    QSqlQuery *m_query;
};

#endif // GOODSTYPEMODEL_H
