#ifndef DIALOGQUERYPASSWD_H
#define DIALOGQUERYPASSWD_H

#include <QDialog>

namespace Ui {
class DialogQueryPasswd;
}

class DialogQueryPasswd : public QDialog
{
    Q_OBJECT

public:
    QString getQueryPasswd(void);
    explicit DialogQueryPasswd(QWidget *parent = 0);
    ~DialogQueryPasswd();

private:
    Ui::DialogQueryPasswd *ui;
};

#endif // DIALOGQUERYPASSWD_H
