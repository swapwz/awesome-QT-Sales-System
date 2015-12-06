#ifndef USERINFOMODEL_H
#define USERINFOMODEL_H

#include "userinfo.h"

class QSqlQuery;
class UserInfoModel
{
public:
    UserInfoModel();
    ~UserInfoModel();
    UserInfo* getUserInfoByName(QString user_name);
    UserInfo* getUserInfoByID(int user_id);
    bool updateUserInfoByID(int id, UserInfo &info);
    bool updateUserNameByID(int id, QString name);
    bool updateUserNickyNameByID(int id, QString nicky_name);
    bool updatePasswdByID(int id, QString passwd);
    bool addUserInfo(UserInfo &info);
    bool deleteUserInfoByID(int id);
    bool deleteUserInfoByName(QString user_name);
private:
    QSqlQuery *m_query;
};

#endif // USERINFOMODEL_H
