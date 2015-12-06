#ifndef SELLTOTALWINDOW_H
#define SELLTOTALWINDOW_H

#include <QMainWindow>

namespace Ui {
class SellTotalWindow;
}

class SellTotalWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SellTotalWindow(QWidget *parent = 0);
    ~SellTotalWindow();
    void showTotal(double total);
signals:
    void sellSubmit(bool);
private slots:
    void on_btn_total_submit_clicked();

    void on_btn_cancel_submit_clicked();

    void on_lEdit_total_payment_returnPressed();

    void on_lEdit_total_change_returnPressed();

private:
    Ui::SellTotalWindow *ui;
};

#endif // SELLTOTALWINDOW_H
