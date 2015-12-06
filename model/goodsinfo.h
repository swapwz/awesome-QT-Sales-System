#ifndef GOODSINFO_H
#define GOODSINFO_H
#include <QtGui>

class GoodsInfo
{
public:
    enum GOODS_INFO_MODEL {
        GOODS_CODE = 0,    /* 1 商品代码 */
        GOODS_NAME,        /* 2 商品名称 */
        GOODS_TYPE,        /* 3 商品类型 */
        GOODS_SPEC,          /* 4 商品规格 */
        GOODS_MANUFACTURER,  /* 5 商品制造商 */
        GOODS_ORIGIN_PLACE,  /* 6 商品原产地*/
        GOODS_MAINSN,      /* 7 商品大件条码 */
        GOODS_MAINUNIT,    /* 8 商品大件计量单位 */
        GOODS_SUBSN,       /* 9 商品小件条码 */
        GOODS_SUBUNIT,     /* 10 商品小件单位 */
        GOODS_EXCHANGE_RATE, /* 11 大件转小件的汇率 B = Rate * S */
        GOODS_MAX_FIELD
    };
    QString Code();
    void Code(QString code);
    QString Name();
    void Name(QString name);
    QString Spec();
    void Spec(QString spec);
    QString Manufacturer();
    void Manufacturer(QString manu);
    QString OriginPlace();
    void OriginPlace(QString orgin);
    QString MainSerialNum();
    void MainSerialNum(QString main_sn);
    QString SubSerialNum();
    void SubSerialNum(QString sub_sn);
    int MainUnit();
    void MainUnit(int main_unit);
    int SubUnit();
    void SubUnit(int sub_unit);
    void Type(int type);
    int Type();
    void ExchangeRate(int rate);
    int ExchangeRate();
    int ID();
    void ID(int id);
    GoodsInfo();
private:
    QString m_code;
    QString m_name;
    QString m_spec;
    QString m_manufacturer;
    QString m_origin_place;
    QString m_mainsn;
    QString m_subsn;
    int     m_id;
    int     m_sub_unit;
    int     m_main_unit;
    int     m_type;
    int     m_exchange_rate;
};

#endif // GOODSINFO_H
