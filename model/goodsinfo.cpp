#include "goodsinfo.h"

GoodsInfo::GoodsInfo()
{
}

QString GoodsInfo::Code()
{
    return m_code;
}

void GoodsInfo::Code(QString code)
{
    m_code = code;
}

void GoodsInfo::Manufacturer(QString manu)
{
    m_manufacturer = manu;
}

QString GoodsInfo::Manufacturer()
{
    return m_manufacturer;
}
void GoodsInfo::Name(QString name)
{
    m_name = name;
}

QString GoodsInfo::Name()
{
    return m_name;
}

void GoodsInfo::Spec(QString spec)
{
    m_spec = spec;
}

QString GoodsInfo::Spec()
{
    return m_spec;
}

void GoodsInfo::OriginPlace(QString origin)
{
    m_origin_place = origin;
}

QString GoodsInfo::OriginPlace()
{
    return m_origin_place;
}

void GoodsInfo::ExchangeRate(int rate)
{
    m_exchange_rate = rate;
}

int GoodsInfo::ExchangeRate()
{
    return m_exchange_rate;
}

void GoodsInfo::MainSerialNum(QString main_sn)
{
    m_mainsn = main_sn;
}

QString GoodsInfo::MainSerialNum()
{
    return m_mainsn;
}

void GoodsInfo::SubSerialNum(QString sub_sn)
{
    m_subsn = sub_sn;
}

QString GoodsInfo::SubSerialNum()
{
    return m_subsn;
}

int GoodsInfo::Type()
{
    return m_type;
}

void GoodsInfo::Type(int type)
{
    m_type = type;
}

int GoodsInfo::MainUnit()
{
    return m_main_unit;
}

void GoodsInfo::MainUnit(int main_unit)
{
    m_main_unit = main_unit;
}

int GoodsInfo::SubUnit()
{
    return m_sub_unit;
}

void GoodsInfo::SubUnit(int sub_unit)
{
    m_sub_unit = sub_unit;
}

int GoodsInfo::ID()
{
    return m_id;
}

void GoodsInfo::ID(int id)
{
    m_id = id;
}
