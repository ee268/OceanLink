#include "contactlistdelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "contactlistmodel.h"
#include "ElaTheme.h"

ContactListDelegate::ContactListDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

void ContactListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    bool isGroup = index.data(ContactListModel::IsGroup).toBool();

    if (isGroup) {
        paintGroupItem(painter, option, index);
    } else {
        paintContactItem(painter, option, index);
    }
}

QSize ContactListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    Q_UNUSED(option);
    bool isGroup = index.data(ContactListModel::IsGroup).toBool();

    if (isGroup) {
        return QSize(200, _groupHeight);
    }
    return QSize(200, _contactHeight);
}

void ContactListDelegate::paintGroupItem(QPainter *painter, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, ElaThemeColor(eTheme->getThemeMode(), BasicHoverAlpha));
    }

    QString name = index.data(ContactListModel::Name).toString();


    //绘制分组名称
    QRect textRect(option.rect.left() + _leftPadding + _spacing,
                   option.rect.top(),
                   option.rect.width() - _leftPadding - _spacing,
                   option.rect.height());

    QFont f;
    f.setPixelSize(14);
    f.setBold(true);
    painter->setFont(f);
    painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicText));
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, name);

    painter->restore();
}

void ContactListDelegate::paintContactItem(QPainter *painter, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, ElaThemeColor(eTheme->getThemeMode(), BasicSelectedAlpha));
    } else if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, ElaThemeColor(eTheme->getThemeMode(), BasicSelectedHoverAlpha));
    } else {
        painter->fillRect(option.rect, Qt::transparent);
    }

    QString name = index.data(ContactListModel::Name).toString();
    QString sign = index.data(ContactListModel::Sign).toString();
    bool status = index.data(ContactListModel::Status).toBool();
    QPixmap avatar = qvariant_cast<QPixmap>(index.data(ContactListModel::Avatar));

    //绘制头像
    QRect avatarRect(option.rect.left() + _leftPadding,
                     option.rect.top() + (option.rect.height() - _avatarSize) / 2,
                     _avatarSize, _avatarSize);

    if (!avatar.isNull()) {
        QPixmap scaledAvatar = avatar.scaled(_avatarSize, _avatarSize,
                                             Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        QPixmap circleAvatar(_avatarSize, _avatarSize);
        circleAvatar.fill(Qt::transparent);

        QPainterPath path;
        path.addEllipse(0, 0, _avatarSize, _avatarSize);

        QPainter avatarPainter(&circleAvatar);
        avatarPainter.setRenderHint(QPainter::Antialiasing);
        avatarPainter.setClipPath(path);
        avatarPainter.drawPixmap(0, 0, scaledAvatar);

        painter->drawPixmap(avatarRect, circleAvatar);
    } else {
        QFont f;
        f.setPixelSize(16);
        painter->setFont(f);
        painter->setPen(QPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep)));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(avatarRect.x(), avatarRect.y(), _avatarSize, _avatarSize);
        painter->drawText(avatarRect, Qt::AlignCenter, name.at(0));
    }
    painter->restore();

    //绘制名字
    QRect nameRect(avatarRect.right() + _spacing,
                   option.rect.top() + 8,
                   option.rect.width() - avatarRect.right() - _spacing - _leftPadding,
                   20);

    QFont nameFont;
    nameFont.setPixelSize(14);
    painter->setFont(nameFont);
    painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicText));
    painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, name);

    //绘制状态
    QRect statusRect(avatarRect.right() + _spacing,
                     nameRect.bottom() + 8,
                     8, 8);

    painter->setPen(Qt::NoPen);
    painter->setBrush(status ? QColor("#2be98c") : QColor("#b8bbc6"));
    painter->drawEllipse(statusRect);

    //绘制状态文本
    QFont statusTextFont;
    statusTextFont.setPixelSize(12);
    QFontMetrics fm(statusTextFont);
    QString statusStr = status ? "在线" : "离线";

    QRect statusTextRect(statusRect.right() + 5,
                         nameRect.bottom() + 4,
                         fm.horizontalAdvance(statusStr),
                         16);

    painter->setFont(statusTextFont);
    painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicTextNoFocus));
    painter->drawText(statusTextRect, Qt::AlignLeft | Qt::AlignVCenter, statusStr);

    //分隔线
    painter->setPen(QPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep), 1));
    painter->drawLine(
        statusTextRect.right() + 5,
        statusTextRect.top(),
        statusTextRect.right() + 5,
        statusTextRect.bottom());

    //绘制签名
    QRect signRect(statusTextRect.right() + 11,
                   nameRect.bottom() + 4,
                   nameRect.width() - 13,
                   16);

    QFont signFont;
    signFont.setPixelSize(12);
    painter->setFont(signFont);
    painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicTextNoFocus));
    painter->drawText(signRect, Qt::AlignLeft | Qt::AlignVCenter, sign);
}
