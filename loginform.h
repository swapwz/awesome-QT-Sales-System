#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>

namespace Ui {
class loginform;
}

class loginform : public QWidget
{
    Q_OBJECT

public:
    explicit loginform(QWidget *parent = 0);
    ~loginform();

private:
    Ui::loginform *ui;
};

#endif // LOGINFORM_H
