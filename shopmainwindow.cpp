#include "shopmainwindow.h"
#include "ui_shopmainwindow.h"
#include "dialogquerypasswd.h"
#include "loginwindow.h"
#include "tool/databaseimpl.h"
#include "tool/editablequerymodel.h"
#include "model/userinfomodel.h"
#include "model/goodstypemodel.h"
#include "model/goodsinfomodel.h"
#include "model/goodsstockmodel.h"
#include "model/goodssellmodel.h"
#include "model/purchaseinfomodel.h"
#include "model/unitinfomodel.h"
#include "readonlyitemdelegate.h"
#include "selltotalwindow.h"
#include "deletedialog.h"
#include <QTextEdit>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QCompleter>


enum TAB_INDEX
{
    TAB_SELL,
    TAB_ENTRY,
    TAB_INFO,
    TAB_QUERY,
    TAB_PERSONAL,
    TAB_MAX
};

enum INFO_LINEEDIT_TYPE
{
    INFO_BIG_SN = 0,
    INFO_SMALL_SN,
    INFO_STOCK_NUM,
    INFO_GOODS_CODE,
    INFO_BIG2SMALL,
    INFO_BIG_PURCHASE_PRICE,
    INFO_BIG_SELL_PRICE,
    INFO_MANUF,
    INFO_ORIG_PLACE,
    INFO_SMALL_SELL_PRICE,
    INFO_GOODS_NAME,
    INFO_MAX_TYPE
};

enum ENTRY_LINEEDIT_TYPE
{
    ENTRY_MAIN_SN,
    ENTRY_PURCHASE_PRICE,
    ENTRY_MAIN_PRICE,
    ENTRY_STOCK_NUMBER,
    ENTRY_SUB_PRICE,
    ENTRY_ENTRY_NUMBER,
    ENTRY_SUPPLIER,
    ENTRY_NAME,
    ENTRY_SUBSN,
    ENTRY_ENTRY_NUMBER1,
    ENTRY_STOCK_NUMBER1,
    ENTRY_MAX
};

enum CHECK_TYPE {
    CHECK_SN,
    CHECK_SUBSN,
    CHECK_STR,
    CHECK_INT,
    CHECK_SUBINT,
    CHECK_DOUBLE,
    CHECK_SUBDOUBLE,
    CHECK_MAX
};


static CHECK_LINE_EDIT_S g_InfoCheckList[] = {
    { INFO_GOODS_NAME, CHECK_STR, false},
    { INFO_GOODS_CODE, CHECK_STR, false},
    { INFO_BIG_SN, CHECK_SN, false},
    { INFO_SMALL_SN, CHECK_SUBSN, false},
    { INFO_BIG_SELL_PRICE, CHECK_DOUBLE, false},
    { INFO_SMALL_SELL_PRICE, CHECK_SUBDOUBLE, false},
    { INFO_BIG_PURCHASE_PRICE, CHECK_DOUBLE, false},
    { INFO_STOCK_NUM, CHECK_INT, false},
    { INFO_BIG2SMALL, CHECK_INT, false}
};

static CHECK_LINE_EDIT_S g_EntryCheckList[] = {
    { ENTRY_MAIN_SN, CHECK_SN, false },
    { ENTRY_NAME, CHECK_STR, false },
    { ENTRY_SUBSN, CHECK_SN, false },
    { ENTRY_PURCHASE_PRICE, CHECK_DOUBLE, false },
    { ENTRY_MAIN_PRICE, CHECK_DOUBLE, false },
    { ENTRY_SUB_PRICE, CHECK_SUBDOUBLE, false},
    { ENTRY_ENTRY_NUMBER, CHECK_INT, false },
    { ENTRY_ENTRY_NUMBER1, CHECK_SUBINT, false },
    { ENTRY_STOCK_NUMBER, CHECK_INT, false },
    { ENTRY_STOCK_NUMBER1, CHECK_SUBINT, false },
};

enum GOODS_INFO_MODEL {
    GOODS_CODE = 0,          /* 1 商品代码 */
    GOODS_NAME,              /* 2 商品名称 */
    GOODS_TYPE,              /* 3 商品类型 */
    GOODS_SPEC,              /* 4 商品规格 */
    GOODS_MANUFACTURER,      /* 5 商品制造商 */
    GOODS_ORIGIN_PLACE,      /* 6 商品原产地*/
    GOODS_PURCHASE_PRICE,    /* 7 商品进价 */
    GOODS_MAINSN,            /* 8 商品大件条码 */
    GOODS_SELL_MAINPRICE,    /* 9 大件商品售价 */
    GOODS_MAINUNIT,          /* 10 商品大件计量单位 */
    GOODS_SUBSN,             /* 11 商品小件条码 */
    GOODS_SELL_SUBPRICE,     /* 12 小件商品售价 */
    GOODS_SUBUNIT,           /* 13 商品小件单位 */
    GOODS_EXCHANGE_RATE,     /* 14 大件转小件的汇率 B = Rate * S */
    GOODS_STOCK_NUMBER,      /* 15 库存数量 */
    GOODS_MAX_SHOW_FIELD
};
enum GOODS_INFO_HIDE_MODEL
{
    GOODS_HIDE_TYPE = GOODS_MAX_SHOW_FIELD, /* 16 隐藏的商品类型ID */
    GOODS_HIDE_BUID,                   /* 17 隐藏的大件单位ID */
    GOODS_HIDE_SUID,                   /* 18 隐藏的小件单位ID */
    GOODS_MAX_FIELD
};

enum GOODS_STOCK_MODEL{
    STOCK_NAME = 0,          /* 1.名称 */
    STOCK_NUMBER,            /* 2.大件库存*/
    STOCK_NUMBER1,            /* 3.小件库存 */
    STOCK_ENTRY_NUMBER,      /* 4.大件入库数 */
    STOCK_ENTRY_NUMBER1,     /* 5.小件入库数*/
    STOCK_UNIT_NAME,         /* 6.入库单位 */
    STOCK_PURCHASE_PRICE,    /* 7.采购价 */
    STOCK_TOTAL,             /* 8.采购总额 */
    STOCK_MAINSN,            /* 9.大件条码 */
    STOCK_MAIN_PRICE,        /* 10.大件售价 */
    STOCK_SUBSN,             /* 11.小件条码 */
    STOCK_SUB_PRICE,         /* 12.小件售价*/
    STOCK_SUPPLIER,          /* 13.供应商*/
    STOCK_REMARK,            /* 14.备注 */
    STOCK_MAX_SHOW_FIELD
};

enum GOODS_STOCK_HIDE{
    STOCK_HIDE_MAINUNIT = STOCK_MAX_SHOW_FIELD,
    STOCK_HIDE_SUBUNIT,
    STOCK_MAX_FIELD
};


static QVector<QLineEdit *> m_goodsInfoLineEdit;
static QVector<QLineEdit *> m_goodsEntryLineEdit;

ShopMainWindow::ShopMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShopMainWindow)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(TAB_SELL);
    setTabFocus(TAB_SELL);

    delete DataBaseImpl::open();

    /*商品入库tab页处理 */
    setEntryTabOrder();
    initEntryLineEditList();
    setStockTableHeader();
    addShortCutForEntryTab();

    /* 初始化结算窗口 */
    m_sellTotalWindow = new SellTotalWindow();
    connect(m_sellTotalWindow, SIGNAL(sellSubmit(bool)), this, SLOT(sellTotal(bool)));
    /* 商品信息tab页处理 */
    initInfoTabUI();
    initInfoLineEditList();
    addShortCurForInfoTab();
    setInfoTabOrder();
    setInfoTableHeader();

    /* 商品销售tableview设置 */
    setSellTableHeader();

    /* 个人焦点设置 */
    setPersonalTabOrder();

    /* 设置查询下拉列表 */
    m_queryModel = new QSqlQueryModel();
    setQueryComboxItemText();
    initQueryDateUI();
    ui->lEdit_query_nameorsn->setFocus();
    ui->tableView_queryView->setShowGrid(true);
    ui->tableView_queryView->resizeColumnsToContents();
    ui->tableView_queryView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView_queryView->setSelectionBehavior(QAbstractItemView::SelectRows);
    hideBalanceButton(true);

    connect(ui->btn_quit, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(addGoodsStock(QString)), this, SLOT(addGoodsStockBySN(QString)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(setTabFocus(int)));
    connect(this, SIGNAL(wrongPasswd()), this, SLOT(wrongHint()));
    connect(this, SIGNAL(fillInfo(QString)), this, SLOT(fillInfoBySN(QString)));
    connect(ui->comboBox_query_table, SIGNAL(currentIndexChanged(int)), this, SLOT(selectModelForQuery(int)));
}

void ShopMainWindow::initQueryDateUI()
{
    ui->dateEdit_query_begin->setDateRange(QDate(2013, 1, 1), QDate(2049, 1, 1));
    ui->dateEdit_query_end->setDateRange(QDate(2013, 1, 1), QDate(2049, 1, 1));
    ui->dateEdit_query_begin->setDate(QDate(2013, 10, 1));
    ui->dateEdit_query_end->setDate(QDate::currentDate());
}

enum GOODS_SELL_VIEW {
    GOODS_SELL_SN = 0,
    GOODS_SELL_NAME,
    GOODS_SELL_PURCHASES_PRICE,
    GOODS_SELL_PRICE,
    GOODS_SELL_NUMBER,
    GOODS_SELL_UNIT,
    GOODS_SELL_DISCOUNT_RATE,
    GOODS_SELL_TOTAL,
    GOODS_SELL_PROFIT,
    GOODS_SELL_SHOW_MAX
};
enum GOODS_SELL_HIDE_VIEW {
    GOODS_SELL_HIDE_UNIT = GOODS_SELL_SHOW_MAX,
    GOODS_SELL_MAX
};
void ShopMainWindow::setSellTableHeader(void)
{
    QFont font = ui->tableView_sellView->horizontalHeader()->font();
    font.setBold(true);
    ui->tableView_sellView->horizontalHeader()->setFont(font);

    m_sellModel = new QStandardItemModel();

    m_sellModel->setColumnCount(GOODS_SELL_MAX);
    m_sellModel->setHeaderData(GOODS_SELL_SN, Qt::Horizontal, tr("条码"));
    m_sellModel->setHeaderData(GOODS_SELL_NAME, Qt::Horizontal, tr("名称"));
    m_sellModel->setHeaderData(GOODS_SELL_PURCHASES_PRICE, Qt::Horizontal, tr("采购价"));
    m_sellModel->setHeaderData(GOODS_SELL_PRICE, Qt::Horizontal, tr("售价"));
    m_sellModel->setHeaderData(GOODS_SELL_NUMBER, Qt::Horizontal, tr("数量"));
    m_sellModel->setHeaderData(GOODS_SELL_UNIT, Qt::Horizontal, tr("单位"));
    m_sellModel->setHeaderData(GOODS_SELL_DISCOUNT_RATE, Qt::Horizontal, tr("折扣率"));
    m_sellModel->setHeaderData(GOODS_SELL_TOTAL, Qt::Horizontal, tr("总金额"));
    m_sellModel->setHeaderData(GOODS_SELL_PROFIT, Qt::Horizontal, tr("利润"));

    ui->tableView_sellView->setItemDelegateForColumn(GOODS_SELL_TOTAL, new readonlyItemDelegate(this));
    ui->tableView_sellView->setItemDelegateForColumn(GOODS_SELL_NAME, new readonlyItemDelegate(this));
    ui->tableView_sellView->setItemDelegateForColumn(GOODS_SELL_SN, new readonlyItemDelegate(this));
    ui->tableView_sellView->setItemDelegateForColumn(GOODS_SELL_UNIT, new readonlyItemDelegate(this));
    ui->tableView_sellView->setItemDelegateForColumn(GOODS_SELL_PROFIT, new readonlyItemDelegate(this));
    ui->tableView_sellView->setShowGrid(true);
    ui->tableView_sellView->resizeColumnsToContents();
    ui->tableView_sellView->setModel(m_sellModel);
    ui->tableView_sellView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView_sellView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_sellView->hideColumn(GOODS_SELL_PROFIT);
    ui->tableView_sellView->hideColumn(GOODS_SELL_PURCHASES_PRICE);
    ui->tableView_sellView->hideColumn(GOODS_SELL_HIDE_UNIT);

    connect(m_sellModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(updateSellView(QStandardItem*)));
}

