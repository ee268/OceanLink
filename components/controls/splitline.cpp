#include "splitline.h"

#include "ElaTheme.h"

#include <QPainter>

SplitLine::SplitLine(QWidget *parent)
    : QPushButton{parent}
{
    setFlat(true);
    setEnabled(false);
}

void SplitLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = this->rect();
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.fillRect(rect, ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep));
}
