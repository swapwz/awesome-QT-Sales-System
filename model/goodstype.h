#ifndef GOODSTYPE_H
#define GOODSTYPE_H
#include <QtGui>
class QString;
class GoodsType
{
public:
    enum {
        TYPE_TOBACCO = 1,
        TYPE_OTHER
    };
    GoodsType();
    void Name(QString name);
    QString Name(void);
    int ID(void);
    void ID(int id);
private:
    int m_id;
    QString m_name;
};

#endif // GOODSTYPE_H