void ShopMainWindow::updateSellView(QStandardItem *item)
{
    QModelIndex index;
    int column = item->column();
    int row = item->row();

    switch(column)
    {
    case GOODS_SELL_PURCHASES_PRICE:
    case GOODS_SELL_DISCOUNT_RATE:
    case GOODS_SELL_PRICE:
    case GOODS_SELL_NUMBER:
    {
        /* 获取当前销售数量 */
        index = m_sellModel->index(row, GOODS_SELL_NUMBER);
        int number = index.data().toInt();

        /* 获取当前销售价格 */
        index = m_sellModel->index(row, GOODS_SELL_PRICE);
        double sell_price = index.data().toDouble();

        /* 获取采购价 */
        index = m_sellModel->index(row, GOODS_SELL_PURCHASES_PRICE);
        double purchase_price = index.data().toDouble();

        /* 获取当前折扣 */
        index = m_sellModel->index(row, GOODS_SELL_DISCOUNT_RATE);
        double discount_rate = index.data().toDouble();

        /* 更新总金额 */
        double total = number * sell_price * discount_rate;
        index = m_sellModel->index(row, GOODS_SELL_TOTAL);
        m_sellModel->setData(index, QVariant(total));

        /* 更新利润 */
        double profit = total - (number * purchase_price);
        index = m_sellModel->index(row, GOODS_SELL_PROFIT);
        m_sellModel->setData(index, QVariant(profit));

        break;
    }
    default:
        break;
    }
    return ;
}

enum QUERY_INDEX{
    QUERY_INFO = 1,
    QUERY_STOCK,
    QUERY_SELL,
    QUERY_PURCHASE,
    QUERY_ONCREDIT,
    QUERY_MAX
};

enum ComBoxItemIndex{
    COMBOX_ITEM_INIT = 0,
    COMBOX_ITEM_INFO,
    COMBOX_ITEM_STOCK,
    COMBOX_ITEM_SELL,
    COMBOX_ITEM_PURCHASE,
    COMBOX_ITEM_ON_CREDIT,
    COMBOX_MAX
};
void ShopMainWindow::setQueryComboxItemText(void)
{
    QStringList tableList;
    tableList << tr("请选择要查询的信息") << tr("商品信息")
              << tr("商品库存") << tr("销售信息") << tr("采购信息") << tr("赊账信息");
    ui->comboBox_query_table->addItems(tableList);
}

enum GOODS_INFO_TABLE {
    TABLE_GOODS_ID,
    TABLE_GOODS_CODE,
    TABLE_GOODS_NAME,
    TABLE_GOODS_TYPE,
    TABLE_GOODS_SPEC,
    TABLE_GOODS_MANUFACTURER,
    TABLE_GOODS_ORIGIN_PLACE,
    TABLE_GOODS_MAINSN,
    TABLE_GOODS_MAINUNIT,
    TABLE_GOODS_SUBSN,
    TABLE_GOODS_SUBUNIT,
    TABLE_GOODS_EXCHANGE_RATE
};
void ShopMainWindow::setQueryInfoHeader()
{
    m_queryModel->setHeaderData(TABLE_GOODS_ID, Qt::Horizontal, tr("信息号"));
    m_queryModel->setHeaderData(TABLE_GOODS_CODE, Qt::Horizontal, tr("代码"));
    m_queryModel->setHeaderData(TABLE_GOODS_NAME, Qt::Horizontal, tr("名称"));
    m_queryModel->setHeaderData(TABLE_GOODS_TYPE, Qt::Horizontal, tr("类型"));
    m_queryModel->setHeaderData(TABLE_GOODS_SPEC, Qt::Horizontal, tr("规格"));
    m_queryModel->setHeaderData(TABLE_GOODS_MANUFACTURER, Qt::Horizontal, tr("制造商"));
    m_queryModel->setHeaderData(TABLE_GOODS_ORIGIN_PLACE, Qt::Horizontal, tr("原产地"));
    m_queryModel->setHeaderData(TABLE_GOODS_MAINSN, Qt::Horizontal, tr("大件条码"));
    m_queryModel->setHeaderData(TABLE_GOODS_MAINUNIT, Qt::Horizontal, tr("大件单位"));
    m_queryModel->setHeaderData(TABLE_GOODS_SUBSN, Qt::Horizontal, tr("小件条码"));
    m_queryModel->setHeaderData(TABLE_GOODS_SUBUNIT, Qt::Horizontal, tr("小件单位"));
    m_queryModel->setHeaderData(TABLE_GOODS_EXCHANGE_RATE, Qt::Horizontal, tr("转换率"));
    ui->tableView_queryView->setModel(m_queryModel);
}

void ShopMainWindow::showInfoTable()
{
    setInfoQuery("SELECT GOODS_ID, GOODS_CODE, GOODS_NAME, "
                 "(select TYPE_NAME from GOODS_TYPE where type_id = goods_type), "
                 "GOODS_SPEC, "
                 "GOODS_MANUFACTURER, "
                 "GOODS_ORIGIN_PLACE, "
                 "GOODS_MAINSN, "
                 "(select UNIT_NAME from UNIT_INFO where UNIT_ID = GOODS_MAINUNIT),"
                 "GOODS_SUBSN, "
                 "(select UNIT_NAME from UNIT_INFO where UNIT_ID = GOODS_SUBUNIT),"
                 "GOODS_EXCHANGE_RATE FROM GOODS_INFO");
}
void ShopMainWindow::setInfoQuery(QString queryStr)
{
    m_queryModel->setQuery(queryStr, DataBaseImpl::connection());
    setQueryInfoHeader();
}

enum GOODS_STOCK_TABLE{
    TABLE_STOCK_ID,
    TABLE_STOCK_DATE,
    TABLE_STOCK_SN,
    TABLE_STOCK_NAME,
    TABLE_STOCK_NUMBER,
    TABLE_STOCK_UNIT_ID,
    TABLE_STOCK_PURCHASE_PRICE,
    TABLE_STOCK_SELL_PRICE
};
void ShopMainWindow::setQueryStockHeader()
{
    m_stockEditModel.setHeaderData(TABLE_STOCK_ID, Qt::Horizontal, tr("库存号"));
    m_stockEditModel.setHeaderData(TABLE_STOCK_DATE, Qt::Horizontal, tr("日期"));
    m_stockEditModel.setHeaderData(TABLE_STOCK_SN, Qt::Horizontal, tr("商品条码"));
    m_stockEditModel.setHeaderData(TABLE_STOCK_NAME, Qt::Horizontal, tr("名称"));
    m_stockEditModel.setHeaderData(TABLE_STOCK_NUMBER, Qt::Horizontal, tr("库存数量"));
    m_stockEditModel.setHeaderData(TABLE_STOCK_UNIT_ID, Qt::Horizontal, tr("单位"));
    m_stockEditModel.setHeaderData(TABLE_STOCK_PURCHASE_PRICE, Qt::Horizontal, tr("采购价"));
    m_stockEditModel.setHeaderData(TABLE_STOCK_SELL_PRICE, Qt::Horizontal, tr("销售价"));
    ui->tableView_queryView->setModel(&m_stockEditModel);
}

void ShopMainWindow::setStockQuery(QString querystr)
{
    m_stockEditModel.setMyQuery(querystr, DataBaseImpl::connection());
    setQueryStockHeader();
}
void ShopMainWindow::showStockTable()
{
    setStockQuery("SELECT STOCK_ID, STOCK_DATE, STOCK_SN,"
                  "(select GOODS_NAME from goods_info where goods_mainsn = stock_sn or goods_subsn = stock_sn),"
                  "STOCK_NUMBER, "
                  "(select unit_name from unit_info where unit_id = stock_unit_id),"
                  "STOCK_PURCHASE_PRICE, STOCK_SELL_PRICE "
                  "FROM GOODS_STOCK");
}

enum CREDIT_TABLE{
    TABLE_CREDIT_ID = 0,
    TABLE_CREDIT_DATE,
    TABLE_CREDIT_TIME,
    TABLE_CREDIT_SN,
    TABLE_CREDIT_NAME,
    TABLE_CREDIT_PURCHASE_PRICE,
    TABLE_CREDIT_PRICE,
    TABLE_CREDIT_NUMBER,
    TABLE_CREDIT_UNIT,
    TABLE_CREDIT_DISCOUNT,
    TABLE_CREDIT_TOTAL,
    TABLE_CREDIT_PROFIT,
    TABLE_CREDIT_MAX
};

void ShopMainWindow::setQueryCreditHeader()
{
    m_queryModel->setHeaderData(TABLE_CREDIT_ID, Qt::Horizontal, tr("赊账号"));
    m_queryModel->setHeaderData(TABLE_CREDIT_DATE, Qt::Horizontal, tr("日期"));
    m_queryModel->setHeaderData(TABLE_CREDIT_TIME, Qt::Horizontal, tr("时间"));
    m_queryModel->setHeaderData(TABLE_CREDIT_SN, Qt::Horizontal, tr("商品条码"));
    m_queryModel->setHeaderData(TABLE_CREDIT_NAME, Qt::Horizontal, tr("名称"));
    m_queryModel->setHeaderData(TABLE_CREDIT_PURCHASE_PRICE, Qt::Horizontal, tr("进价"));
    m_queryModel->setHeaderData(TABLE_CREDIT_PRICE, Qt::Horizontal, tr("售价"));
    m_queryModel->setHeaderData(TABLE_CREDIT_NUMBER, Qt::Horizontal, tr("数量"));
    m_queryModel->setHeaderData(TABLE_CREDIT_UNIT, Qt::Horizontal, tr("单位"));
    m_queryModel->setHeaderData(TABLE_CREDIT_DISCOUNT, Qt::Horizontal, tr("折扣率"));
    m_queryModel->setHeaderData(TABLE_CREDIT_TOTAL, Qt::Horizontal, tr("赊账金额"));
    m_queryModel->setHeaderData(TABLE_CREDIT_PROFIT, Qt::Horizontal, tr("应得利润"));
    ui->tableView_queryView->setModel(m_queryModel);
}

void ShopMainWindow::showCreditTable()
{
    setCreditQuery("SELECT credit_id, credit_date, credit_time, credit_sn, credit_name, "
                   " credit_purchase_price, credit_price, credit_number, credit_unit_name, "
                   " credit_discount, credit_total, credit_profit "
                   "from goods_oncredit");
}
void ShopMainWindow::setCreditQuery(QString querystr)
{
    m_queryModel->setQuery(querystr, DataBaseImpl::connection());
    setQueryCreditHeader();
}

enum SELL_TABLE{
    TABLE_SELL_ID = 0,
    TABLE_SELL_DATE,
    TABLE_SELL_TIME,
    TABLE_SELL_SN,
    TABLE_SELL_NAME,
    TABLE_SELL_PURCHASE_PRICE,
    TABLE_SELL_PRICE,
    TABLE_SELL_NUMBER,
    TABLE_SELL_UNIT,
    TABLE_SELL_DISCOUNT,
    TABLE_SELL_TOTAL,
    TABLE_SELL_PROFIT,
    TABLE_SELL_MAX
};

void ShopMainWindow::setQuerySellHeader()
{
    m_queryModel->setHeaderData(TABLE_SELL_ID, Qt::Horizontal, tr("销售单号"));
    m_queryModel->setHeaderData(TABLE_SELL_DATE, Qt::Horizontal, tr("日期"));
    m_queryModel->setHeaderData(TABLE_SELL_TIME, Qt::Horizontal, tr("时间"));
    m_queryModel->setHeaderData(TABLE_SELL_SN, Qt::Horizontal, tr("商品条码"));
    m_queryModel->setHeaderData(TABLE_SELL_NAME, Qt::Horizontal, tr("名称"));
    m_queryModel->setHeaderData(TABLE_SELL_PURCHASE_PRICE, Qt::Horizontal, tr("进价"));
    m_queryModel->setHeaderData(TABLE_SELL_PRICE, Qt::Horizontal, tr("售价"));
    m_queryModel->setHeaderData(TABLE_SELL_NUMBER, Qt::Horizontal, tr("数量"));
    m_queryModel->setHeaderData(TABLE_SELL_UNIT, Qt::Horizontal, tr("单位"));
    m_queryModel->setHeaderData(TABLE_SELL_DISCOUNT, Qt::Horizontal, tr("折扣率"));
    m_queryModel->setHeaderData(TABLE_SELL_TOTAL, Qt::Horizontal, tr("总金额"));
    m_queryModel->setHeaderData(TABLE_SELL_PROFIT, Qt::Horizontal, tr("利润"));
    ui->tableView_queryView->setModel(m_queryModel);
}

