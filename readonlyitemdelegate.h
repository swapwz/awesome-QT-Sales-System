#ifndef READONLYITEMDELEGATE_H
#define READONLYITEMDELEGATE_H

#include <QItemDelegate>

class readonlyItemDelegate : public QItemDelegate
{
public:
    readonlyItemDelegate(QObject* parent = 0) : QItemDelegate(parent){}
    readonlyItemDelegate();
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // READONLYITEMDELEGATE_H
