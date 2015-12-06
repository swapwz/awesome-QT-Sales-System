#ifndef USERINFO_H
#define USERINFO_H
#include <QtGui>
class QString;
class UserInfo
{
public:
    UserInfo();
    int ID(void);
    void ID(int id);
    QString Name(void);
    void Name(QString name);
    QString NickyName(void);
    void NickyName(QString nicky_name);
    QString Password(void);
    void Password(QString passwd);
private:
    QString m_name;
    QString m_nicky_name;
    QString m_passwd;
    int m_id;
};

#endif // USERINFO_H