void ShopMainWindow::showSellTable()
{
    setSellQuery("SELECT sell_id, sell_date, sell_time, sell_sn, "
                 "(select goods_name from goods_info where goods_mainsn = sell_sn or goods_subsn = sell_sn),"
                 "(select stock_purchase_price from goods_stock where stock_sn = sell_sn), "
                 "sell_price, sell_number, "
                 "(select unit_name from unit_info where unit_id = sell_unit_id),"
                 "sell_discount_rate, sell_total, sell_profit "
                 " FROM GOODS_SELL");
}
void ShopMainWindow::setSellQuery(QString querystr)
{
    m_queryModel->setQuery(querystr, DataBaseImpl::connection());
    setQuerySellHeader();
}
enum PURCHASE_TABLE {
    TABLE_PURCHASE_ID,
    TABLE_PURCHASE_DATE,
    TABLE_PURCHASE_TIME,
    TABLE_PURCHASE_SN,
    TABLE_PURCHASE_NAME,
    TABLE_PURCHASE_UNIT_ID,
    TABLE_PURCHASE_SUPPLIER,
    TABLE_PURCHASE_PRICE,
    TABLE_PURCHASE_NUMBER,
    TABLE_PURCHASE_TOTAL,
    TABLE_PURCHASE_REMARK

};
void ShopMainWindow::setQueryPurchaseHeader()
{
    m_queryModel->setHeaderData(TABLE_PURCHASE_ID, Qt::Horizontal, tr("采购号"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_DATE, Qt::Horizontal, tr("日期"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_TIME, Qt::Horizontal, tr("时间"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_SN, Qt::Horizontal, tr("条码"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_NAME, Qt::Horizontal, tr("名称"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_UNIT_ID, Qt::Horizontal, tr("单位"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_SUPPLIER, Qt::Horizontal, tr("供应商"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_PRICE, Qt::Horizontal, tr("采购价"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_NUMBER, Qt::Horizontal, tr("采购数量"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_TOTAL, Qt::Horizontal, tr("采购总金额"));
    m_queryModel->setHeaderData(TABLE_PURCHASE_REMARK, Qt::Horizontal, tr("备注"));
    ui->tableView_queryView->setModel(m_queryModel);
}

void ShopMainWindow::setPurchaseQuery(QString querystr)
{
    m_queryModel->setQuery(querystr, DataBaseImpl::connection());
    setQueryPurchaseHeader();
}
void ShopMainWindow::showPurchaseTable()
{
    setPurchaseQuery("SELECT id, purchase_date, purchase_time, purchase_sn, "
                     "(select goods_name from goods_info where goods_mainsn = purchase_sn or goods_subsn = purchase_sn),"
                     "(select unit_name from unit_info where unit_id = purchase_unit_id),"
                     "purchase_supplier, purchase_price, purchase_number, purchase_total, purchase_remark "
                     "FROM GOODS_PURCHASE");
}

void ShopMainWindow::selectModelForQuery(int index)
{
    switch (index)
    {
    case QUERY_INFO:
        showInfoTable();
        hideBalanceButton(true);
        break;
    case QUERY_STOCK:
        hideBalanceButton(true);
        showStockTable();
        break;
    case QUERY_PURCHASE:
        hideBalanceButton(false);
        showPurchaseTable();
        break;
    case QUERY_SELL:
        hideBalanceButton(false);
        showSellTable();
        break;
    case QUERY_ONCREDIT:
        hideBalanceButton(false);
        showCreditTable();
        break;
    default:
        m_queryModel->removeRows(0, m_queryModel->rowCount());
        m_stockEditModel.removeRows(0, m_stockEditModel.rowCount());
        ui->tableView_queryView->setModel(NULL);
        hideBalanceButton(true);
        break;
    }
}

void ShopMainWindow::setStockTableHeader(void)
{
    QFont font = ui->tableView_stockEntry->horizontalHeader()->font();
    font.setBold(true);
    ui->tableView_stockEntry->horizontalHeader()->setFont(font);

    m_stockModel = new QStandardItemModel();

    m_stockModel->setColumnCount(STOCK_MAX_FIELD);
    m_stockModel->setHeaderData(STOCK_NAME, Qt::Horizontal, tr("名称"));
    m_stockModel->setHeaderData(STOCK_NUMBER, Qt::Horizontal, tr("大件库存"));
    m_stockModel->setHeaderData(STOCK_NUMBER1, Qt::Horizontal, tr("小件库存"));
    m_stockModel->setHeaderData(STOCK_ENTRY_NUMBER, Qt::Horizontal, tr("大件入库数"));
    m_stockModel->setHeaderData(STOCK_ENTRY_NUMBER1, Qt::Horizontal, tr("小件入库数"));
    m_stockModel->setHeaderData(STOCK_UNIT_NAME, Qt::Horizontal, tr("入库单位"));
    m_stockModel->setHeaderData(STOCK_PURCHASE_PRICE, Qt::Horizontal, tr("采购价"));
    m_stockModel->setHeaderData(STOCK_TOTAL, Qt::Horizontal, tr("采购总额"));
    m_stockModel->setHeaderData(STOCK_MAINSN, Qt::Horizontal, tr("大件条码"));
    m_stockModel->setHeaderData(STOCK_MAIN_PRICE, Qt::Horizontal, tr("大件售价"));
    m_stockModel->setHeaderData(STOCK_SUBSN, Qt::Horizontal, tr("小件条码"));
    m_stockModel->setHeaderData(STOCK_SUB_PRICE, Qt::Horizontal, tr("小件售价"));
    m_stockModel->setHeaderData(STOCK_SUPPLIER, Qt::Horizontal, tr("供应商"));
    m_stockModel->setHeaderData(STOCK_REMARK, Qt::Horizontal, tr("备注"));
    m_stockModel->setHeaderData(STOCK_HIDE_MAINUNIT, Qt::Horizontal, tr("大件单位ID"));
    m_stockModel->setHeaderData(STOCK_HIDE_SUBUNIT, Qt::Horizontal, tr("小件单位ID"));


    ui->tableView_stockEntry->setItemDelegateForColumn(STOCK_NAME, new readonlyItemDelegate(this));
    ui->tableView_stockEntry->setItemDelegateForColumn(STOCK_UNIT_NAME, new readonlyItemDelegate(this));
    ui->tableView_stockEntry->setItemDelegateForColumn(STOCK_MAINSN, new readonlyItemDelegate(this));
    ui->tableView_stockEntry->setItemDelegateForColumn(STOCK_SUBSN, new readonlyItemDelegate(this));
    ui->tableView_stockEntry->setShowGrid(true);
    ui->tableView_stockEntry->resizeColumnsToContents();
    ui->tableView_stockEntry->setModel(m_stockModel);
    ui->tableView_stockEntry->setSelectionMode(QTableView::SingleSelection);
    ui->tableView_stockEntry->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_stockEntry->hideColumn(STOCK_HIDE_MAINUNIT);
    ui->tableView_stockEntry->hideColumn(STOCK_HIDE_SUBUNIT);

    connect(m_stockModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(updateStockView(QStandardItem *)));
}

void ShopMainWindow::updateStockView(QStandardItem *item)
{
    QModelIndex index;
    int column = item->column();
    int row = item->row();

    switch(column)
    {
    case STOCK_ENTRY_NUMBER1:
    case STOCK_PURCHASE_PRICE:
    case STOCK_ENTRY_NUMBER:
    {
        /* 获取当前输入大件库存 */
        index = m_stockModel->index(row, STOCK_ENTRY_NUMBER);
        int stock_number = index.data().toInt();

        /* 获取当前采购价 */
        index = m_stockModel->index(row, STOCK_PURCHASE_PRICE);
        double purchase_price = index.data().toDouble();

        /* 获取大小条码 */
        index = m_stockModel->index(row, STOCK_MAINSN);
        QString mainsn = index.data().toString();
        index = m_stockModel->index(row, STOCK_SUBSN);
        QString subsn = index.data().toString();

        /* 更新总金额 */
        double total = stock_number * purchase_price;
        if ( mainsn != subsn )
        {
            /* 获取当前输入小件库存的金额 */
            index = m_stockModel->index(row, STOCK_ENTRY_NUMBER1);
            int substock_number = index.data().toInt();
            GoodsInfoModel info_model;
            int rate = info_model.getRateBySN(mainsn);
            total += (substock_number * purchase_price / rate);
        }

        index = m_stockModel->index(row, STOCK_TOTAL);
        m_stockModel->setData(index, QVariant(total));

        break;
    }
    default:
        break;
    }
}

void ShopMainWindow::fillInfoBySN(QString main_sn)
{
    ui->tabWidget->setCurrentIndex(TAB_INFO);
    ui->lEdit_goodsBigSN->setText(main_sn);
}

void ShopMainWindow::initEntryLineEditList(void)
{
    m_goodsEntryLineEdit.append(ui->lEdit_stock_mainSN);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_purchasePrice);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_mainPrice);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_leftNumber);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_subPrice);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_entryNumber);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_supplier);
    m_goodsEntryLineEdit.append(ui->lEdit_stockName);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_subSN);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_entryNumber1);
    m_goodsEntryLineEdit.append(ui->lEdit_stock_leftNumber1);
}

void ShopMainWindow::setPersonalTabOrder()
{
    ui->tab_personalInfo->setTabOrder(ui->lEdit_personal_userName, ui->lEdit_personal_newPasswd);
    ui->tab_personalInfo->setTabOrder(ui->lEdit_personal_newPasswd, ui->lEdit_personal_confirmPasswd);
}

int ShopMainWindow::askPasswd(void)
{
    DialogQueryPasswd query;
    if ( QDialog::Accepted != query.exec())
    {
        ui->tabWidget->setCurrentIndex(TAB_SELL);
    }
    else
    {
        UserInfoModel user_model;
        UserInfo *user = user_model.getUserInfoByID(m_userid);
        if ( NULL != user )
        {
            QString passwd = user->Password();
            QString name = user->Name();
            delete user;
            if ( query.getQueryPasswd() == passwd )
            {
                ui->lEdit_personal_userName->setText(name);
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    return 2;
}

void ShopMainWindow::wrongHint()
{
    showMessage("不好意思,密码错误!");
    ui->tabWidget->setCurrentIndex(TAB_SELL);
}

void ShopMainWindow::setTabFocus(int index)
{
    switch (index)
    {
    case TAB_ENTRY:
        ui->lEdit_stock_mainSN->setFocus();
        break;
    case TAB_SELL:
        ui->edit_input_sell_sn->setFocus();
        break;
    case TAB_INFO:
        ui->lEdit_goodsName->setFocus();
        break;
    case TAB_PERSONAL:
    {
        int ret = askPasswd();
        if ( 0 == ret)
        {
            ui->lEdit_personal_userName->setFocus();
        }
        else if ( 1 == ret )
        {
            emit wrongPasswd();
        }
        else
        {
            ui->tabWidget->setCurrentIndex(TAB_SELL);
        }
        break;
    }
    default:
        break;
    }

    return ;

}

void ShopMainWindow::setEntryTabOrder(void)
{
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_mainSN, ui->lEdit_stockName);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stockName, ui->lEdit_stock_subSN);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_subSN,  ui->lEdit_stock_purchasePrice);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_purchasePrice, ui->lEdit_stock_mainPrice);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_mainPrice, ui->lEdit_stock_subPrice);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_subPrice, ui->lEdit_stock_entryNumber);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_entryNumber, ui->lEdit_stock_entryNumber1);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_entryNumber1, ui->lEdit_stock_leftNumber);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_leftNumber, ui->lEdit_stock_leftNumber1);
    ui->tab_goodsEntry->setTabOrder(ui->lEdit_stock_leftNumber1, ui->lEdit_stock_supplier);
}

void ShopMainWindow::addGoodsStockBySN(QString sn)
{
    GoodsInfoModel info_model;
    GoodsInfo *info = info_model.getGoodsInfoByMainSN(sn);

    ui->tabWidget->setCurrentIndex(TAB_ENTRY);
    if ( info != NULL )
    {
        /* 填充入库表单 */
        ui->lEdit_stock_leftNumber->setText("0");
        ui->lEdit_stockName->setText(info->Name());
        ui->lEdit_stock_subSN->setText(info->SubSerialNum());
        ui->lEdit_stock_mainSN->setText(sn);
        ui->lEdit_stock_purchasePrice->setFocus();
        delete info;
    }
}

