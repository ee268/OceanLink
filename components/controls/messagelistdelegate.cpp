#include "messagelistdelegate.h"

#include <QPainter>
#include <QPainterPath>
#include <QTextDocument>
#include <QDebug>

#include "messagelistmodel.h"

#include "ElaTheme.h"

const int ChatMsgMaxWidth = 500;

MessageListDelegate::MessageListDelegate(QObject *parent)
    : QAbstractItemDelegate{parent}
    , _avatarSize(40)
    , _topMargin(25)
    , _spacing(7)
{
    _font.setPixelSize(14);
}

void MessageListDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    bool isSelf = index.data(MessageListModel::IsSelf).toBool();

    if (isSelf) {
        this->drawSelfBubble(painter, option, index);
    }
    else {
        this->drawOtherBubble(painter, option, index);
    }
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QString msg = index.data(MessageListModel::Msg).toString();
    QSize size = getTextSize(option, msg);

    size.setHeight(size.height() + _topMargin + 10);

    return size;
}

void MessageListDelegate::drawSelfBubble(QPainter *painter,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QString msg = index.data(MessageListModel::Msg).toString();
    QString date = index.data(MessageListModel::Date).toString();
    QPixmap avatar = qvariant_cast<QPixmap>(index.data(MessageListModel::Avatar));
    QString name = index.data(MessageListModel::Name).toString();

    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter->save();
    // painter->fillRect(option.rect, Qt::gray);

    //头像
    QRect avatarRect(option.rect.right() - _avatarSize - _topMargin,
                     option.rect.top() + _topMargin,
                     _avatarSize, _avatarSize);
    if (!avatar.isNull()) {
        QPixmap pixmap = avatar.scaled(_avatarSize, _avatarSize,
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPainterPath path;
        path.addEllipse(avatarRect);

        painter->setClipPath(path); //设置裁切区域
        painter->drawPixmap(avatarRect, pixmap);
        painter->restore();
    }
    else {
        painter->setPen(QPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep)));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(avatarRect.x(), avatarRect.y(), _avatarSize, _avatarSize);
        painter->drawText(avatarRect, Qt::AlignCenter, name.at(0));
        painter->restore();
    }

    //信息
    QSize size = this->getTextSize(option, msg);
    int w = size.width();
    int h = size.height();

    QRect msgRect(avatarRect.right() - _avatarSize - _spacing - w,
                  avatarRect.top(),
                  w, h);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#118df0"));
    painter->drawRoundedRect(msgRect, 10, 10);

    QRect textRect = msgRect.adjusted(5, 0, -5, 0);
    painter->setFont(_font);
    painter->setPen(Qt::white);
    painter->drawText(textRect, Qt::TextWrapAnywhere | Qt::AlignLeft | Qt::AlignVCenter, msg);
}

void MessageListDelegate::drawOtherBubble(QPainter *painter,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    QString msg = index.data(MessageListModel::Msg).toString();
    QString date = index.data(MessageListModel::Date).toString();
    QPixmap avatar = qvariant_cast<QPixmap>(index.data(MessageListModel::Avatar));
    QString name = index.data(MessageListModel::Name).toString();

    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter->save();
    // painter->fillRect(option.rect, Qt::cyan);

    //头像
    QRect avatarRect(option.rect.left() + _topMargin,
                     option.rect.top() + _topMargin,
                     _avatarSize, _avatarSize);
    if (!avatar.isNull()) {
        QPixmap pixmap = avatar.scaled(_avatarSize, _avatarSize,
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPainterPath path;
        path.addEllipse(avatarRect);

        painter->setClipPath(path); //设置裁切区域
        painter->drawPixmap(avatarRect, pixmap);
        painter->restore();
    }
    else {
        painter->setPen(QPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep)));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(avatarRect.x(), avatarRect.y(), _avatarSize, _avatarSize);
        painter->drawText(avatarRect, Qt::AlignCenter, name.at(0));
        painter->restore();
    }

    //信息
    QSize size = this->getTextSize(option, msg);
    int w = size.width();
    int h = size.height();

    QRect msgRect(avatarRect.right() + _spacing,
                  avatarRect.top(),
                  w, h);
    painter->setPen(Qt::NoPen);
    painter->setBrush(ElaThemeColor(eTheme->getThemeMode(), BasicBaseDeep));
    painter->drawRoundedRect(msgRect, 10, 10);

    QRect textRect = msgRect.adjusted(5, 0, -5, 0);
    painter->setFont(_font);
    painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicText));
    painter->drawText(textRect, Qt::TextWrapAnywhere | Qt::AlignLeft | Qt::AlignVCenter, msg);
}

QSize MessageListDelegate::getTextSize(const QStyleOptionViewItem &option,
                                     const QString &text) const
{
    QFontMetrics fm(_font);

    QStringList lines = text.split('\n');
    int actualMaxWidth = 0;

    //最大行宽获取
    for (const QString& line : lines) {

        int lineWidth = fm.horizontalAdvance(line);

        //空行
        if (line.size() == 0) {
            //至少计算一个字符宽度，确保气泡高度正确
            lineWidth = fm.horizontalAdvance("p");
        }

        if (lineWidth > actualMaxWidth) {
            actualMaxWidth = lineWidth;
        }
    }

    QTextDocument textDoc;
    textDoc.setDefaultFont(_font);
    textDoc.setTextWidth(qMin(actualMaxWidth, ChatMsgMaxWidth));
    textDoc.setPlainText(text);
    QSizeF sizeF = textDoc.size();
    QSize res(sizeF.width() + 10, sizeF.height());

    return res;
}


