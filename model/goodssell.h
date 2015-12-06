#ifndef GOODSSELL_H
#define GOODSSELL_H
#include <QtGui>
class GoodsSell
{
public:
    GoodsSell();
    int ID(void) { return m_id; }
    void ID(int id) { m_id = id; }
    QString SN(void) { return m_sn; }
    void SN(QString sn){ m_sn = sn; }
    int UnitID(void) { return m_id; }
    void UnitID(int id) { m_id = id; }
    int Number(void) {return m_number; }
    void Number(int num) { m_number = num; }
    double Price(void) { return m_price; }
    void Price(double price) { m_price = price; }
    double Total(void) { return m_total; }
    void Total(double total) { m_total = total; }
    double Profit(void) { return m_profit; }
    void Profit(double profit) { m_profit = profit; }
    double DiscountRate(void) { return m_discount_rate; }
    void DiscountRate(double rate)  { m_discount_rate = rate; }
    QDate Date(void) { return m_date; }
    void Date(QDate date) { m_date = date; }
    QTime Time(void) { return m_time; }
    void Time(QTime time) { m_time = time; }
private:
    int m_id;
    QString m_sn;
    int m_unit_id;
    int m_number;
    double m_price;
    double m_total;
    double m_profit;
    double m_discount_rate;
    QDate m_date;
    QTime m_time;
};

#endif // GOODSSELL_H
