#include "collectlistdelegate.h"

#include <QPainter>
#include <QFontMetrics>

#include "ElaTheme.h"

#include "collectlistmodel.h"

CollectListDelegate::CollectListDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
}

CollectListDelegate::~CollectListDelegate()
{
}

void CollectListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, ElaThemeColor(eTheme->getThemeMode(), BasicBaseDeepAlpha));
    }

    QPixmap cover = index.data(CollectListModel::Cover).value<QPixmap>();
    QString content = index.data(CollectListModel::Content).toString();
    QString date = index.data(CollectListModel::Date).toString();
    QString sourceName = "来自: " + index.data(CollectListModel::SourceName).toString();

    QRect itemRect(option.rect.left(), option.rect.top(),
                   option.rect.width(), option.rect.height() - 1);
    int margin = 20;
    int coverSize = itemRect.height() - 2 * margin;
    int spacing = 10;

    //分隔线
    QRect line(option.rect.left() + margin, option.rect.bottom() - 1,
               option.rect.width() - margin * 2, 1);
    painter->fillRect(line, ElaThemeColor(eTheme->getThemeMode(), BasicBorder));

    //cover + content
    QRect coverRect(itemRect.left() + margin,
                    itemRect.top() + (itemRect.height() - coverSize) / 2,
                    coverSize, coverSize);

    //cover
    if (!cover.isNull()) {
        QPixmap scaledCover = cover.scaled(coverSize, coverSize,
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation);
        int x = coverRect.left() + (coverSize - scaledCover.width()) / 2;
        int y = coverRect.top() + (coverSize - scaledCover.height()) / 2;
        painter->drawPixmap(x, y, scaledCover);
    }

    //content
    int contentX = coverRect.right() + spacing;
    if (cover.isNull())
        contentX = margin;
    int contentWidth = itemRect.width() / 2 - margin - coverSize - spacing;
    QRect contentRect(contentX, itemRect.top(), contentWidth, itemRect.height());

    painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicText));
    QFont contentFont = painter->font();
    contentFont.setPixelSize(13);
    painter->setFont(contentFont);

    QFontMetrics fm(contentFont);
    QString elidedText = fm.elidedText(content, Qt::ElideRight, contentWidth);
    painter->drawText(contentRect, Qt::AlignVCenter | Qt::TextSingleLine, elidedText);

    //date + sourceName
    int rightWidth = itemRect.width() / 2 - margin;
    QRect rightRect(itemRect.right() - margin - rightWidth, itemRect.top(),
                    rightWidth, itemRect.height());

    painter->setPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep));
    QFont dateFont = painter->font();
    dateFont.setPixelSize(11);
    painter->setFont(dateFont);

    QFontMetrics dateFm(dateFont);
    int dateHeight = dateFm.height();

    //date
    QRect dateRect(rightRect.left(), rightRect.top() + (rightRect.height() - dateHeight * 2 - spacing) / 2,
                   rightRect.width(), dateHeight);
    QString elidedDate = dateFm.elidedText(date, Qt::ElideRight, rightRect.width());
    painter->drawText(dateRect, Qt::AlignRight | Qt::AlignVCenter, elidedDate);

    //sourceName
    QRect sourceRect(rightRect.left(), dateRect.bottom() + spacing,
                     rightRect.width(), dateHeight);
    QString elidedSource = dateFm.elidedText(sourceName, Qt::ElideRight, rightRect.width());
    painter->drawText(sourceRect, Qt::AlignRight | Qt::AlignVCenter, elidedSource);

    painter->restore();
}

QSize CollectListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(200, 100);
}
