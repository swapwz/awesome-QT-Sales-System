#ifndef SHOPMAINWINDOW_H
#define SHOPMAINWINDOW_H

#include <QLineEdit>
#include <QtGui>
#include <QMainWindow>
#include "tool/editablequerymodel.h"

namespace Ui {
class ShopMainWindow;
}

typedef struct CheckLineEdit
{
    int editType;
    int checkType;
    bool checkFailed;
} CHECK_LINE_EDIT_S;

class QFileDialog;
class EditableQueryModel;
class SellTotalWindow;
class GoodsStock;
class GoodsSell;
class PurchaseInfo;
class GoodsInfo;
class QSqlQueryModel;
class ShopMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShopMainWindow(QWidget *parent = 0);
    ~ShopMainWindow();

signals:
    void addGoodsStock(QString);
    void wrongPasswd(void);
    void fillInfo(QString);

private slots:
    void selectModelForQuery(int index);
    void fillInfoBySN(QString main_sn);
    void wrongHint(void);
    void login(int user_id);
    void on_edit_input_sell_sn_returnPressed();
    void clearHint(void);
    void on_btn_goodsInfoReset_clicked();
    void on_btn_goodsAddNewInfo_clicked();
    void addGoodsStockBySN(QString sn);
    void on_btn_goodsInfoSubmitAll_clicked();
    void setTabFocus(int index);
    void on_pushButton_2_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_btn_personal_submit_clicked();

    void on_btn_stock_reset_clicked();

    void on_lEdit_stock_mainSN_returnPressed();

    void on_btn_stock_addNew_clicked();

    void on_lEdit_stock_mainSN_textChanged(const QString &arg1);

    void on_btn_stock_deleteSelected_clicked();

    void on_btn_goodsInfoDeleteSelected_clicked();

    void on_btn_stock_submit_clicked();

    void on_pushButton_8_clicked();
    void updateStockView(QStandardItem *item);
    void on_btn_query_deleteSelected_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_7_clicked();

    void sellTotal(bool);

    void on_pushButton_6_clicked();
    void updateSellView(QStandardItem *item);

    void on_btn_query_refresh_clicked();

    void on_lEdit_personal_userName_returnPressed();

    void on_lEdit_personal_newPasswd_returnPressed();

    void on_lEdit_personal_confirmPasswd_returnPressed();

    void on_btn_query_submit_clicked();

    void on_btn_query_balance_by_date_clicked();

    void on_btn_query_balance_today_clicked();

    void on_lEdit_stockName_returnPressed();

    void on_lEdit_stockName_textChanged(const QString &arg1);

private:

    void WriteModelDataToFile(QString name, QSqlQueryModel* model);
    void initQueryDateUI();
    QString getCurrentSubSN();
    QString getCurrentMainSN();
    bool checkSubCode(QString subsn);
    bool hasRecord(void);
    void exportPurchaseBalance(void);
    void exportPurchaseBalanceFile(QString name);
    void exportSellBalanceFile(QString saveFile);
    QString showSaveFileDialog(QString defaultName);
    void exportSellBalance(void);
    void setQueryStock();
    void setQueryStockHeader();
    void setQuerySell();
    void setQuerySellHeader();
    void setQueryPurchase();
    void setQueryPurchaseHeader();
    void setQueryInfo();
    void setQueryInfoHeader();
    void setInfoQuery(QString querystr);
    void setStockQuery(QString querystr);
    void setPurchaseQuery(QString querystr);
    void setSellQuery(QString querystr);
    void freeSellList(QVector<GoodsSell *> list);
    void freePurchaseList(QVector<PurchaseInfo *> list);
    void addGoodsToSellView(GoodsInfo *info, GoodsStock *stock);
    void showInfoTable();
    void showPurchaseTable();
    void showStockTable();
    void showSellTable();
    void exportFile(QString name);
    void setQueryComboxItemText(void);
    void freeStockList(QVector<GoodsStock *> list);
    void setStockTableHeader(void);
    QLineEdit* entryLineEdit(int type);
    void initEntryLineEditList(void);
    void setPersonalTabOrder(void);
    int askPasswd(void);
    void setEntryTabOrder(void);
    void initInfoTabUI(void);
    void setInfoTabOrder(void);
    void addShortCurForInfoTab(void);
    void addShortCutForEntryTab(void);
    bool validateInfoLineEdit(CHECK_LINE_EDIT_S *checkList, int size, int &edit_type);
    void setInfoTableHeader(void);
    void showMessage(QString text);
    QLineEdit* getCurrentLineEdit(int type);
    QStandardItemModel *m_goodsInfoModel;
    QStandardItemModel *m_stockModel;
    QStandardItemModel *m_sellModel;
    QSqlQueryModel *m_queryModel;
    void initInfoLineEditList(void);
    void setTableView();
    bool checkBarcode(const QString &);
    bool checkModelHasStock(QString sn);
    void setSellTableHeader(void);
    QLineEdit* infoLineEdit(int type);
    Ui::ShopMainWindow *ui;
    int m_userid;
    void updateStockTableBySellList(QVector<GoodsSell *> sell_list);
    SellTotalWindow *m_sellTotalWindow;
    EditableQueryModel m_stockEditModel;
    void hideBalanceButton(bool isHide);
    void showCreditTable();
    void setQueryCreditHeader();
    void setCreditQuery(QString querystr);
};

#endif // SHOPMAINWINDOW_H
