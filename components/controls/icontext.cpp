#include "icontext.h"

#include <QPainter>

IconText::IconText(QWidget *parent/* = nullptr*/)
    : QLabel(parent)
    , _spacing(4)
{
    initText();
}

IconText::IconText(const QString &text, QWidget *parent/* = nullptr*/)
    : QLabel(text, parent)
    , _spacing(4)
{
    initText();
}

IconText::~IconText()
{

}

void IconText::setPixelSize(int size)
{
    QFont f = this->font();
    f.setPixelSize(size);
    this->setFont(f);
}

void IconText::setTextColor(QColor color)
{
    disconnect(eTheme, &ElaTheme::themeModeChanged, this, &IconText::slotChangedTheme);

    _textColor = color;
    update();
}

void IconText::setTextBold(bool enable)
{
    QFont f(this->font());
    f.setBold(enable);
    this->setFont(f);
}

void IconText::setSpacing(int spacing)
{
    _spacing = spacing;
    update();
}

void IconText::setTextColorDark(const QColor &color)
{
    _textColorDark = color;
}

void IconText::setTextColorLight(const QColor &color)
{
    _textColorLight = color;
}

void IconText::setIcon(const QIcon &icon)
{
    _icon = icon;
    update();
}

QSize IconText::sizeHint() const
{
    QFontMetrics fm(font());
    return QSize(fm.horizontalAdvance(text()), fm.height());
}

QSize IconText::minimumSizeHint() const
{
    return sizeHint();
}

void IconText::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    if (eTheme->getThemeMode() == ElaThemeType::Light) {
        painter.setPen(QPen(_textColorLight));
    }
    else {
        painter.setPen(QPen(_textColorDark));
    }

    if (_textColor.isValid()) {
        painter.setPen(QPen(_textColor));
    }

    if (!_icon.isNull()) {
        QFontMetrics fm(this->font());
        int size = fm.height();
        QPixmap pixmap = _icon.pixmap(size, size);
        QPainter pmPainter(&pixmap);
        pmPainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        if (_textColor.isValid()) {
            pmPainter.fillRect(pixmap.rect(), _textColor);
        }
        else {
            pmPainter.fillRect(pixmap.rect(), ElaThemeColor(eTheme->getThemeMode(), BasicText));
        }
        pmPainter.end();

        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter.drawPixmap(0, 0, pixmap);

        painter.drawText(size + _spacing, 0,
                         this->width() - size - _spacing, this->height(),
                         Qt::AlignVCenter, this->text());
        this->setFixedWidth(fm.horizontalAdvance(this->text()) + size + _spacing);

        return;
    }

    painter.drawText(0, 0,
                     this->width(), this->height(),
                     Qt::AlignVCenter, this->text());
}

void IconText::initText()
{
    setWordWrap(false);
    setPixelSize(13);
    setTextColorLight(QColor(121, 113, 123));
    setTextColorDark(QColor(141, 133, 143));

    connect(eTheme, &ElaTheme::themeModeChanged, this, &IconText::slotChangedTheme);

    QFontMetrics fm(font());
    setFixedHeight(fm.height());
}

void IconText::slotChangedTheme(ElaThemeType::ThemeMode mode)
{
    update();
}
