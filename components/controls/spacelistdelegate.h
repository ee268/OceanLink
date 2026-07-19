#ifndef SPACELISTDELEGATE_H
#define SPACELISTDELEGATE_H

#include <QStyledItemDelegate>

class SpaceListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SpaceListDelegate(QObject *parent = nullptr);
    ~SpaceListDelegate() = default;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    int _avatarSize = 48;
    int _margin = 12;
};

#endif // SPACELISTDELEGATE_H
