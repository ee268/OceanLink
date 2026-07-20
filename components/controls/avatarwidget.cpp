#include "avatarwidget.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"

AvatarWidget::AvatarWidget(QWidget *parent)
    : QWidget(parent)
    , _name("")
    , _namePixelSize(18)
{

}

void AvatarWidget::setAvatar(const QPixmap &pixmap)
{
    _avatar = pixmap;
    update();
}

void AvatarWidget::setName(const QString &name)
{
    _name = name;
    update();
}

void AvatarWidget::setPixeSize(int size)
{
    _namePixelSize = size;
    update();
}

void AvatarWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    if (_avatar.isNull()) {
        painter.setPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(this->rect().adjusted(1, 1, -1, -1));

        QFont f;
        f.setBold(true);
        f.setPixelSize(_namePixelSize);
        painter.setFont(f);
        QString name = "?";
        if (!_name.isEmpty()) {
            name = _name.at(0);
        }
        painter.drawText(this->rect(), Qt::AlignCenter, name);
    }
    else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);
        QPainterPath path;
        path.addEllipse(this->rect());
        painter.setClipPath(path);
        painter.drawPixmap(this->rect(), _avatar);
    }
}
