#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteDialog;
}

class DeleteDialog : public QDialog
{
    Q_OBJECT

public:
    void setText(QString label);
    explicit DeleteDialog(QWidget *parent = 0);
    ~DeleteDialog();

private:
    Ui::DeleteDialog *ui;
};

#endif // DELETEDIALOG_H
