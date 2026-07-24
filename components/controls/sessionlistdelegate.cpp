#include "sessionlistdelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "sessionlistmodel.h"

#include "ElaTheme.h"

SessionListDelegate::SessionListDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{

}

SessionListDelegate::~SessionListDelegate() {}

void SessionListDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    QString name = index.data(SessionListModel::Name).toString();
    QPixmap avatar = qvariant_cast<QPixmap>(index.data(SessionListModel::Avatar));
    QString latestMsg = index.data(SessionListModel::LastestMsg).toString();
    QString latestDate = index.data(SessionListModel::LastestDate).toString();
    qint8 unreadCnt = index.data(SessionListModel::UnreadCnt).toInt();
    int margin = 12;

    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter->save();

    //文字
    QFont font = painter->font();
    font.setPixelSize(14);
    font.setBold(true);
    painter->setFont(font);

    //背景
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, ElaThemeColor(eTheme->getThemeMode(), BasicSelectedAlpha));
    } else if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, ElaThemeColor(eTheme->getThemeMode(), BasicSelectedHoverAlpha));
    } else {
        painter->fillRect(option.rect, Qt::transparent);
    }

    //头像
    int avatarSize = 48;
    QRect avatarRect(option.rect.left() + margin,
                     option.rect.top() + (option.rect.height() - avatarSize) / 2,
                     avatarSize,
                     avatarSize);

    if (!avatar.isNull()) {
        QPixmap pixmap = avatar.scaled(avatarSize, avatarSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPainterPath path;
        path.addEllipse(avatarRect);

        painter->setClipPath(path); //设置裁切区域
        painter->drawPixmap(avatarRect, pixmap);
        painter->restore();
    }
    else {
        painter->setPen(QPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep)));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(avatarRect.x(), avatarRect.y(), avatarSize, avatarSize);
        painter->drawText(avatarRect, Qt::AlignCenter, name.at(0));
    }

    //用户名
    QRect nameRect(avatarRect.right() + 10,
                   avatarRect.top(),
                   option.rect.width() - avatarRect.right() - 10 - 70 - 5,
                   20);
    if (!name.isEmpty()) {
        font.setBold(false);
        QFontMetrics fm (font);
        QString elideName = fm.elidedText(name, Qt::ElideRight, nameRect.width());

        painter->setFont(font);
        painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicText));
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, elideName);
    }

    //时间
    if (!latestDate.isEmpty()) {
        font.setPixelSize(10);
        painter->setFont(font);
        painter->setPen(QColor(128, 128, 128));
        QRect dateRect(option.rect.right() - margin - 70,
                       avatarRect.top(),
                       70,
                       16);
        painter->drawText(dateRect, Qt::AlignRight | Qt::AlignVCenter, latestDate);
    }

    //最新消息
    if (!latestMsg.isEmpty()) {
        font.setPixelSize(12);
        painter->setFont(font);
        painter->setPen(QColor(128, 128, 128));
        QRect msgRect(avatarRect.right() + 10,
                      avatarRect.top() + avatarSize - 20,
                      option.rect.width() - avatarRect.right() - 10 - margin - 23,
                      20);
        QFontMetrics fm (font);
        QString elideMsg = fm.elidedText(latestMsg, Qt::ElideRight, msgRect.width());

        painter->drawText(msgRect, Qt::AlignLeft | Qt::AlignVCenter, elideMsg);
    }

    //未读消息数
    if (unreadCnt > 0) {
        QString str;
        int size = 20;
        if (unreadCnt > 99) {
            str = "99+";
            size = 23;
        }
        else {
            str = QString::number(unreadCnt);
        }
        QRect unreadRect(option.rect.right() - margin - size,
                         avatarRect.top() + avatarSize - size,
                         size, size);

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawEllipse(unreadRect.x(), unreadRect.y(), size, size);

        font.setPixelSize(10);
        font.setBold(true);

        painter->setPen(Qt::white);
        painter->setFont(font);
        painter->drawText(unreadRect, Qt::AlignCenter, str);
    }
}

QSize SessionListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(150, 80);
}
