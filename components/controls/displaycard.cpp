#include "displaycard.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QRadialGradient>
#include <QtMath>

#include "ElaTheme.h"

DisplayCard::DisplayCard(QWidget *parent)
    : ElaPromotionCard{parent}
{
    setMouseTracking(true);
    _hoverGradient = new QRadialGradient();
    _hoverGradient->setRadius(170);
    _hoverGradient->setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 40));
    _hoverGradient->setColorAt(1, QColor(0xFF, 0xFF, 0xFF, 0));

    _pressGradient = new QRadialGradient();
    _pressGradient->setRadius(170);
    _pressGradient->setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 0));
    _pressGradient->setColorAt(1, QColor(0xFF, 0xFF, 0xFF, 40));
}

DisplayCard::~DisplayCard()
{
    delete _hoverGradient;
    delete _pressGradient;
}

qreal DisplayCard::hoverOpacity() const
{
    return _hoverOpacity;
}

void DisplayCard::setHoverOpacity(qreal opacity)
{
    _hoverOpacity = opacity;
}

qreal DisplayCard::pressOpacity() const
{
    return _pressOpacity;
}

void DisplayCard::setPressOpacity(qreal opacity)
{
    _pressOpacity = opacity;
}

qreal DisplayCard::pressRadius() const
{
    return _pressRadius;
}

void DisplayCard::setPressRadius(qreal radius)
{
    _pressRadius = radius;
}

qreal DisplayCard::_getLongestDistance(const QPoint& point)
{
    qreal topLeftDis = _distance(point, QPoint(0, 0));
    qreal topRightDis = _distance(point, QPoint(width(), 0));
    qreal bottomLeftDis = _distance(point, QPoint(0, height()));
    qreal bottomRightDis = _distance(point, QPoint(width(), height()));
    QList<qreal> disList{topLeftDis, topRightDis, bottomLeftDis, bottomRightDis};
    std::sort(disList.begin(), disList.end());
    return disList[3];
}

