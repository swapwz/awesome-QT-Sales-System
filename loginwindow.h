#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

public slots:
    void rememberUserName(void);
    void on_btnLogin_clicked();
signals:
    void needRemeberUserName(void);
    void loginValidate(int);

private slots:
    void clearErrInfo();
    void on_lineEditPasswd_returnPressed();
    void on_lineEditUserName_returnPressed();

private:
    Ui::LoginWindow *ui;
    void setErrInfo();
};

#endif // LOGINWINDOW_H
