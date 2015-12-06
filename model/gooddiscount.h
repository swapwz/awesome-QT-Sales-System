#ifndef GOODDISCOUNT_H
#define GOODDISCOUNT_H

#include <QtGui>
class GoodDiscount
{
public:
    GoodDiscount();
    int ID();
    void ID(int id);
    QString SN(void);
    void SN(QString sn);
    QDate BeginDate(void);
    void BeginDate(QDate b_date);
    void EndDate(QDate e_date);
    QDate EndDate(void);
    double Rate(void);
    void Rate(double rate);
    QString Remark(void);
    void Remark(QString remark);
private:
    int m_id;
    QString m_sn;
    QDate m_begin_date;
    QDate m_end_date;
    double m_rate;
    QString m_remark;
};

#endif // GOODDISCOUNT_H
