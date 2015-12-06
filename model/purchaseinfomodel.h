#ifndef PURCHASEINFOMODEL_H
#define PURCHASEINFOMODEL_H
#include "purchaseinfo.h"
class QSqlQuery;
class PurchaseInfoModel
{
public:
    PurchaseInfoModel();
    ~PurchaseInfoModel();
    bool deletePurchaseByID(int id);
    bool batchInsertPurchaseInfo(QVector<PurchaseInfo *> purchase_list);
private:
    QSqlQuery *m_query;
};

#endif // PURCHASEINFOMODEL_H
