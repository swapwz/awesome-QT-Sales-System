#ifndef UNITINFOMODEL_H
#define UNITINFOMODEL_H
#include "unitinfo.h"
class QSqlQuery;
class UnitInfoModel
{
public:
    UnitInfoModel();
    ~UnitInfoModel();
    QStringList getAllUnitName(void);
    QString getUnitNameByID(int id);
private:
    QSqlQuery *m_query;
};

#endif // UNITINFOMODEL_H