void ShopMainWindow::initInfoTabUI(void)
{
    /* 填充所有下拉框列表 */
    QStringList unit_list;
    QStringList type_list;
    UnitInfoModel unit_model;
    GoodsTypeModel type_model;

    type_list = type_model.getAllTypeNames();
    unit_list = unit_model.getAllUnitName();
    ui->cBox_goodsBigUnit->addItem(tr("请选择"));
    ui->cBox_goodsSmallUnit->addItem(tr("请选择"));
    ui->cBox_goodsType->addItem(tr("请选择"));
    ui->cBox_goodsType->addItems(type_list);
    ui->cBox_goodsBigUnit->addItems(unit_list);
    ui->cBox_goodsSmallUnit->addItems(unit_list);

    ui->cBox_goodsType->setCurrentIndex(GoodsType::TYPE_TOBACCO);
    ui->cBox_goodsBigUnit->setCurrentIndex(UnitInfo::UNIT_INFO_BUNDLE);
    ui->cBox_goodsSmallUnit->setCurrentIndex(UnitInfo::UNIT_INFO_BOX);

    /* 设置默认的转换率为10 */
    ui->lEdit_goodsBig2Small->setText(tr("10"));
}

void ShopMainWindow::addShortCurForInfoTab(void)
{
    ui->btn_goodsAddNewInfo->setShortcut(QKeySequence(tr("Ctrl+I")));
    ui->btn_goodsInfoReset->setShortcut(QKeySequence(tr("Ctrl+R")));
    ui->btn_goodsInfoSubmitAll->setShortcut(QKeySequence(tr("Ctrl+S")));
    ui->btn_goodsInfoDeleteSelected->setShortcut(QKeySequence(tr("Ctrl+D")));
}

void ShopMainWindow::addShortCutForEntryTab(void)
{
    ui->btn_stock_addNew->setShortcut(QKeySequence(tr("Ctrl+I")));
    ui->btn_stock_deleteSelected->setShortcut(QKeySequence(tr("Ctrl+D")));
    ui->btn_stock_reset->setShortcut(QKeySequence(tr("Ctrl+R")));
    ui->btn_stock_submit->setShortcut(QKeySequence(tr("Ctrl+S")));
}

void ShopMainWindow::setInfoTabOrder(void)
{
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_GOODS_NAME), ui->cBox_goodsType);
    ui->tab_goodsInfo->setTabOrder(ui->cBox_goodsType, infoLineEdit(INFO_GOODS_CODE));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_GOODS_CODE), infoLineEdit(INFO_BIG_SN));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_BIG_SN), infoLineEdit(INFO_SMALL_SN));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_SMALL_SN), infoLineEdit(INFO_MANUF));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_MANUF), ui->cBox_goodsBigUnit);
    ui->tab_goodsInfo->setTabOrder(ui->cBox_goodsBigUnit, ui->cBox_goodsSmallUnit);
    ui->tab_goodsInfo->setTabOrder(ui->cBox_goodsSmallUnit, infoLineEdit(INFO_ORIG_PLACE));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_ORIG_PLACE), infoLineEdit(INFO_BIG_SELL_PRICE));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_BIG_SELL_PRICE), infoLineEdit(INFO_SMALL_SELL_PRICE));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_SMALL_SELL_PRICE), infoLineEdit(INFO_BIG2SMALL));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_BIG2SMALL), infoLineEdit(INFO_BIG_PURCHASE_PRICE));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_BIG_PURCHASE_PRICE), infoLineEdit(INFO_STOCK_NUM));
    ui->tab_goodsInfo->setTabOrder(infoLineEdit(INFO_STOCK_NUM), ui->tEdit_goodsSpec);
}

void ShopMainWindow::setInfoTableHeader(void)
{
    QFont font = ui->tableView_goodsInfoView->horizontalHeader()->font();
    font.setBold(true);
    ui->tableView_goodsInfoView->horizontalHeader()->setFont(font);

    m_goodsInfoModel = new QStandardItemModel();

    m_goodsInfoModel->setColumnCount(GOODS_MAX_FIELD);
    m_goodsInfoModel->setHeaderData(GOODS_CODE, Qt::Horizontal, tr("商品代码"));
    m_goodsInfoModel->setHeaderData(GOODS_NAME, Qt::Horizontal, tr("名称"));
    m_goodsInfoModel->setHeaderData(GOODS_TYPE, Qt::Horizontal, tr("类型"));
    m_goodsInfoModel->setHeaderData(GOODS_SPEC, Qt::Horizontal, tr("规格"));
    m_goodsInfoModel->setHeaderData(GOODS_MANUFACTURER, Qt::Horizontal, tr("制造商"));
    m_goodsInfoModel->setHeaderData(GOODS_ORIGIN_PLACE, Qt::Horizontal, tr("原产地"));
    m_goodsInfoModel->setHeaderData(GOODS_PURCHASE_PRICE, Qt::Horizontal, tr("采购价"));
    m_goodsInfoModel->setHeaderData(GOODS_MAINSN, Qt::Horizontal, tr("大件条码"));
    m_goodsInfoModel->setHeaderData(GOODS_SELL_MAINPRICE, Qt::Horizontal, tr("大件售价"));
    m_goodsInfoModel->setHeaderData(GOODS_MAINUNIT, Qt::Horizontal, tr("大件单位"));
    m_goodsInfoModel->setHeaderData(GOODS_SUBSN, Qt::Horizontal, tr("小件条码"));
    m_goodsInfoModel->setHeaderData(GOODS_SELL_SUBPRICE, Qt::Horizontal, tr("小件售价"));
    m_goodsInfoModel->setHeaderData(GOODS_SUBUNIT, Qt::Horizontal, tr("小件单位"));
    m_goodsInfoModel->setHeaderData(GOODS_EXCHANGE_RATE, Qt::Horizontal, tr("转换数目"));
    m_goodsInfoModel->setHeaderData(GOODS_STOCK_NUMBER, Qt::Horizontal, tr("库存"));
    m_goodsInfoModel->setHeaderData(GOODS_HIDE_TYPE, Qt::Horizontal, tr("隐藏类型"));
    m_goodsInfoModel->setHeaderData(GOODS_HIDE_BUID, Qt::Horizontal, tr("隐藏大单位"));
    m_goodsInfoModel->setHeaderData(GOODS_HIDE_SUID, Qt::Horizontal, tr("小单位"));

    ui->tableView_goodsInfoView->setItemDelegateForColumn(GOODS_TYPE, new readonlyItemDelegate(this));
    ui->tableView_goodsInfoView->setItemDelegateForColumn(GOODS_MAINUNIT, new readonlyItemDelegate(this));
    ui->tableView_goodsInfoView->setItemDelegateForColumn(GOODS_SUBUNIT, new readonlyItemDelegate(this));
    ui->tableView_goodsInfoView->setShowGrid(true);
    ui->tableView_goodsInfoView->resizeColumnsToContents();
    ui->tableView_goodsInfoView->setModel(m_goodsInfoModel);
    ui->tableView_goodsInfoView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView_goodsInfoView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_goodsInfoView->hideColumn(GOODS_HIDE_TYPE);
    ui->tableView_goodsInfoView->hideColumn(GOODS_HIDE_BUID);
    ui->tableView_goodsInfoView->hideColumn(GOODS_HIDE_SUID);
}


void ShopMainWindow::initInfoLineEditList(void)
{
    m_goodsInfoLineEdit.append(ui->lEdit_goodsBigSN);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsSmallSN);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsStockNum);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsCode);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsBig2Small);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsBigPurchasePrice);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsBigSellPrice);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsManuf);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsOrigPlace);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsSmallSellPrice);
    m_goodsInfoLineEdit.append(ui->lEdit_goodsName);
}

QLineEdit* ShopMainWindow::infoLineEdit(int type)
{
    return m_goodsInfoLineEdit[type];
}

QLineEdit* ShopMainWindow::entryLineEdit(int type)
{
    return m_goodsEntryLineEdit[type];
}


void ShopMainWindow::login(int user_id)
{
    m_userid = user_id;
    this->show();
}

ShopMainWindow::~ShopMainWindow()
{
    delete m_sellModel;
    delete m_queryModel;
    delete m_goodsInfoModel;
    delete m_stockModel;
    delete ui;
    DataBaseImpl::close();
}

bool ShopMainWindow::checkBarcode(const QString &text)
{
    bool bIsValidate = false;
    int iTextLen = text.length();

    if ( iTextLen == 13 )
    {
        bIsValidate = true;
        for ( int i = 0; i < text.length(); i++ )
        {
            QChar c = text.data()[i];
            if ( ('0' > c ) || ('9' < c ) )
            {
                bIsValidate = false;
                break;
            }
        }
    }

    return bIsValidate;
}

void ShopMainWindow::showMessage(QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("温馨提示"));
    msgBox.setText(text);
    msgBox.exec();

    return ;
}

/* 添加商品到列表中去，这里不检查库存，能刷的就是有的 */
void ShopMainWindow::on_edit_input_sell_sn_returnPressed()
{
    QString sn = ui->edit_input_sell_sn->text();
    if ( true != checkBarcode(sn) )
    {
        showMessage(tr("条形码输入有误!"));
        return ;
    }

    /* 从信息库里查找此商品 */
    GoodsInfoModel info_model;
    GoodsInfo *info = NULL;
    info = info_model.getGoodsInfoBySN(sn);
    if ( NULL == info )/* 查询信息表 */
    {
        showMessage(tr("查无此商品!"));
        ui->edit_input_sell_sn->setText("");
        return ;
    }

    /* 先查库存表中是否有记录 */
    GoodsStockModel stock_model;
    GoodsStock *stock = stock_model.getStockBySN(sn);
    if ( NULL == stock )
    {
        showMessage(tr("库存中无此商品,请先入库!"));
        DeleteDialog dialog;
        dialog.setText(tr("是否现在入库?"));
        dialog.setWindowTitle(tr("温馨提示"));
        if ( QDialog::Accepted == dialog.exec() )
        {
            emit addGoodsStock(info->MainSerialNum());
        }

        ui->edit_input_sell_sn->setText("");
        ui->edit_input_sell_sn->setFocus();
        delete info;
        return ;
    }

    /* 库存中有，取出相关信息添加到销售表格中 */
    addGoodsToSellView(info, stock);

    delete info;
    delete stock;

    return ;
}

void ShopMainWindow::addGoodsToSellView(GoodsInfo *info, GoodsStock *stock)
{
    QModelIndex index;
    int rows = m_sellModel->rowCount();
    bool found = false;
    for ( int i = 0; i < rows; i++ )
    {
        /* 先查找model里是否已经存在此商品，是则增加商品的个数 */
        index = m_sellModel->index(i, GOODS_SELL_SN);
        QString sn = index.data().toString();
        if ( stock->SN() == sn )
        {
            found = true;
            /* 增加销售数量，这里会产生信号，自动改变总金额 */
            index = m_sellModel->index(i, GOODS_SELL_NUMBER);
            int number = index.data().toInt();
            number++;
            m_sellModel->setData(index, number);
            break;
        }
    }

    /* 若没找到，则新增一条 */
    if ( true != found )
    {
        QList <QStandardItem*> row;
        double sell_price = stock->SellPrice();
        double purchase_price = stock->PurchasePrice();
        double profit = sell_price - purchase_price;
        QStandardItem *item_sn    = new QStandardItem(stock->SN());
        QStandardItem *item_name  = new QStandardItem(info->Name());
        QStandardItem *item_purch_price = new QStandardItem(QString::number(purchase_price));
        QStandardItem *item_sell_price = new QStandardItem(QString::number(sell_price));
        QStandardItem *item_number = new QStandardItem("1");
        UnitInfoModel unit_model;
        QStandardItem *item_unit  = new QStandardItem(unit_model.getUnitNameByID(stock->UnitID()));
        QStandardItem *item_unit_id = new QStandardItem(QString::number(stock->UnitID()));
        QStandardItem *item_discount = new QStandardItem("1.0");
        QStandardItem *item_total       = new QStandardItem(QString::number(sell_price));
        QStandardItem *item_profit  = new QStandardItem(QString::number(profit));


        row << item_sn << item_name << item_purch_price << item_sell_price << item_number
            << item_unit << item_discount << item_total << item_profit << item_unit_id;
        m_sellModel->appendRow(row);
    }

    ui->edit_input_sell_sn->setText("");
    ui->edit_input_sell_sn->setFocus();

    return ;
}

void ShopMainWindow::on_btn_goodsInfoReset_clicked()
{
    for ( int i = 0; i < INFO_MAX_TYPE; i++ )
    {
        infoLineEdit(i)->setText("");
    }
    ui->tEdit_goodsSpec->setText("");
    ui->cBox_goodsBigUnit->setCurrentIndex(UnitInfo::UNIT_INFO_BUNDLE);
    ui->cBox_goodsSmallUnit->setCurrentIndex(UnitInfo::UNIT_INFO_BOX);
    ui->cBox_goodsType->setCurrentIndex(GoodsType::TYPE_TOBACCO);
    infoLineEdit(INFO_BIG2SMALL)->setText(tr("10"));
    infoLineEdit(INFO_GOODS_NAME)->setFocus();

    return ;
}

