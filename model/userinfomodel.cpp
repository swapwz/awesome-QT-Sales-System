#include "userinfomodel.h"
#include "tool/databaseimpl.h"
#include <QSqlError>

UserInfoModel::UserInfoModel()
{
    m_query = DataBaseImpl::open();
}

UserInfoModel::~UserInfoModel()
{
    m_query->clear();
    delete m_query;
    DataBaseImpl::close();
}

UserInfo*  UserInfoModel::getUserInfoByID(int user_id)
{
    UserInfo *info = NULL;

    m_query->prepare("SELECT * FROM USER_INFO WHERE USER_ID = :ID");
    m_query->bindValue(":ID", user_id);
    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            info = new UserInfo();
            if ( NULL != info )
            {
                info->ID(user_id);
                info->Name(m_query->value("USER_NAME").toString());
                info->NickyName(m_query->value("USER_NICKY_NAME").toString());
                info->Password(m_query->value("USER_PASSWD").toString());
            }
        }
    }

    m_query->clear();
    m_query->finish();
    return info;
}

bool UserInfoModel::updateUserNameByID(int id, QString name)
{
    m_query->prepare(QObject::tr("UPDATE USER_INFO SET USER_NAME = :name WHERE USER_ID = :ID"));
    m_query->bindValue(":name", name);
    m_query->bindValue(":ID", QVariant(id).toInt());

    bool queryOK = m_query->exec();
    if ( true != queryOK )
    {
        qDebug() << m_query->lastError();
    }
    m_query->clear();
    m_query->finish();
    return queryOK;
}

bool UserInfoModel::updatePasswdByID(int id, QString passwd)
{
    char szQuerySql[120];
    szQuerySql[0] = 0;
    snprintf(szQuerySql, sizeof(szQuerySql), "UPDATE user_info SET user_passwd = '%s' where user_id = %d",
             passwd.toStdString().c_str(), id);
    bool queryOK = m_query->exec(szQuerySql);
    if ( true != queryOK )
    {
        qDebug() << m_query->lastError();
    }

    m_query->clear();
    m_query->finish();
    return queryOK;
}

UserInfo* UserInfoModel::getUserInfoByName(QString user_name)
{
    UserInfo *info = NULL;

    m_query->prepare(QObject::tr("SELECT * FROM USER_INFO WHERE USER_NAME = :NAME"));
    m_query->bindValue(":NAME", user_name);

    if ( true == m_query->exec() )
    {
        if ( true == m_query->first() )
        {
            info = new UserInfo();
            if ( NULL != info )
            {
                info->ID(m_query->value("USER_ID").toInt());
                info->Name(m_query->value("USER_NAME").toString());
                info->NickyName(m_query->value("USER_NICKY_NAME").toString());
                info->Password(m_query->value("USER_PASSWD").toString());
            }
        }
    }
    m_query->clear();
    m_query->finish();
    return info;
}
bool UserInfoModel::addUserInfo(UserInfo &info)
{
    bool ret;

    m_query->prepare(QObject::tr("INSERT INTO USER_INFO (USER_NAME, USER_NICKY_NAME, USER_PASSWD) "
                                 "VALUES (:NAME, :NICKY_NAME, :PASSWD)"));
    m_query->bindValue(":NAME", info.Name());
    m_query->bindValue(":NICKY_NAME", info.NickyName());
    m_query->bindValue(":PASSWD", info.Password());
    ret = m_query->exec();

    m_query->clear();
    m_query->finish();
    return ret;
}
