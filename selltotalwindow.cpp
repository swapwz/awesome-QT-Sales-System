#include "selltotalwindow.h"
#include "ui_selltotalwindow.h"
#include <QTextCodec>
#include <QMessageBox>

SellTotalWindow::SellTotalWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SellTotalWindow)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    ui->setupUi(this);
    ui->lEdit_total_payment->setFocus();
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
}

SellTotalWindow::~SellTotalWindow()
{
    delete ui;
}

void SellTotalWindow::showTotal(double total)
{
    ui->lEdit_total_total->setText(QString::number(total));
    this->show();
    ui->lEdit_total_payment->setFocus();
}

void SellTotalWindow::on_btn_total_submit_clicked()
{
    QString payment = ui->lEdit_total_payment->text();
    QString change = ui->lEdit_total_change->text();

    if ( (payment.length() == 0) || (change.length() == 0) || (change.toDouble() < 0))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("温馨提示"));
        msgBox.setText(QObject::tr("输入错误"));
        msgBox.exec();
        ui->lEdit_total_payment->setFocus();

        return ;
    }
    this->setWindowFlags(Qt::Window);
    emit sellSubmit(true);
    this->hide();
    ui->lEdit_total_change->setText("");
    ui->lEdit_total_payment->setText("");
    ui->lEdit_total_total->setText("");
}

void SellTotalWindow::on_btn_cancel_submit_clicked()
{
    emit sellSubmit(false);
    ui->lEdit_total_change->setText("");
    ui->lEdit_total_payment->setText("");
    ui->lEdit_total_total->setText("");
    this->hide();
}

void SellTotalWindow::on_lEdit_total_payment_returnPressed()
{
    bool isDouble;
    double total = ui->lEdit_total_total->text().toDouble();
    double payment = ui->lEdit_total_payment->text().toDouble(&isDouble);
    if ( (true == isDouble) && (payment - total >= 0))
    {
        ui->lEdit_total_change->setText(QString::number(payment-total));
        ui->lEdit_total_change->setFocus();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("温馨提示"));
        msgBox.setText(QObject::tr("输入错误"));
        msgBox.exec();
        ui->lEdit_total_payment->setText("");
        ui->lEdit_total_payment->setFocus();
    }
}

void SellTotalWindow::on_lEdit_total_change_returnPressed()
{
    on_btn_total_submit_clicked();
}