QLineEdit * ShopMainWindow::getCurrentLineEdit(int type)
{
    QLineEdit *lEdit = NULL;
    switch (ui->tabWidget->currentIndex())
    {
    case TAB_INFO:
        lEdit = infoLineEdit(type);
        break;
    case TAB_ENTRY:
        lEdit = entryLineEdit(type);
        break;
    default:
        break;
    }

    return lEdit;
}

bool ShopMainWindow::validateInfoLineEdit(CHECK_LINE_EDIT_S *checkList, int size, int &error_type)
{
    bool isCheckOK = true;
    int type;
    error_type = -1;
    for ( int i = 0; i < size; i++ )
    {
        bool checkPass = true;
        type = checkList[i].editType;
        QLineEdit *lineEdit = getCurrentLineEdit(type);
        switch (checkList[i].checkType)
        {
            case CHECK_INT:
            {
                lineEdit->text().toInt(&checkPass);
                break;
            }
            case CHECK_DOUBLE:
            {
                lineEdit->text().toDouble(&checkPass);
                break;
            }
            case CHECK_STR:
            {
                if ( 0 == lineEdit->text().length() )
                {
                    checkPass = false;
                }
                break;
            }
            case CHECK_SN:
            {
                checkPass = checkBarcode(lineEdit->text());
                break;
            }
            case CHECK_SUBSN:
            {
                checkPass = checkSubCode(lineEdit->text());
                break;
            }
            case CHECK_SUBDOUBLE:
            {
                QString mainsn = getCurrentMainSN();
                QString subsn  = getCurrentSubSN();
                if ( mainsn != subsn )
                {
                    lineEdit->text().toDouble(&checkPass);
                }
                break;
            }
            case CHECK_SUBINT:
            {
                QString mainsn = getCurrentMainSN();
                QString subsn = getCurrentSubSN();
                if ( mainsn != subsn )
                {
                    lineEdit->text().toInt(&checkPass);
                }
                break;
            }
            default:
                break;
        }

        if ( false == checkPass )
        {
            isCheckOK = false;
            lineEdit->setStyleSheet("background: yellow");
            checkList[i].checkFailed = true;
            if ( error_type == -1 )
            {
                error_type = checkList[i].editType;
            }
        }
    }

    if ( false == isCheckOK )
    {
        QTimer::singleShot(3000, this, SLOT(clearHint()));
    }

    return isCheckOK;
}

QString ShopMainWindow::getCurrentSubSN()
{
    QString subsn = "";
    int index = ui->tabWidget->currentIndex();
    if ( TAB_INFO == index )
    {
        subsn = infoLineEdit(INFO_SMALL_SN)->text();
    }
    else if ( TAB_ENTRY == index )
    {
        subsn = entryLineEdit(ENTRY_SUBSN)->text();
    }

    return subsn;
}

QString ShopMainWindow::getCurrentMainSN()
{
    QString mainsn = "";
    int index = ui->tabWidget->currentIndex();
    if ( index == TAB_INFO )
    {
        mainsn = infoLineEdit(INFO_BIG_SN)->text();
    }
    else if ( index == TAB_ENTRY )
    {
        mainsn = entryLineEdit(ENTRY_MAIN_SN)->text();
    }

    return mainsn;
}

bool ShopMainWindow::checkSubCode(QString subsn)
{
    QString mainsn = getCurrentMainSN();
    bool checkPass = checkBarcode(mainsn);
    if ( (checkPass == true) && (mainsn != subsn) )
    {
        checkPass = checkBarcode(subsn);
    }

    return checkPass;
}

void ShopMainWindow::clearHint(void)
{
    int currentIndex = ui->tabWidget->currentIndex();

    switch(currentIndex)
    {
    case TAB_INFO:
    {
        int size = sizeof(g_InfoCheckList)/sizeof(g_InfoCheckList[0]);

        for (int i = 0; i < size; i++)
        {
            if ( g_InfoCheckList[i].checkFailed == true )
            {
                infoLineEdit(g_InfoCheckList[i].editType)->setStyleSheet( tr("QLineEdit{ font: 15px; background-color: white}"));
                g_InfoCheckList[i].checkFailed = false;
            }
        }
        break;
    }
    case TAB_ENTRY:
    {
        int size = sizeof(g_EntryCheckList)/sizeof(g_EntryCheckList[0]);
        for (int i = 0; i < size; i++)
        {
            if ( g_EntryCheckList[i].checkFailed == true )
            {
                entryLineEdit(g_EntryCheckList[i].editType)->setStyleSheet( tr("QLineEdit{ font: 15px; background-color: white}"));
                g_EntryCheckList[i].checkFailed = false;
            }
        }
        break;
    }
    default:
        break;
    }

    return ;
}
  /* 1 商品代码 */
  /* 2 商品名称 */
  /* 3 商品类型 */
  /* 4 商品规格 */
  /* 5 商品制造商 */
  /* 6 商品原产地*/
  /* 7 商品进价 */
  /* 8 商品大件条码 */
  /* 9 大件商品售价 */
  /* 10 商品大件计量单位 */
  /* 11 商品小件条码 */
  /* 12 小件商品售价 */
  /* 13 商品小件单位 */
  /* 14 大件转小件的汇率 B = Rate * S */
  /* 15 库存数量 */
void ShopMainWindow::on_btn_goodsAddNewInfo_clicked()
{
    int edit_type = INFO_GOODS_NAME;
    if ( true != validateInfoLineEdit(g_InfoCheckList, sizeof(g_InfoCheckList)/sizeof(g_InfoCheckList[0]), edit_type))
    {
        showMessage(tr("输入有误，请检查!"));
        infoLineEdit(edit_type)->setFocus();
        return ;
    }

    GoodsInfoModel info_model;
    if ( true != info_model.hasGoods(infoLineEdit(INFO_BIG_SN)->text()))
    {
        QList <QStandardItem*> row;
        QStandardItem *item_goods_code     = new QStandardItem(infoLineEdit(INFO_GOODS_CODE)->text());
        QStandardItem *item_goods_name     = new QStandardItem(infoLineEdit(INFO_GOODS_NAME)->text());
        QStandardItem *item_goods_type     = new QStandardItem(ui->cBox_goodsType->currentText());
        QStandardItem *item_goods_spec     = new QStandardItem(ui->tEdit_goodsSpec->toPlainText());
        QStandardItem *item_goods_manu     = new QStandardItem(infoLineEdit(INFO_MANUF)->text());
        QStandardItem *item_goods_orig     = new QStandardItem(infoLineEdit(INFO_ORIG_PLACE)->text());
        QStandardItem *item_purchase_price = new QStandardItem( infoLineEdit(INFO_BIG_PURCHASE_PRICE)->text());
        QStandardItem *item_goods_bigsn    = new QStandardItem(infoLineEdit(INFO_BIG_SN)->text());
        QStandardItem *item_big_price      = new QStandardItem(infoLineEdit(INFO_BIG_SELL_PRICE)->text());
        QStandardItem *item_big_unit       = new QStandardItem(ui->cBox_goodsBigUnit->currentText());
        QStandardItem *item_goods_smsn     = new QStandardItem(infoLineEdit(INFO_SMALL_SN)->text());
        QStandardItem *item_small_price    = new QStandardItem(infoLineEdit(INFO_SMALL_SELL_PRICE)->text());
        QStandardItem *item_small_unit     = new QStandardItem(ui->cBox_goodsSmallUnit->currentText());
        QStandardItem *item_exchange_rate  = new QStandardItem(infoLineEdit(INFO_BIG2SMALL)->text());
        QStandardItem *item_stock_num      = new QStandardItem(infoLineEdit(INFO_STOCK_NUM)->text());
        QStandardItem *item_hide_type      = new QStandardItem(QString::number(ui->cBox_goodsType->currentIndex()));
        QStandardItem *item_hide_bunit_id  = new QStandardItem(QString::number(ui->cBox_goodsBigUnit->currentIndex()));
        QStandardItem *item_hide_sunit_id  = new QStandardItem(QString::number(ui->cBox_goodsSmallUnit->currentIndex()));

        row << item_goods_code << item_goods_name << item_goods_type << item_goods_spec
            << item_goods_manu << item_goods_orig << item_purchase_price << item_goods_bigsn
            << item_big_price << item_big_unit << item_goods_smsn << item_small_price << item_small_unit
            << item_exchange_rate << item_stock_num << item_hide_type << item_hide_bunit_id << item_hide_sunit_id;
        m_goodsInfoModel->appendRow(row);
    }
    else
    {
        showMessage(tr("商品已存在, 请重新输入!"));
    }
    on_btn_goodsInfoReset_clicked();

    return;
}

void ShopMainWindow::on_btn_goodsInfoSubmitAll_clicked()
{
    /* 使用批量插入到信息表和库存表中 */
    QVector<GoodsInfo *> info_list;
    QVector<GoodsStock *> stock_list;

    int rows = m_goodsInfoModel->rowCount();
    for ( int i = 0; i < rows; i++ )
    {
        QString name = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_NAME)).toString();
        QString spec = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_SPEC)).toString();
        QString code = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_CODE)).toString();
        QString mainsn = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_MAINSN)).toString();
        QString subsn = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_SUBSN)).toString();
        QString manuf = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_MANUFACTURER)).toString();
        QString orig = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_ORIGIN_PLACE)).toString();
        double main_price = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_SELL_MAINPRICE)).toDouble();
        double sub_price = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_SELL_SUBPRICE)).toDouble();
        double purchase_price = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_PURCHASE_PRICE)).toDouble();
        int rate = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_EXCHANGE_RATE)).toInt();
        int type = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_HIDE_TYPE)).toInt();
        int buid = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_HIDE_BUID)).toInt();
        int suid =  m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_HIDE_SUID)).toInt();
        int stock_num = m_goodsInfoModel->data(m_goodsInfoModel->index(i, GOODS_STOCK_NUMBER)).toInt();

        GoodsInfo *info = new GoodsInfo();
        info->Type(type);
        info->Spec(spec);
        info->Code(code);
        info->Name(name);
        info->ExchangeRate(rate);
        info->MainSerialNum(mainsn);
        info->SubSerialNum(subsn);
        info->Manufacturer(manuf);
        info->OriginPlace(orig);
        info->MainUnit(buid);
        info->SubUnit(suid);
        info_list.append(info);

        GoodsStock *main_stock = new GoodsStock();
        main_stock->Date(QDate::currentDate());
        main_stock->Number(stock_num);
        main_stock->SellPrice(main_price);
        main_stock->PurchasePrice(purchase_price);
        main_stock->setSmall("N");
        main_stock->UnitID(buid);
        main_stock->SN(mainsn);
        stock_list.append(main_stock);

        if ( subsn != mainsn )
        {
            GoodsStock *sub_stock = new GoodsStock();
            sub_stock->Number(0);
            sub_stock->SellPrice(sub_price);
            sub_stock->PurchasePrice(purchase_price/rate);
            sub_stock->setSmall("Y");
            sub_stock->UnitID(suid);
            sub_stock->SN(subsn);
            sub_stock->Date(QDate::currentDate());
            stock_list.append(sub_stock);
        }
    }

    if ( rows > 0 )
    {
        GoodsInfoModel inf_model;
        GoodsStockModel stock_model;
        (void)inf_model.batchInsertGoodsInfo(info_list);
        (void)stock_model.batchInsertGoodsStock(stock_list);
        showMessage(tr("添加成功!"));
        /* 清空表格，重置输入框 */
        m_goodsInfoModel->removeRows(0, rows);

        for ( int i = 0; i < info_list.size(); i++ )
        {
            delete info_list[i];
        }

        for ( int i = 0; i < stock_list.size(); i++ )
        {
            delete stock_list[i];
        }
    }
    else
    {
        showMessage(tr("请添加信息!"));
    }
    on_btn_goodsInfoReset_clicked();

    return ;
}

void ShopMainWindow::on_pushButton_2_clicked()
{
    ui->tabWidget->setCurrentIndex(TAB_SELL);
    ui->edit_input_sell_sn->setFocus();
}

void ShopMainWindow::on_pushButton_5_clicked()
{
    ui->tabWidget->setCurrentIndex(TAB_ENTRY);
    ui->lEdit_stock_mainSN->setFocus();
}

void ShopMainWindow::on_pushButton_4_clicked()
{
    ui->tabWidget->setCurrentIndex(TAB_INFO);
    ui->lEdit_goodsName->setFocus();
}

void ShopMainWindow::on_pushButton_3_clicked()
{
    ui->tabWidget->setCurrentIndex(TAB_QUERY);
    ui->comboBox_query_table->setFocus();
}

