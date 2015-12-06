#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "model/userinfomodel.h"
#include <QPushButton>
#include <QSqlQuery>
#include <QTimer>


LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->lineEditUserName->setFocus();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    this->setTabOrder(ui->lineEditUserName, ui->lineEditPasswd);
    this->setTabOrder(ui->lineEditPasswd, ui->btnLogin);
    this->setTabOrder(ui->btnLogin,  ui->lineEditUserName);
    connect(this, SIGNAL(needRemeberUserName()), this, SLOT(rememberUserName()));
    /* Read remember user to line edit */
    QFile rem_file(".rem_user.data");
    if ( true == rem_file.exists() )
    {
        rem_file.open(QIODevice::ReadOnly);
        ui->lineEditUserName->setText(tr(rem_file.readAll()));
        rem_file.close();
        ui->checkBox->setChecked(true);
        ui->lineEditPasswd->setFocus();
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_btnLogin_clicked()
{
    QString szUserName = ui->lineEditUserName->text();
    QString szPasswd   = ui->lineEditPasswd->text();

    bool bIsValidate = false;

    if ( ("" != szUserName.trimmed() ) && ("" != szPasswd) )
    {
        UserInfoModel model;
        UserInfo *info;
        info = model.getUserInfoByName(szUserName);

        if ( NULL != info )
        {
            qDebug() << info->Name() << info->Password();
            if ( info->Password() == szPasswd )
            {
                emit loginValidate(info->ID());
                emit needRemeberUserName();
                this->close();

                bIsValidate = true;
            }
            delete info;
        }
    }

    if ( true != bIsValidate )
    {
        setErrInfo();
        QTimer::singleShot(1000, this, SLOT(clearErrInfo()));
    }

    return ;
}

void LoginWindow::setErrInfo()
{
    ui->labelErrInfo->setText("用户名或密码错误");
    return;
}

void LoginWindow::clearErrInfo()
{
    ui->labelErrInfo->setText("");
    return;
}

void LoginWindow::on_lineEditPasswd_returnPressed()
{
    on_btnLogin_clicked();
}

void LoginWindow::on_lineEditUserName_returnPressed()
{
    ui->lineEditPasswd->setFocus();
}

void LoginWindow::rememberUserName(void)
{
    QFile rem_file(".rem_user.data");
    if ( true == rem_file.exists() )
    {
        rem_file.remove();
    }
    if ( true == ui->checkBox->isChecked() )
    {
        rem_file.open(QIODevice::WriteOnly);
        rem_file.write(ui->lineEditUserName->text().toStdString().c_str());
        rem_file.close();
    }
}
