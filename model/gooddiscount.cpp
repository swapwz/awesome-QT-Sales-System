#include "gooddiscount.h"

GoodDiscount::GoodDiscount()
{
}

int GoodDiscount::ID()
{
    return m_id;
}

void GoodDiscount::ID(int id)
{
    m_id = id;
}
QString GoodDiscount::SN(void)
{
    return m_sn;
}
void GoodDiscount::SN(QString sn)
{
    m_sn = sn;
}
QDate GoodDiscount::BeginDate(void)
{
    return m_begin_date;
}
void GoodDiscount::BeginDate(QDate b_date)
{
    m_begin_date = b_date;
}
void GoodDiscount::EndDate(QDate e_date)
{
    m_end_date = e_date;
}
QDate GoodDiscount::EndDate(void)
{
    return m_end_date;
}
double GoodDiscount::Rate(void)
{
    return m_rate;
}
void GoodDiscount::Rate(double rate)
{
    m_rate = rate;
}
QString GoodDiscount::Remark(void)
{
    return m_remark;
}
void GoodDiscount::Remark(QString remark)
{
    m_remark = remark;
}
