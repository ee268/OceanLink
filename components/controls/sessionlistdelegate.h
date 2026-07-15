#ifndef SESSIONLISTDELEGATE_H
#define SESSIONLISTDELEGATE_H

#include <QAbstractItemDelegate>

class SessionListDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit SessionListDelegate(QObject *parent = nullptr);
    ~SessionListDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

};

#endif // SESSIONLISTDELEGATE_H
