#ifndef UNITINFO_H
#define UNITINFO_H

#include <QtGui>

class UnitInfo
{
public:
    enum {
      UNIT_INFO_BUNDLE = 1,
      UNIT_INFO_BOX
    };
    UnitInfo();
    int ID(void) { return m_id; }
    void ID(int id) { m_id = id; }
    QString Name(void) { return m_name; }
    void Name(QString name) { m_name = name; }
private:
    int m_id;
    QString m_name;
};

#endif // UNITINFO_H