void ShopMainWindow::on_pushButton_clicked()
{
    ui->lEdit_personal_confirmPasswd->setText("");
    ui->lEdit_personal_newPasswd->setText("");
    ui->lEdit_personal_userName->setText("");
    ui->lEdit_personal_userName->setFocus();
}

void ShopMainWindow::on_btn_personal_submit_clicked()
{
    QString new_passwd;
    QString confirm_passwd;

    new_passwd = ui->lEdit_personal_newPasswd->text();
    confirm_passwd = ui->lEdit_personal_confirmPasswd->text();
    UserInfo *user;
    UserInfoModel user_model;
    user = user_model.getUserInfoByID(m_userid);
    /* 检查两次密码是否输入一致 */
    if ((user->Password() != new_passwd ) && ( new_passwd == confirm_passwd ) && ( new_passwd.length() > 0 ) )
    {
        (void)user_model.updatePasswdByID(m_userid, new_passwd);
    }
    else if ( new_passwd != confirm_passwd )
    {
        showMessage(tr("密码不一致"));
        delete user;
        return ;
    }

    QString user_name = ui->lEdit_personal_userName->text();
    if ( (user_name != user->Name()) && (user_name.length() > 0) )
    {
        (void)user_model.updateUserNameByID(m_userid, user_name);
    }

    delete user;
    showMessage(tr("更新成功!"));
    on_pushButton_clicked();

    return;
}

void ShopMainWindow::on_btn_stock_reset_clicked()
{
    for (int i = 0; i < ENTRY_MAX; i++ )
    {
        entryLineEdit(i)->setText("");
    }
    ui->tEdit_remark->setText("");
    entryLineEdit(ENTRY_MAIN_SN)->setFocus();
}

void ShopMainWindow::on_lEdit_stock_mainSN_returnPressed()
{
    QString main_sn = entryLineEdit(ENTRY_MAIN_SN)->text();
    on_lEdit_stock_mainSN_textChanged(main_sn);
    entryLineEdit(ENTRY_PURCHASE_PRICE)->setFocus();
    return ;
}

bool ShopMainWindow::checkModelHasStock(QString sn)
{
    bool found = false;
    int rows = m_stockModel->rowCount();
    for ( int i = 0; i < rows; i++ )
    {
        QString sn_in_model = m_stockModel->data(m_stockModel->index(i, STOCK_MAINSN)).toString();
        if ( sn == sn_in_model )
        {
            found = true;
            break;
        }
    }

    return found;
}

void ShopMainWindow::on_btn_stock_addNew_clicked()
{
    int edit_type = ENTRY_MAIN_SN;
    /* 检查输入是否有误 */
    if ( true != validateInfoLineEdit(g_EntryCheckList, sizeof(g_EntryCheckList)/sizeof(g_EntryCheckList[0]), edit_type))
    {
        showMessage(tr("输入有误，请检查!"));
        entryLineEdit(edit_type)->setFocus();
        return ;
    }

    /* 检查model中是否已经存在更新的库存，有则直接提示已经存在 */
    if ( true == checkModelHasStock(entryLineEdit(ENTRY_MAIN_SN)->text()))
    {
        showMessage(tr("新增库存已存在列表中，请直接双击列表修改!"));
        return ;
    }
    /* 添加到stock model中 */
    UnitInfoModel unit_model;
    GoodsInfoModel info_model;
    QString mainsn = entryLineEdit(ENTRY_MAIN_SN)->text();
    QString subsn = entryLineEdit(ENTRY_SUBSN)->text();
    int main_unit = info_model.getMainUnitBySN(mainsn);
    int sub_unit = info_model.getSubUnitBySN(subsn);
    int entry_num = entryLineEdit(ENTRY_ENTRY_NUMBER)->text().toInt();
    double purchase_price = entryLineEdit(ENTRY_PURCHASE_PRICE)->text().toDouble();
    double total = entry_num * purchase_price;
    if ( subsn != mainsn )
    {
        int rate = info_model.getRateBySN(mainsn);
        int sub_entry_num = entryLineEdit(ENTRY_ENTRY_NUMBER1)->text().toInt();
        double total1 = sub_entry_num * purchase_price / rate;
        total += total1;
    }
    QString main_unit_name = unit_model.getUnitNameByID(main_unit);
    QList <QStandardItem*> row;
    QStandardItem *item_stock_name  = new QStandardItem(entryLineEdit(ENTRY_NAME)->text());
    QStandardItem *item_stock_num   = new QStandardItem(entryLineEdit(ENTRY_STOCK_NUMBER)->text());
    QStandardItem *item_entry_num   = new QStandardItem(QString::number(entry_num));
    QStandardItem *item_stock_unit  = new QStandardItem(main_unit_name);
    QStandardItem *item_purch_price = new QStandardItem(QString::number(purchase_price));
    QStandardItem *item_total       = new QStandardItem(QString::number(total));
    QStandardItem *item_mainsn      = new QStandardItem(mainsn);
    QStandardItem *item_main_price  = new QStandardItem(entryLineEdit(ENTRY_MAIN_PRICE)->text());
    QStandardItem *item_subsn       = new QStandardItem(subsn);
    QStandardItem *item_sub_price   = new QStandardItem(entryLineEdit(ENTRY_SUB_PRICE)->text());
    QStandardItem *item_supplier    = new QStandardItem(entryLineEdit(ENTRY_SUPPLIER)->text());
    QStandardItem *item_remark      = new QStandardItem(ui->tEdit_remark->toPlainText());
    QStandardItem *item_main_unit   = new QStandardItem(QString::number(main_unit));
    QStandardItem *item_sub_unit    = new QStandardItem(QString::number(sub_unit));
    QStandardItem *item_sub_entry_num = new QStandardItem(entryLineEdit(ENTRY_ENTRY_NUMBER1)->text());
    QStandardItem *item_sub_stock_num = new QStandardItem(entryLineEdit(ENTRY_STOCK_NUMBER1)->text());

    row << item_stock_name << item_stock_num << item_sub_stock_num << item_entry_num
        << item_sub_entry_num << item_stock_unit << item_purch_price
        << item_total << item_mainsn << item_main_price << item_subsn << item_sub_price
        << item_supplier << item_remark << item_main_unit << item_sub_unit;
    m_stockModel->appendRow(row);
    on_btn_stock_reset_clicked();
}

void ShopMainWindow::on_lEdit_stock_mainSN_textChanged(const QString &arg1)
{
    if ( true != checkBarcode(arg1))
    {
        return ;
    }

    /* 查询信息库填充商品名称和小件条码 */
    GoodsInfoModel goods_info_model;
    GoodsInfo *goods_info = goods_info_model.getGoodsInfoByMainSN(arg1);

    if ( NULL != goods_info )
    {
        entryLineEdit(ENTRY_NAME)->setText(goods_info->Name());
        entryLineEdit(ENTRY_SUBSN)->setText(goods_info->SubSerialNum());
        GoodsStockModel stock_model;
        GoodsStock *stock_info = stock_model.getStockBySN(arg1);
        GoodsStock *sub_stock = stock_model.getStockBySN(goods_info->SubSerialNum());
        if ( NULL != sub_stock )
        {
            entryLineEdit(ENTRY_SUB_PRICE)->setText(QString::number(sub_stock->SellPrice()));
            delete sub_stock;
        }
        if ( NULL != stock_info )
        {
            entryLineEdit(ENTRY_PURCHASE_PRICE)->setText(QString::number(stock_info->PurchasePrice()));
            entryLineEdit(ENTRY_MAIN_PRICE)->setText(QString::number(stock_info->SellPrice()));
            entryLineEdit(ENTRY_STOCK_NUMBER)->setText(QString::number(stock_info->Number()));
            delete stock_info;
        }
        delete goods_info;
    }
    else
    {
        showMessage(tr("无此商品,请增加商品信息"));
        DeleteDialog dialog;
        dialog.setText(tr("是否增加商品信息?"));
        dialog.setWindowTitle(tr("温馨提示"));
        if ( QDialog::Accepted == dialog.exec() )
        {
            emit fillInfo(arg1);
        }
        entryLineEdit(ENTRY_MAIN_SN)->setFocus();
        return ;
    }

    return ;
}

void ShopMainWindow::on_btn_stock_deleteSelected_clicked()
{
    QModelIndex list = ui->tableView_stockEntry->currentIndex();

    m_stockModel->removeRow(list.row());

    entryLineEdit(ENTRY_MAIN_SN)->setFocus();
}

void ShopMainWindow::on_btn_goodsInfoDeleteSelected_clicked()
{
    QModelIndex list = ui->tableView_goodsInfoView->currentIndex();

    m_goodsInfoModel->removeRow(list.row());

    ui->lEdit_goodsName->setFocus();
}

void ShopMainWindow::on_btn_stock_submit_clicked()
{
    int rows = m_stockModel->rowCount();
    if ( 0 == rows )
    {
        showMessage(tr("请添加入库商品"));
        entryLineEdit(ENTRY_MAIN_SN)->setFocus();
        return ;
    }

    GoodsStockModel stock_model;
    GoodsInfoModel  info_model;
    QVector<GoodsStock *> new_stock_list;
    QVector<GoodsStock *> update_stock_list;
    QVector<PurchaseInfo *> purchase_list;
    bool hasNewStock = false;
    /* 遍历整个表，添加大件入库和小件入库 */
    for (int i = 0; i < rows; i++ )
    {
        QString SN = m_stockModel->data(m_stockModel->index(i, STOCK_MAINSN)).toString();
        QString SSN = m_stockModel->data(m_stockModel->index(i, STOCK_SUBSN)).toString();
        QString remark = m_stockModel->data(m_stockModel->index(i, STOCK_REMARK)).toString();
        QString supplier = m_stockModel->data(m_stockModel->index(i, STOCK_SUPPLIER)).toString();
        int entry_num = m_stockModel->data(m_stockModel->index(i, STOCK_ENTRY_NUMBER)).toInt();
        int sub_entry_num = m_stockModel->data(m_stockModel->index(i, STOCK_ENTRY_NUMBER1)).toInt();
        int main_unit = m_stockModel->data(m_stockModel->index(i, STOCK_HIDE_MAINUNIT)).toInt();
        int sub_unit  = m_stockModel->data(m_stockModel->index(i, STOCK_HIDE_SUBUNIT)).toInt();
        int stock_num = m_stockModel->data(m_stockModel->index(i, STOCK_NUMBER)).toInt();
        int substock_num = m_stockModel->data(m_stockModel->index(i, STOCK_NUMBER1)).toInt();
        double purchase_price = m_stockModel->data(m_stockModel->index(i, STOCK_PURCHASE_PRICE)).toDouble();
        double main_price = m_stockModel->data(m_stockModel->index(i, STOCK_MAIN_PRICE)).toDouble();
        double sub_price = m_stockModel->data(m_stockModel->index(i, STOCK_SUB_PRICE)).toDouble();
        /* 如果大件SN对应的小件SN不等于输入的小件SN，则进行更新 */
        GoodsInfo *info =  info_model.getGoodsInfoByMainSN(SN);
        QString current_subsn = info->SubSerialNum();
        if ( SSN != current_subsn )
        {
            /* 删除原来库存中小件SN */
            stock_model.updateStockSNBySN(current_subsn, SSN);
            /* 更新信息表中的小件SN */
            info_model.updateSubSNByID(SSN, info->ID());
        }
        QString current_name = info->Name();
        QString input_name = m_stockModel->data(m_stockModel->index(i, STOCK_NAME)).toString();
        if ( current_name != input_name )
        {
            /* 更新信息表中对应SN的名字 */
            info_model.updateGoodsNameByID(input_name, info->ID());
        }
        int rate = info->ExchangeRate();
        delete info;
        /* 新增大小件库存信息 */
        GoodsStock *main_stock = new GoodsStock();
        main_stock->Date(QDate::currentDate());
        main_stock->setSmall("N");
        main_stock->SN(SN);
        main_stock->PurchasePrice(purchase_price);
        main_stock->SellPrice(main_price);
        main_stock->Number(stock_num+entry_num);
        main_stock->UnitID(main_unit);
        if ( true != stock_model.hasGoods(SN) )
        {
            hasNewStock = true;
            new_stock_list.append(main_stock);
        }
        else
        {
            update_stock_list.append(main_stock);
        }
        if ( SSN != SN )
        {
            qDebug("need update ssn\r\n");
            GoodsStock *sub_stock  = new GoodsStock();
            sub_stock->Date(QDate::currentDate());
            sub_stock->UnitID(sub_unit);
            sub_stock->setSmall("Y");
            sub_stock->SN(SSN);
            sub_stock->PurchasePrice(purchase_price/rate);
            sub_stock->SellPrice(sub_price);
            sub_stock->Number(substock_num+sub_entry_num);
            if ( true != stock_model.hasGoods(SSN) )
            {
                qDebug("need insert ssn\r\n");
                hasNewStock = true;
                new_stock_list.append(sub_stock);
            }
            else
            {
                update_stock_list.append(sub_stock);
            }
        }

        /* 新增采购信息 */
        if ( entry_num > 0 )
        {
            PurchaseInfo *purchase_info = new PurchaseInfo();
            purchase_info->Date(QDate::currentDate());
            purchase_info->Time(QTime::currentTime());
            purchase_info->SN(SN);
            purchase_info->Remark(remark);
            purchase_info->Number(entry_num);
            purchase_info->Price(purchase_price);
            purchase_info->Total(entry_num * purchase_price);
            purchase_info->Supplier(supplier);
            purchase_info->UnitID(main_unit);
            purchase_list.append(purchase_info);
        }
        if ( sub_entry_num > 0 )
        {
            PurchaseInfo *sub_purchase_info = new PurchaseInfo();
            sub_purchase_info->Date(QDate::currentDate());
            sub_purchase_info->Time(QTime::currentTime());
            sub_purchase_info->SN(SSN);
            sub_purchase_info->Remark(remark);
            sub_purchase_info->Number(sub_entry_num);
            sub_purchase_info->Price(purchase_price);
            sub_purchase_info->Total(sub_entry_num * purchase_price / rate);
            sub_purchase_info->Supplier(supplier);
            sub_purchase_info->UnitID(sub_unit);
            purchase_list.append(sub_purchase_info);
        }
    }

    /* 新的库存信息插入 */
    if ( true == hasNewStock )
    {
        stock_model.batchInsertGoodsStock(new_stock_list);
        freeStockList(new_stock_list);
    }

    /* 更新库存 */
    stock_model.batchUpdateGoodsStock(update_stock_list);
    freeStockList(update_stock_list);

    /* 插入采购表 */
    PurchaseInfoModel purchase_model;
    (void)purchase_model.batchInsertPurchaseInfo(purchase_list);
    freePurchaseList(purchase_list);

    m_stockModel->removeRows(0, m_stockModel->rowCount());
    on_btn_goodsInfoReset_clicked();
    showMessage(tr("更新库存信息成功!"));
}

