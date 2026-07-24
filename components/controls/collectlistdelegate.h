#ifndef COLLECTLISTDELEGATE_H
#define COLLECTLISTDELEGATE_H

#include <QAbstractItemDelegate>

class CollectListDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit CollectListDelegate(QObject* parent = nullptr);
    ~CollectListDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};

#endif // COLLECTLISTDELEGATE_H
