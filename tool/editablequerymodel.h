#ifndef EDITABLEQUERYMODEL_H
#define EDITABLEQUERYMODEL_H

#include <QSqlQueryModel>

class EditableQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit EditableQueryModel(QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setMyQuery(const QString &query, const QSqlDatabase &db);

private:
    QString m_querystr;
    QSqlDatabase m_db;
    bool setStockSellPrice(int stockID, double stockSellPrice);
    bool setStockPurchasePrice(int stockID, double stockPurchasePrice);
    bool setStockNumber(int stockID, int stockNum);
    void refresh();

signals:

public slots:

};

#endif // EDITABLEQUERYMODEL_H
