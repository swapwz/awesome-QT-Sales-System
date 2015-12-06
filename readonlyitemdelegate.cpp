#include "readonlyitemdelegate.h"

readonlyItemDelegate::readonlyItemDelegate()
{
}

QWidget *readonlyItemDelegate::createEditor(QWidget *parent,
                    const QStyleOptionViewItem &option,
                    const QModelIndex &index) const
{
    (void) option;
    (void) index;
    (void) parent;
    return NULL;
}