void ShopMainWindow::freeStockList(QVector<GoodsStock *> list)
{
    int size = list.size();
    for ( int i = 0; i < size; i++ )
    {
        delete list[i];
    }
    return ;
}

void ShopMainWindow::freePurchaseList(QVector<PurchaseInfo *> list)
{
    int size = list.size();
    for ( int i = 0; i < size; i++ )
    {
        delete list[i];
    }
    return ;
}

void ShopMainWindow::freeSellList(QVector<GoodsSell *> list)
{
    int size = list.size();
    for ( int i = 0; i < size; i++ )
    {
        delete list[i];
    }
    return ;
}

void ShopMainWindow::WriteModelDataToFile(QString name, QSqlQueryModel* model)
{
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       return;
    }

    QTextStream out(&file);
    QString headData;
    int rowCount =  model->rowCount();
    int columnCount =  model->columnCount();

    out.setCodec("GBK");
    for ( int index = 0; index < columnCount; index++)
    {
        headData += model->headerData(index,  Qt::Horizontal).toString() + ",";
    }
    out << headData << "\n";
    for ( int i = 0; i < rowCount; i++ )
    {
       QModelIndex index;
       for ( int j = 0; j < columnCount; j++ )
       {
           index =  model->index(i, j);
           out <<  model->data(index).toString().replace("\n", " ") << ",";
       }
       out << "\n";
    }
    out.flush();
    file.close();
}

void ShopMainWindow::exportFile(QString name)
{
    if ( COMBOX_ITEM_STOCK != ui->comboBox_query_table->currentIndex() )
    {
        WriteModelDataToFile(name, m_queryModel);
    }
    else
    {
        WriteModelDataToFile(name, &m_stockEditModel);
    }
}

QString ShopMainWindow::showSaveFileDialog(QString defaultName)
{
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    QString saveFile = dialog.getSaveFileName(this, tr("Save File"),
                                             defaultName,
                                             tr("电子表格 (*.csv)"));
    qDebug() << "save file" << saveFile;
    return saveFile;
}

void ShopMainWindow::on_pushButton_8_clicked()
{
    QDate current = QDate::currentDate();
    int year = current.year();
    int month = current.month();
    int day = current.day();
    QString defaultName = tr("未命名-%1-%2-%3.csv").arg(QString::number(year), QString::number(month), QString::number(day));
    QString saveFile = showSaveFileDialog(defaultName);
    if ( saveFile != "" )
    {
        exportFile(saveFile);
    }
}

void ShopMainWindow::on_btn_query_deleteSelected_clicked()
{
    QModelIndex list = ui->tableView_queryView->currentIndex();
    if ( -1 == list.row() )
    {
        return ;
    }

    DeleteDialog dialog;
    if ( QDialog::Accepted != dialog.exec() )
    {
        return ;
    }

    int index = ui->comboBox_query_table->currentIndex();
    switch (index)
    {
        case COMBOX_ITEM_INFO:
        {
            GoodsInfoModel info_model;
            int id = m_queryModel->data(m_queryModel->index(list.row(), TABLE_GOODS_ID)).toInt();
            info_model.deleteGoodsByID(id);
            showInfoTable();
            break;
        }
        case COMBOX_ITEM_PURCHASE:
        {
            int id = m_queryModel->data(m_queryModel->index(list.row(), TABLE_PURCHASE_ID)).toInt();
            PurchaseInfoModel purchase_model;
            purchase_model.deletePurchaseByID(id);
            showPurchaseTable();
            break;
        }
        case COMBOX_ITEM_SELL:
        {
            int id = m_queryModel->data(m_queryModel->index(list.row(), TABLE_SELL_ID)).toInt();
            GoodsSellModel sell_model;
            sell_model.deleteSellByID(id);
            showSellTable();
            break;
        }
        case COMBOX_ITEM_STOCK:
        {
            GoodsStockModel stock_model;
            int id = m_stockEditModel.data(m_stockEditModel.index(list.row(), TABLE_STOCK_ID)).toInt();
            stock_model.deleteStockByID(id);
            showStockTable();
            break;
        }
        case COMBOX_ITEM_ON_CREDIT:
        {
            /* TODO: 赊账处理，还账处理, 插入到销售列表中，从赊账列表删除 */
           // GoodsCreditModel credit_model;

            showCreditTable();
        }
        default:
            break;
    }
    return ;
}

void ShopMainWindow::on_pushButton_10_clicked()
{
    QModelIndex list = ui->tableView_sellView->currentIndex();

    m_sellModel->removeRow(list.row());

    ui->edit_input_sell_sn->setFocus();
}

void ShopMainWindow::on_pushButton_7_clicked()
{
    m_sellModel->removeRows(0, m_sellModel->rowCount());
    ui->edit_input_sell_sn->setText("");
    ui->edit_input_sell_sn->setFocus();
}

void ShopMainWindow::updateStockTableBySellList(QVector<GoodsSell *> sell_list)
{
    int rows = sell_list.size();
    if ( rows <= 0 )
    {
        return ;
    }

    /* 分为大小件商品 */
    GoodsStockModel stock_model;
    QVector<GoodsStock *> main_list;
    QVector<GoodsStock *> sub_list;
    for ( int i = 0; i < rows; i++ )
    {
        GoodsStock *stock = stock_model.getStockBySN(sell_list[i]->SN());
        int sell_number = sell_list[i]->Number();
        int stock_number = stock->Number();
        int cur_num = stock_number - sell_number;
        stock->Number(cur_num);
        if ( stock->isSmall() != "Y" )
        {
            main_list.append(stock);
        }
        else if ( (stock->isSmall() == "Y") && (cur_num >= 0 ) )
        {
            main_list.append(stock);
        }
        else
        {
            sub_list.append(stock);
        }
    }

    stock_model.batchUpdateGoodsStock(main_list);
    freeStockList(main_list);

    int sub_rows = sub_list.size();
    if ( sub_rows > 0 )
    {
        GoodsInfoModel info_model;
        for ( int i = 0; i < sub_rows; i++ )
        {
            QString main_sn = info_model.getMainSerialNumBySubSN(sub_list[i]->SN());
            int rate = info_model.getRateBySN(main_sn);
            GoodsStock *m_stock = stock_model.getStockBySN(main_sn);
            GoodsStock *s_stock = sub_list[i];
            int need_big_num = 1 - ( s_stock->Number() / rate );
            int cur_num = m_stock->Number() - need_big_num;
            m_stock->Number(cur_num);
            s_stock->Number(need_big_num * rate + s_stock->Number());
            qDebug("need big:%d, current big :%d***\r\n", need_big_num, cur_num);
            sub_list.append(m_stock);
        }
        stock_model.batchUpdateGoodsStock(sub_list);
        freeStockList(sub_list);
    }


    return ;
}

void ShopMainWindow::sellTotal(bool total)
{
    if ( true == total )
    {        
        /* 插入销售表,并更新库存 */
        int rows = m_sellModel->rowCount();
        QVector<GoodsSell *> sell_list;
        QModelIndex index;
        for ( int i = 0; i < rows; i++ )
        {
            GoodsSell *sell = new GoodsSell();
            /* 售价 */
            index = m_sellModel->index(i, GOODS_SELL_PRICE);
            sell->Price(index.data().toDouble());

            /* 日期和时间 */
            sell->Date(QDate::currentDate());
            sell->Time(QTime::currentTime());

            /* SN */
            index = m_sellModel->index(i, GOODS_SELL_SN);
            sell->SN(index.data().toString());

            /* 折扣率 */
            index = m_sellModel->index(i, GOODS_SELL_DISCOUNT_RATE);
            sell->DiscountRate(index.data().toDouble());

            /* 销售数量 */
            index = m_sellModel->index(i, GOODS_SELL_NUMBER);
            sell->Number(index.data().toInt());

            /* 总金额 */
            index = m_sellModel->index(i, GOODS_SELL_TOTAL);
            sell->Total(index.data().toDouble());

            /* 利润*/
            index = m_sellModel->index(i, GOODS_SELL_PROFIT);
            sell->Profit(index.data().toDouble());

            /* 单位 */
            index = m_sellModel->index(i, GOODS_SELL_HIDE_UNIT);
            sell->UnitID(index.data().toInt());

            sell_list.append(sell);
        }
        GoodsSellModel sell_model;
        (void)sell_model.batchInsertSell(sell_list);

        updateStockTableBySellList(sell_list);

        freeSellList(sell_list);

        showMessage(tr("交易成功"));
    }
    else
    {
        showMessage(tr("交易取消"));
    }

    on_pushButton_7_clicked();

    return ;
}

void ShopMainWindow::on_pushButton_6_clicked()
{
    QModelIndex index;
    double total = 0.0;
    int rows = m_sellModel->rowCount();

    for ( int i = 0; i < rows; i++ )
    {
        index = m_sellModel->index(i, GOODS_SELL_TOTAL);
        total += index.data().toDouble();
    }
    /* 弹出结算界面 */
    if ( rows > 0 )
    {
        m_sellTotalWindow->showTotal(total);
    }
    else
    {
        ui->edit_input_sell_sn->setFocus();
    }

    return ;
}


void ShopMainWindow::on_btn_query_refresh_clicked()
{
    int index = ui->comboBox_query_table->currentIndex();
    switch (index)
    {
    case COMBOX_ITEM_INFO:
        showInfoTable();
        break;
    case COMBOX_ITEM_PURCHASE:
        showPurchaseTable();
        break;
    case COMBOX_ITEM_SELL:
        showSellTable();
        break;
    case COMBOX_ITEM_STOCK:
        showStockTable();
        break;
    default:
        break;
    }

    return ;
}

void ShopMainWindow::on_lEdit_personal_userName_returnPressed()
{
    ui->lEdit_personal_newPasswd->setFocus();
    ui->lEdit_personal_newPasswd->selectAll();
}

void ShopMainWindow::on_lEdit_personal_newPasswd_returnPressed()
{
    ui->lEdit_personal_confirmPasswd->setFocus();
    ui->lEdit_personal_confirmPasswd->selectAll();
}

void ShopMainWindow::on_lEdit_personal_confirmPasswd_returnPressed()
{
    on_btn_personal_submit_clicked();
}

