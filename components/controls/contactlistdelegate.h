#ifndef CONTACTLISTDELEGATE_H
#define CONTACTLISTDELEGATE_H

#include <QStyledItemDelegate>

class ContactListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ContactListDelegate(QObject *parent = nullptr);
    ~ContactListDelegate() = default;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    void paintGroupItem(QPainter *painter, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
    void paintContactItem(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    int _groupHeight = 40;
    int _contactHeight = 60;
    int _iconSize = 16;
    int _avatarSize = 40;
    int _leftPadding = 10;
    int _spacing = 10;
};

#endif // CONTACTLISTDELEGATE_H
