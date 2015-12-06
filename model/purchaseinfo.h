#ifndef PURCHASEINFO_H
#define PURCHASEINFO_H
#include <QtGui>
#include <QTime>

class PurchaseInfo
{
public:
    PurchaseInfo();
    int ID() { return m_id; }
    void ID(int id) {m_id = id; }
    int UnitID() { return m_unit_id; }
    void UnitID(int uid) { m_unit_id = uid; }
    QString Supplier() { return m_supplier; }
    void Supplier(QString supplier) { m_supplier = supplier;}
    double Price() { return m_price; }
    void Price(double price) { m_price = price; }
    double Total(void) { return m_total; }
    void Total(double total) { m_total = total; }
    int Number(void) { return m_number; }
    void Number(int num) { m_number = num; }
    QDate Date(void) { return m_date; }
    void Date(QDate date) { m_date = date; }
    QTime Time(void) { return m_time; }
    void Time(QTime time) { m_time = time; }
    void SN(QString sn) { m_sn = sn; }
    QString SN(void) { return m_sn; }
    QString Remark(void) { return m_remark; }
    void Remark(QString remark) { m_remark = remark; }
private:
    int m_id;
    QString m_sn;
    int m_unit_id;
    QString m_supplier;
    double m_price;
    double m_total;
    int m_number;
    QDate m_date;
    QTime m_time;
    QString m_remark;
};

#endif // PURCHASEINFO_H
