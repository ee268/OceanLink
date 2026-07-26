#include "borderwidget.h"

#include <QPainter>
#include <QHBoxLayout>

#include "ElaTheme.h"

BorderWidget::BorderWidget(QWidget *parent)
    : QWidget{parent}
    , _borderRadius(8)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    this->setLayout(mainLayout);
    this->setFixedHeight(60);
}

void BorderWidget::setBorderRadius(int radius)
{
    _borderRadius = radius;
}

QHBoxLayout *BorderWidget::layout() const
{
    auto ly = dynamic_cast<QHBoxLayout*>(QWidget::layout());
    return ly;
}

void BorderWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    ElaThemeType::ThemeMode mode = eTheme->getThemeMode();
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(ElaThemeColor(mode, BasicBorder), 1));
    painter.setBrush(ElaThemeColor(mode, BasicBase));

    painter.drawRoundedRect(this->rect().adjusted(1, 1, -1, -1),
                            _borderRadius, _borderRadius);

    QWidget::paintEvent(event);
}
