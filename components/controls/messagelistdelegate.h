#ifndef MessageListDelegate_H
#define MessageListDelegate_H

#include <QAbstractItemDelegate>

class MessageListDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit MessageListDelegate(QObject *parent = nullptr);
    ~MessageListDelegate() = default;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    void drawSelfBubble(QPainter *painter,
                        const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawOtherBubble(QPainter *painter,
                         const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize getTextSize(const QStyleOptionViewItem &option, const QString& text) const;

private:
    int _avatarSize;
    int _topMargin;
    int _spacing;
    QFont _font;
};

#endif // MessageListDelegate_H
