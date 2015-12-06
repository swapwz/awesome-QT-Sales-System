#include "loginform.h"
#include "ui_loginform.h"

loginform::loginform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginform)
{
    ui->setupUi(this);
}

loginform::~loginform()
{
    delete ui;
}
