#ifndef DISPLAYCARD_H
#define DISPLAYCARD_H

#include "ElaPromotionCard.h"

class QRadialGradient;

class DisplayCard : public ElaPromotionCard
{
    Q_OBJECT
    Q_PROPERTY(qreal hoverOpacity READ hoverOpacity WRITE setHoverOpacity)
    Q_PROPERTY(qreal pressOpacity READ pressOpacity WRITE setPressOpacity)
    Q_PROPERTY(qreal pressRadius READ pressRadius WRITE setPressRadius)
public:
    explicit DisplayCard(QWidget *parent = nullptr);
    ~DisplayCard();

    qreal hoverOpacity() const;
    void setHoverOpacity(qreal opacity);
    qreal pressOpacity() const;
    void setPressOpacity(qreal opacity);
    qreal pressRadius() const;
    void setPressRadius(qreal radius);

protected:
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;

private:
    int _shadowBorderWidth = 6;
    qreal _hoverOpacity = 0;
    qreal _pressOpacity = 0;
    qreal _pressRadius = 0;
    bool _isPressAnimationFinished = true;
    QRadialGradient* _hoverGradient = nullptr;
    QRadialGradient* _pressGradient = nullptr;

    qreal _getLongestDistance(const QPoint& point);
    qreal _distance(const QPoint& point1, const QPoint& point2);
    void _startHoverOpacityAnimation(bool isVisible);
};

#endif // DISPLAYCARD_H