void ShopMainWindow::on_btn_query_submit_clicked()
{
    int index = ui->comboBox_query_table->currentIndex();
    switch(index)
    {
    case COMBOX_ITEM_INFO:
    {
        setQueryInfo();
        break;
    }
    case COMBOX_ITEM_PURCHASE:
    {
        setQueryPurchase();
        break;
    }
    case COMBOX_ITEM_SELL:
    {
        setQuerySell();
        break;
    }
    case COMBOX_ITEM_STOCK:
    {
        setQueryStock();
        break;
    }
    default:
        break;
    }

    return ;
}

void ShopMainWindow::setQueryStock()
{
    QString beginDate;
    QString endDate;
    beginDate = ui->dateEdit_query_begin->date().toString("yyyy-MM-dd");
    endDate = ui->dateEdit_query_end->date().toString("yyyy-MM-dd");
    QString nameorsn = ui->lEdit_query_nameorsn->text();
    QString querystr = "SELECT STOCK_ID, STOCK_DATE, STOCK_SN,"
            "(select GOODS_NAME from goods_info where goods_mainsn = stock_sn or goods_subsn = stock_sn) as name,"
            "STOCK_NUMBER, "
            "(select unit_name from unit_info where unit_id = stock_unit_id),"
            "STOCK_PURCHASE_PRICE, STOCK_SELL_PRICE "
            "FROM GOODS_STOCK" ;
    setStockQuery(querystr + " where (name like '%" + nameorsn + "%' OR " +
                  " STOCK_SN like '%" + nameorsn + "%') AND (" +
                  " STOCK_DATE BETWEEN '" + beginDate + "' AND '" + endDate + "')");
}

void ShopMainWindow::setQueryInfo()
{
    QString nameorsn = ui->lEdit_query_nameorsn->text();
    QString querystr = "SELECT GOODS_ID, GOODS_CODE, GOODS_NAME, "
            "(select TYPE_NAME from GOODS_TYPE where type_id = goods_type), "
            "GOODS_SPEC, "
            "GOODS_MANUFACTURER, "
            "GOODS_ORIGIN_PLACE, "
            "GOODS_MAINSN, "
            "(select UNIT_NAME from UNIT_INFO where UNIT_ID = GOODS_MAINUNIT),"
            "GOODS_SUBSN, "
            "(select UNIT_NAME from UNIT_INFO where UNIT_ID = GOODS_SUBUNIT),"
            "GOODS_EXCHANGE_RATE FROM GOODS_INFO ";
    setInfoQuery(querystr + " where (GOODS_NAME like '%" + nameorsn + "%' OR " +
                 " GOODS_MAINSN like '%" + nameorsn + "%' OR " + " GOODS_SUBSN like '%" + nameorsn + "%')");
}

void ShopMainWindow::setQueryPurchase()
{
    QString beginDate;
    QString endDate;
    beginDate = ui->dateEdit_query_begin->date().toString("yyyy-MM-dd");
    endDate = ui->dateEdit_query_end->date().toString("yyyy-MM-dd");
    QString nameorsn = ui->lEdit_query_nameorsn->text();
    QString querystr = "SELECT id, purchase_date, purchase_time, purchase_sn, "
            "(select goods_name from goods_info where goods_mainsn = purchase_sn or goods_subsn = purchase_sn) as name,"
            "(select unit_name from unit_info where unit_id = purchase_unit_id),"
            "purchase_supplier, purchase_price, purchase_number, purchase_total, purchase_remark "
            "FROM GOODS_PURCHASE ";
    setPurchaseQuery(querystr + "where (name like '%" + nameorsn + "%' OR " +
                     " purchase_sn like '%" + nameorsn + "%') AND (" +
                     " purchase_date BETWEEN '" + beginDate + "' AND '" + endDate + "')");
}

void ShopMainWindow::setQuerySell()
{
    QString beginDate;
    QString endDate;
    beginDate = ui->dateEdit_query_begin->date().toString("yyyy-MM-dd");
    endDate = ui->dateEdit_query_end->date().toString("yyyy-MM-dd");
    QString nameorsn = ui->lEdit_query_nameorsn->text();
    QString querystr = "SELECT sell_id, sell_date, sell_time, sell_sn, "
            "(select goods_name from goods_info where goods_mainsn = sell_sn or goods_subsn = sell_sn) as name,"
            "(select stock_purchase_price from goods_stock where stock_sn = sell_sn), "
            "sell_price, sell_number, "
            "(select unit_name from unit_info where unit_id = sell_unit_id),"
            "sell_discount_rate, sell_total, sell_profit "
            " FROM GOODS_SELL ";
    setSellQuery(querystr + " where (name like '%" + nameorsn + "%' OR " +
                 " sell_sn like '%" + nameorsn + "%') AND (" +
                 " sell_date BETWEEN '" + beginDate + "' AND '" + endDate + "')");
}

void ShopMainWindow::hideBalanceButton(bool isHide)
{
    if ( true == isHide )
    {
        ui->btn_query_balance_by_date->hide();
        ui->btn_query_balance_today->hide();
    }
    else
    {
        ui->btn_query_balance_by_date->show();
        ui->btn_query_balance_today->show();
    }
}

bool ShopMainWindow::hasRecord(void)
{
    int rowCount = m_queryModel->rowCount();
    if ( rowCount > 0 )
        return true;
    return false;
}

void ShopMainWindow::exportSellBalance(void)
{
    if ( true == hasRecord() )
    {
        QDate current = QDate::currentDate();
        int year = current.year();
        int month = current.month();
        int day = current.day();
        QString defaultName = tr("销售结算-%1-%2-%3.csv").arg(QString::number(year), QString::number(month), QString::number(day));
        QString saveFile = showSaveFileDialog(defaultName);
        if ( saveFile != "" )
        {
            exportSellBalanceFile(saveFile);
        }
    }
    else
    {
        showMessage(tr("今天没有卖出任何商品 :("));
    }
}

void ShopMainWindow::exportSellBalanceFile(QString saveFile)
{
    QFile file(saveFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       return;
    }

    QTextStream out(&file);
    int rowCount = m_queryModel->rowCount();
    int columnCount = m_queryModel->columnCount();
    double sell_total = 0.0;
    double sell_profit = 0.0;

    out.setCodec("GBK");
    QString headData;
    for ( int index = 0; index < columnCount; index++)
    {
        headData += m_queryModel->headerData(index,  Qt::Horizontal).toString() + ",";
    }
    out << headData << "\n";
    for ( int i = 0; i < rowCount; i++ )
    {
       QModelIndex index;
       for ( int j = 0; j < columnCount; j++ )
       {
           index = m_queryModel->index(i, j);
           if ( j == TABLE_SELL_TOTAL )
           {
               sell_total += index.data().toDouble();
           }
           else if ( j == TABLE_SELL_PROFIT )
           {
               sell_profit += index.data().toDouble();
           }
           out << m_queryModel->data(index).toString().replace("\n", " ") << ",";
       }
       out << "\n";
    }
    QString begin_date = ui->dateEdit_query_begin->text();
    QString end_date = ui->dateEdit_query_end->text();
    out << "\n\n\n";
    out << tr(",,销售总金额,销售总利润,结算起始日期,结算终止日期, ") << "\n";
    out << ",,"<< QString::number(sell_total) << "," << QString::number(sell_profit) << ","
        << begin_date << "," << end_date << "," << "\n";
    out.flush();
    file.close();
    QString end = tr("销售日期( %3 - %4 ): 销售总金额:%1元, 销售总利润:%2元").arg(QString::number(sell_total),
                                                                       QString::number(sell_profit),
                                                                       begin_date, end_date);
    showMessage(end);
}

void ShopMainWindow::exportPurchaseBalanceFile(QString name)
{
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       return;
    }

    QTextStream out(&file);
    int rowCount = m_queryModel->rowCount();
    int columnCount = m_queryModel->columnCount();
    double purchase_total  = 0.0;

    out.setCodec("GBK");
    QString headData;
    for ( int index = 0; index < columnCount; index++)
    {
        headData += m_queryModel->headerData(index,  Qt::Horizontal).toString() + ",";
    }
    out << headData << "\n";
    for ( int i = 0; i < rowCount; i++ )
    {
       QModelIndex index;
       for ( int j = 0; j < columnCount; j++ )
       {
           index = m_queryModel->index(i, j);
           if ( j == TABLE_PURCHASE_TOTAL )
           {
               purchase_total += index.data().toDouble();
           }
           out << m_queryModel->data(index).toString().replace("\n", " ") << ",";
       }
       out << "\n";
    }
    QString begin_date = ui->dateEdit_query_begin->text();
    QString end_date = ui->dateEdit_query_end->text();
    out << "\n\n\n";
    out << tr(",,采购总金额,采购起始日期,采购终止日期,") << "\n";
    out << ",,"<< QString::number(purchase_total) << ","
        << begin_date << "," << end_date << "," << "\n";
    out.flush();
    file.close();
    QString end = tr("采购日期( %3 - %4 ): 采购总金额:%1元").arg(QString::number(purchase_total),
                                                                       begin_date, end_date);
    showMessage(end);
}
void ShopMainWindow::exportPurchaseBalance(void)
{
    if ( true == hasRecord() )
    {
        QDate current = QDate::currentDate();
        int year = current.year();
        int month = current.month();
        int day = current.day();
        QString defaultName = tr("采购结算-%1-%2-%3.csv").arg(QString::number(year), QString::number(month), QString::number(day));
        QString saveFile = showSaveFileDialog(defaultName);
        if ( saveFile != "" )
        {
            exportPurchaseBalanceFile(saveFile);
        }
    }
    else
    {
        showMessage(tr("今天没有采购任何商品!"));
    }
}

void ShopMainWindow::on_btn_query_balance_by_date_clicked()
{
    int selectTable = ui->comboBox_query_table->currentIndex();

    if ( COMBOX_ITEM_SELL == selectTable )
    {
        setQuerySell();
        exportSellBalance();
    }
    else if ( COMBOX_ITEM_PURCHASE == selectTable )
    {
        setQueryPurchase();
        exportPurchaseBalance();
    }
}

void ShopMainWindow::on_btn_query_balance_today_clicked()
{
    int selectTable = ui->comboBox_query_table->currentIndex();
    ui->dateEdit_query_begin->setDate(QDate::currentDate());
    ui->dateEdit_query_end->setDate(QDate::currentDate());
    if ( COMBOX_ITEM_SELL == selectTable )
    {
        setQuerySell();
        exportSellBalance();
    }
    else if ( COMBOX_ITEM_PURCHASE == selectTable )
    {
        setQueryPurchase();
        exportPurchaseBalance();
    }
}

void ShopMainWindow::on_lEdit_stockName_returnPressed()
{
    /* 查询信息库填充商品名称和小件条码 */
    GoodsInfoModel info_model;
    GoodsInfo *info = info_model.getGoodsInfoByName(entryLineEdit(ENTRY_NAME)->text());
    if ( info != NULL )
    {
        GoodsStockModel stock_model;
        QString subsn = info->SubSerialNum();
        QString mainsn = info->MainSerialNum();
        entryLineEdit(ENTRY_SUBSN)->setText(subsn);
        entryLineEdit(ENTRY_MAIN_SN)->setText(mainsn);
        GoodsStock *mainstock = stock_model.getStockBySN(mainsn);
        if ( NULL != mainstock )
        {
            entryLineEdit(ENTRY_STOCK_NUMBER)->setText(QString::number(mainstock->Number()));
            entryLineEdit(ENTRY_PURCHASE_PRICE)->setText(QString::number(mainstock->PurchasePrice()));
            entryLineEdit(ENTRY_MAIN_PRICE)->setText(QString::number(mainstock->SellPrice()));
            delete mainstock;
        }
        GoodsStock *substock = stock_model.getStockBySN(subsn);
        if ( NULL != substock )
        {
            entryLineEdit(ENTRY_STOCK_NUMBER1)->setText(QString::number(substock->Number()));
            entryLineEdit(ENTRY_SUB_PRICE)->setText(QString::number(substock->SellPrice()));
            delete substock;
        }

        delete info;
    }
    else
    {
        for (int i = 0; i < ENTRY_MAX; i++ )
        {
            if ( i != ENTRY_NAME )
            {
                entryLineEdit(i)->setText("");
            }
        }
    }
    return ;
}

void ShopMainWindow::on_lEdit_stockName_textChanged(const QString &arg1)
{
    /* 查询信息库填充商品名称和小件条码 */
    GoodsInfoModel info_model;
    QStringList wordList = info_model.getGoodsInfoNameListByName(arg1);
    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer->setWrapAround(true);
    completer->setMaxVisibleItems(5);
    entryLineEdit(ENTRY_NAME)->setCompleter(completer);

    on_lEdit_stockName_returnPressed();

    return ;
}
