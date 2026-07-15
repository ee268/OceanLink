#include "iconbutton.h"

#include <QEvent>
#include <QPainter>
#include <QPaintEvent>

#include "ElaTheme.h"

IconButton::IconButton(QIcon icon, QWidget *parent)
    : QPushButton(parent)
    , _borderRadius(6)
    , _icon(icon)
    , _iconSize(16)
    , _state(Normal)
    , _borderWidth(1)
    , _enableTransparent(false)
{
    installEventFilter(this);
}

void IconButton::setBorderRadius(int borderRadius)
{
    _borderRadius = borderRadius;
    update();
}

void IconButton::setIconSize(int size)
{
    _iconSize = size;
    update();
}

void IconButton::setBorderWidth(int width)
{
    _borderWidth = width;
    update();
}

void IconButton::setTransparentBackground(bool enable)
{
    _enableTransparent = enable;
    update();
}

bool IconButton::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == this) {
        if (event->type() == QEvent::Enter) {
            this->setCursor(Qt::PointingHandCursor);
            _state = Hover;
            update();
        }
        else if (event->type() == QEvent::Leave) {
            this->unsetCursor();
            _state = Normal;
            update();
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                _state = Pressed;
                update();
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            _state = Hover;
            update();
        }
    }

    return QPushButton::eventFilter(obj, event);
}

void IconButton::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    ElaThemeType::ThemeMode mode = eTheme->getThemeMode();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    painter.setPen(QPen(ElaThemeColor(mode, BasicBorder), _borderWidth));

    switch (_state) {
    case Hover:
        painter.setBrush(QBrush(ElaThemeColor(mode, BasicHover)));
        break;
    case Pressed:
        painter.setBrush(QBrush(ElaThemeColor(mode, BasicHoverAlpha)));
        break;
    case Normal:
    default:
        painter.setBrush(QBrush(ElaThemeColor(mode, BasicBase)));
        break;
    }

    if (_enableTransparent) {
        painter.setBrush(Qt::transparent);
    }

    painter.drawRoundedRect(
        this->rect().adjusted(_borderWidth, _borderWidth, -_borderWidth, -_borderWidth)
        , _borderRadius, _borderRadius);

    QPixmap pixmap = _icon.pixmap(_iconSize, _iconSize);
    QPainter pmPainter(&pixmap);
    pmPainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    pmPainter.fillRect(pixmap.rect(), ElaThemeColor(mode, BasicText));
    pmPainter.end();

    painter.drawPixmap((this->width() - _iconSize) / 2,
                       (this->height() - _iconSize) / 2,
                       pixmap);
}
