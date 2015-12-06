#include "selltotalform.h"
#include "ui_selltotalform.h"

SellTotalForm::SellTotalForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SellTotalForm)
{
    ui->setupUi(this);
}

SellTotalForm::~SellTotalForm()
{
    delete ui;
}
