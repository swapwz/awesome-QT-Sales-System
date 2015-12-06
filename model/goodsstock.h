#ifndef GOODSSTOCK_H
#define GOODSSTOCK_H

#include <QtGui>
class GoodsStock
{
public:
    GoodsStock();
    int ID(void) { return m_id; }
    void ID(int id) { m_id = id; }
    QString SN(void) { return m_sn; }
    void SN(QString sn) { m_sn = sn; }
    int Number(void) { return m_number; }
    void Number(int num) { m_number = num; }
    int UnitID(void) { return m_unit_id; }
    void UnitID(int id) { m_unit_id = id; }
    double PurchasePrice(void) { return m_purchase_price; }
    void PurchasePrice(double price) { m_purchase_price = price; }
    double SellPrice(void) { return m_sell_price; }
    void SellPrice(double price) { m_sell_price = price; }
    QDate Date(void) {return m_date;}
    void Date(QDate date) {m_date = date;}
    QString isDisdount(void) {return m_is_discount; }
    void setDiscount(QString isDiscount) { m_is_discount  = isDiscount; }
    QString isSmall(void){ return m_is_small; }
    void setSmall(QString isSmall) { m_is_small = isSmall; }
private:
    int m_id;
    QString m_sn;
    int m_number;
    int m_unit_id;
    double m_purchase_price;
    double m_sell_price;
    QDate m_date;
    QString m_is_discount;
    QString m_is_small;
};

#endif // GOODSSTOCK_H
