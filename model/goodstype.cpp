#include "goodstype.h"

GoodsType::GoodsType()
{
}

void GoodsType::Name(QString name)
{
    m_name = name;
}

QString GoodsType::Name(void)
{
    return m_name;
}

int GoodsType::ID(void)
{
    return m_id;
}

void GoodsType::ID(int id)
{
    m_id = id;
}