qreal DisplayCard::_distance(const QPoint& point1, const QPoint& point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) +
                     (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void DisplayCard::_startHoverOpacityAnimation(bool isVisible)
{
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "hoverOpacity");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    opacityAnimation->setDuration(250);
    opacityAnimation->setStartValue(_hoverOpacity);
    opacityAnimation->setEndValue(isVisible ? 1.0 : 0.0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

bool DisplayCard::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonPress:
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pressOpacity");
        connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
            _isPressAnimationFinished = true;
        });
        opacityAnimation->setDuration(300);
        opacityAnimation->setEasingCurve(QEasingCurve::InQuad);
        opacityAnimation->setStartValue(1);
        opacityAnimation->setEndValue(0);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* pressAnimation = new QPropertyAnimation(this, "pressRadius");
        connect(pressAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _pressGradient->setRadius(value.toReal());
        });
        pressAnimation->setDuration(300);
        pressAnimation->setEasingCurve(QEasingCurve::InQuad);
        pressAnimation->setStartValue(30);
        pressAnimation->setEndValue(_getLongestDistance(mouseEvent->pos()) * 1.8);
        pressAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _isPressAnimationFinished = false;
        _pressGradient->setFocalPoint(mouseEvent->pos());
        _pressGradient->setCenter(mouseEvent->pos());
        _startHoverOpacityAnimation(false);
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        Q_EMIT promotionCardClicked();
        break;
    }
    case QEvent::MouseMove:
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if (_hoverOpacity < 1 && _isPressAnimationFinished)
        {
            _startHoverOpacityAnimation(true);
        }
        if (_isPressAnimationFinished)
        {
            _hoverGradient->setCenter(mouseEvent->pos());
            _hoverGradient->setFocalPoint(mouseEvent->pos());
        }
        update();
        break;
    }
    case QEvent::Enter:
    {
        _startHoverOpacityAnimation(true);
        break;
    }
    case QEvent::Leave:
    {
        _startHoverOpacityAnimation(false);
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void DisplayCard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    eTheme->drawEffectShadow(&painter, rect(), _shadowBorderWidth, getBorderRadius());
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth,
                         width() - 2 * _shadowBorderWidth, height() - 2 * _shadowBorderWidth);
    QPainterPath path;
    path.addRoundedRect(foregroundRect, getBorderRadius(), getBorderRadius());
    painter.setClipPath(path);

    QPixmap pixmap = getCardPixmap();
    if (!pixmap.isNull()) {
        qreal pixmapRatio = (qreal)pixmap.width() / pixmap.height();
        qreal rectRatio = (qreal)foregroundRect.width() / foregroundRect.height();
        QRect sourceRect;
        if (pixmapRatio > rectRatio) {
            int srcWidth = pixmap.height() * rectRatio;
            int srcX = (pixmap.width() - srcWidth) / 2;
            sourceRect = QRect(srcX, 0, srcWidth, pixmap.height());
        } else {
            int srcHeight = pixmap.width() / rectRatio;
            int srcY = (pixmap.height() - srcHeight) / 2;
            sourceRect = QRect(0, srcY, pixmap.width(), srcHeight);
        }
        painter.drawPixmap(foregroundRect, pixmap, sourceRect);
    }

    painter.fillRect(foregroundRect, QColor(0, 0, 0, 40));

    painter.save();
    QFont font = painter.font();

    font.setWeight(QFont::Bold);
    font.setPixelSize(getCardTitlePixelSize());
    painter.setFont(font);
    painter.setPen(getCardTitleColor());
    painter.drawText(QRect(25, 25, foregroundRect.width() - 25, foregroundRect.height()),
                     Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, getCardTitle());

    font.setWeight(QFont::Bold);
    font.setPixelSize(getTitlePixelSize());
    painter.setFont(font);
    painter.setPen(getTitleColor());
    int titleTextHeight = painter.fontMetrics().height();
    QRect titleRect(25, (height() - titleTextHeight) / 2, foregroundRect.width() - 25, titleTextHeight);
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignBottom | Qt::TextSingleLine, getTitle());

    QString promotionTitle = getPromotionTitle();
    if (!promotionTitle.isEmpty()) {
        font.setWeight(QFont::Normal);
        font.setPixelSize(getPromotionTitlePixelSize());
        painter.setFont(font);
        int promotionTitleTextWidth = painter.fontMetrics().horizontalAdvance(promotionTitle);
        int promotionTitleTextHeight = painter.fontMetrics().height();
        QRect promotionTitleTextRect(32, titleRect.top() - promotionTitleTextHeight - 5,
                                     foregroundRect.width() / 2 - 25, promotionTitleTextHeight);
        painter.setPen(Qt::NoPen);
        painter.setBrush(getPromotionTitleBaseColor());
        painter.drawRoundedRect(QRect(25, promotionTitleTextRect.top() - 2,
                                       promotionTitleTextWidth + 14, promotionTitleTextHeight + 4), 8, 8);
        painter.setPen(getPromotionTitleColor());
        painter.drawText(promotionTitleTextRect, Qt::AlignLeft | Qt::AlignBottom | Qt::TextSingleLine,
                         promotionTitle);
    }

    font.setWeight(QFont::Medium);
    font.setPixelSize(getSubTitlePixelSize());
    painter.setFont(font);
    painter.setPen(getSubTitleColor());
    painter.drawText(QRect(25, titleRect.bottom(), foregroundRect.width() / 2 - 25, height() / 2),
                     Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, getSubTitle());

    painter.restore();

    if (_isPressAnimationFinished) {
        painter.setOpacity(_hoverOpacity);
        painter.setBrush(*_hoverGradient);
        painter.drawEllipse(_hoverGradient->center(), _hoverGradient->radius(), _hoverGradient->radius());
    } else {
        painter.setOpacity(_pressOpacity);
        painter.setBrush(*_pressGradient);
        painter.drawEllipse(_pressGradient->center(), _pressRadius, _pressRadius / 1.1);
    }

    painter.restore();
}
