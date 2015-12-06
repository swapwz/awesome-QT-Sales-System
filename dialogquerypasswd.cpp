#include "dialogquerypasswd.h"
#include "ui_dialogquerypasswd.h"
#include <QTextCodec>

DialogQueryPasswd::DialogQueryPasswd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogQueryPasswd)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    ui->setupUi(this);
    ui->lEdit_dialog_askPasswd->setFocus();
    this->setWindowTitle(QObject::tr("安全提示"));
}
QString DialogQueryPasswd::getQueryPasswd(void)
{
    return ui->lEdit_dialog_askPasswd->text();
}
DialogQueryPasswd::~DialogQueryPasswd()
{
    delete ui;
}
