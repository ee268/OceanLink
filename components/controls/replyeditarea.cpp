#include "replyeditarea.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QMouseEvent>

#include "avatarwidget.h"
#include "ElaTheme.h"

ReplyEditArea::ReplyEditArea(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(100);
    setHidden(true);
    _isShow = false;

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    AvatarWidget* avatar = new AvatarWidget(this);
    avatar->setFixedSize(40, 40);
    avatar->setPixeSize(15);

    _replyEdit = new ElaLineEdit(this);
    _replyEdit->setPlaceholderText("写下你的回复...");

    _sendBtn = new ThemeColorButton("发送", this);
    _sendBtn->setBorderRadius(8);
    _sendBtn->setFixedWidth(80);

    connect(_sendBtn, &ThemeColorButton::clicked, this, &ReplyEditArea::slotSendBtnClicked);

    layout->addWidget(avatar);
    layout->addWidget(_replyEdit);
    layout->addWidget(_sendBtn);
    setLayout(layout);

    _slideAnimation = new QPropertyAnimation(this, "geometry");
    _slideAnimation->setDuration(300);
    _slideAnimation->setEasingCurve(QEasingCurve::OutCubic);

    if (parent) {
        parent->installEventFilter(this);
    }
}

void ReplyEditArea::showAnimation()
{
    if (_isShow) {
        return;
    }

    QRect parentRect = parentWidget()->rect();
    QRect startRect(parentRect.left(), parentRect.bottom(),
                    parentRect.width(), height());
    QRect endRect(parentRect.left(), parentRect.bottom() - height(),
                  parentRect.width(), height());

    _slideAnimation->setStartValue(startRect);
    _slideAnimation->setEndValue(endRect);

    setHidden(false);
    _slideAnimation->start();
    _isShow = true;
}

void ReplyEditArea::hideAnimation()
{
    QRect parentRect = parentWidget()->rect();
    QRect startRect = geometry();
    QRect endRect(parentRect.left(), parentRect.bottom(),
                  parentRect.width(), height());

    _slideAnimation->setStartValue(startRect);
    _slideAnimation->setEndValue(endRect);
    _slideAnimation->start();
    _isShow = false;
}

void ReplyEditArea::setReplyCommentData(std::shared_ptr<ReplyCommentData> data)
{
    _data = data;
}

bool ReplyEditArea::getIsShow() const
{
    return _isShow;
}

void ReplyEditArea::setIsShow(bool isShow)
{
    _isShow = isShow;
}

void ReplyEditArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    int shadowBorderWidth = 6;
    int borderRadius = 8;

    eTheme->drawEffectShadow(&painter, rect(), shadowBorderWidth, borderRadius);

    QRect foregroundRect(shadowBorderWidth, shadowBorderWidth,
                         width() - 2 * shadowBorderWidth, height() - 2 * shadowBorderWidth);
    painter.setBrush(ElaThemeColor(eTheme->getThemeMode(), BasicBase));
    painter.drawRoundedRect(foregroundRect, borderRadius, borderRadius);

    QWidget::paintEvent(event);
}

bool ReplyEditArea::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == parentWidget() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if (!geometry().contains(mouseEvent->pos())) {
            hideAnimation();
        }
    }

    return QWidget::eventFilter(obj, event);
}

void ReplyEditArea::slotSendBtnClicked()
{
    QString text = _replyEdit->text();
    if (text.isEmpty() || !_data) {
        return;
    }

    _replyEdit->clear();
    emit sigSendReplyBtnClicked(_data, text);
    hideAnimation();
}
