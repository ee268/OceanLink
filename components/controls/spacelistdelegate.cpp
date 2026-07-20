#include "spacelistdelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "spacelistmodel.h"
#include "ElaTheme.h"

SpaceListDelegate::SpaceListDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

void SpaceListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    QString name = index.data(SpaceListModel::Name).toString();
    QPixmap avatar = qvariant_cast<QPixmap>(index.data(SpaceListModel::Avatar));
    QString latestTitle = index.data(SpaceListModel::LatestTitle).toString();
    QString account = index.data(SpaceListModel::Account).toString();

    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    QFont font = painter->font();
    font.setPixelSize(16);
    font.setBold(true);
    painter->setFont(font);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, ElaThemeColor(eTheme->getThemeMode(), BasicSelectedAlpha));
    } else if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, ElaThemeColor(eTheme->getThemeMode(), BasicSelectedHoverAlpha));
    } else {
        painter->fillRect(option.rect, Qt::transparent);
    }

    int spacing = 10;

    QRect avatarRect(option.rect.left() + _margin,
                     option.rect.top() + (option.rect.height() - _avatarSize) / 2,
                     _avatarSize, _avatarSize);

    if (!avatar.isNull()) {
        QPixmap pixmap = avatar.scaled(_avatarSize, _avatarSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPainterPath path;
        path.addEllipse(avatarRect);
        painter->save();
        painter->setClipPath(path);
        painter->drawPixmap(avatarRect, pixmap);
        painter->restore();
    } else {
        painter->setPen(QPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep)));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(avatarRect);
        painter->drawText(avatarRect, Qt::AlignCenter, name.at(0));
    }

    QRect nameRect(avatarRect.right() + spacing,
                   avatarRect.top(),
                   option.rect.width() - avatarRect.right() - spacing - _margin,
                   20);
    if (!name.isEmpty()) {
        font.setBold(false);
        QFontMetrics fm(font);
        QString elideName = fm.elidedText(name, Qt::ElideRight, nameRect.width());
        painter->setFont(font);
        painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicText));
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, elideName);
    }

    if (!latestTitle.isEmpty()) {
        font.setPixelSize(12);
        painter->setFont(font);
        painter->setPen(QColor(128, 128, 128));
        QRect msgRect(avatarRect.right() + spacing,
                      avatarRect.top() + _avatarSize - 20,
                      option.rect.width() - avatarRect.right() - spacing - _margin,
                      20);
        QFontMetrics fm(font);
        QString elideMsg = fm.elidedText(latestTitle, Qt::ElideRight, msgRect.width());
        painter->drawText(msgRect, Qt::AlignLeft | Qt::AlignVCenter, elideMsg);
    }
}

QSize SpaceListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(150, 80);
}
