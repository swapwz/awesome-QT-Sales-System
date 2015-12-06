#include "userinfo.h"

UserInfo::UserInfo()
{
}

int UserInfo::ID()
{
    return m_id;
}

void UserInfo::ID(int id)
{
    m_id = id;
}

QString UserInfo::Name()
{
    return m_name;
}

void UserInfo::Name(QString name)
{
    m_name = name;
}

void UserInfo::NickyName(QString nicky_name)
{
    m_nicky_name = nicky_name;
}

QString UserInfo::NickyName()
{
    return m_nicky_name;
}

QString UserInfo::Password()
{
    return m_passwd;
}

void UserInfo::Password(QString passwd)
{
    m_passwd = passwd;
}
