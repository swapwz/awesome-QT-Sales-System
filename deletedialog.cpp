#include "deletedialog.h"
#include "ui_deletedialog.h"

DeleteDialog::DeleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteDialog)
{
    ui->setupUi(this);
}

void DeleteDialog::setText(QString label)
{
    ui->label->setText(label);
}

DeleteDialog::~DeleteDialog()
{
    delete ui;
}
