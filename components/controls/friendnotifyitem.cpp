#include "friendnotifyitem.h"

#include <QHBoxLayout>
#include <QPainter>

#include "ElaImageCard.h"
#include "ElaPushButton.h"
#include "ElaTheme.h"

FriendNotifyItem::FriendNotifyItem(const FriendNotifyData& data, QWidget *parent)
    : QWidget{parent}
    , _data(data)
    , _agreeButton(nullptr)
    , _refuseButton(nullptr)
    , _statusText(nullptr)
{
    initContent();
}

FriendNotifyData FriendNotifyItem::getNotifyData() const
{
    return _data;
}

void FriendNotifyItem::setStatusText(FriendNotifyStatus status)
{
    if (!_statusText) {
        return;
    }

    if (_data.status == FriendNotifyStatus::Passed) {
        _statusText->setText("已同意");
    }
    else if (_data.status == FriendNotifyStatus::NotPassed) {
        _statusText->setText("未同意");
    }
    else if (_data.status == FriendNotifyStatus::WaitingPassed) {
        _statusText->setText("等待验证");
    }
}

void FriendNotifyItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect selfRect = this->rect();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(eTheme->getThemeMode(), BasicBase));
    painter.drawRoundedRect(selfRect, 8, 8);

    if (_data.avatar.isNull()) {
        QRect avatarRect(selfRect.left() + 20,
                         selfRect.top() + (this->height() - 40) / 2,
                         40, 40);
        painter.setPen(QPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep)));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(avatarRect);

        QFont f;
        f.setPixelSize(13);
        painter.setFont(f);
        painter.drawText(avatarRect, Qt::AlignCenter, _data.name.at(0));
    }

    QWidget::paintEvent(event);
}

QSize FriendNotifyItem::sizeHint() const
{
    return QSize(300, 80);
}

void FriendNotifyItem::initContent()
{
    int pixelSize = 13;
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    QImage avatarImg;
    if (!_data.avatar.isNull()) {
        avatarImg = _data.avatar.toImage();
    }
    ElaImageCard* avatar = new ElaImageCard(this);
    avatar->setCardImage(avatarImg);
    avatar->setFixedSize(40, 40);
    avatar->setBorderRadius(40);

    IconText* name = new IconText(_data.name, this);
    name->setTextColor(QColor("#118df0"));
    name->setPixelSize(pixelSize);
    IconText* isSelfText = new IconText(this);
    isSelfText->setPixelSize(pixelSize);
    isSelfText->setTextColorDark(Qt::white);
    isSelfText->setTextColorLight(Qt::black);

    IconText* date = new IconText(_data.date, this);
    date->setPixelSize(pixelSize - 2);

    IconText* msg = new IconText("留言: " + _data.msg, this);
    msg->setPixelSize(pixelSize);

    QWidget* subWid = new QWidget(this);
    QVBoxLayout* subWidLayout = new QVBoxLayout(subWid);

    QWidget* subWid2 = new QWidget(this);
    QHBoxLayout* subWid2Layout = new QHBoxLayout(subWid2);
    subWid2Layout->setContentsMargins(0, 0, 0, 0);
    subWid2Layout->setSpacing(5);
    subWid2Layout->addWidget(name);
    subWid2Layout->addWidget(isSelfText);
    subWid2Layout->addWidget(date, 2, Qt::AlignVCenter);
    subWid2->setLayout(subWid2Layout);

    subWidLayout->setContentsMargins(0, 0, 0, 0);
    subWidLayout->setSpacing(0);
    subWidLayout->addWidget(subWid2);
    subWidLayout->addStretch();
    subWidLayout->addWidget(msg);
    subWid->setFixedHeight(avatar->height());
    subWid->setLayout(subWidLayout);

    _agreeButton = new ElaPushButton("同意", this);
    _refuseButton = new ElaPushButton("拒绝", this);
    QFont f;
    f.setPixelSize(pixelSize);
    _agreeButton->setFixedSize(45, 30);
    _agreeButton->setFont(f);
    _refuseButton->setFixedSize(_agreeButton->size());
    _refuseButton->setFont(f);

    _statusText = new IconText(this);
    _statusText->setPixelSize(pixelSize);
    this->setStatusText(_data.status);

    if (_data.isSelf) {
        this->setButtonHidden();
        isSelfText->setText("正在验证你的申请");
    }
    else {
        _statusText->setHidden(true);
        isSelfText->setText("请求添加你为好友");
    }

    connect(_agreeButton, &ElaPushButton::clicked,
            this, &FriendNotifyItem::slotAgreeButtonClicked);
    connect(_refuseButton, &ElaPushButton::clicked,
            this, &FriendNotifyItem::slotRefuseButtonClicked);

    QWidget* buttonWid = new QWidget(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonWid);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(7);
    buttonLayout->addStretch();
    buttonLayout->addWidget(_agreeButton);
    buttonLayout->addWidget(_refuseButton);
    buttonLayout->addWidget(_statusText);
    buttonWid->setLayout(buttonLayout);

    mainLayout->setContentsMargins(20, 0, 20, 0);
    mainLayout->setSpacing(7);
    mainLayout->addWidget(avatar, 0, Qt::AlignVCenter);
    mainLayout->addWidget(subWid, 1, Qt::AlignVCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonWid, 3, Qt::AlignVCenter);
    this->setLayout(mainLayout);
}

void FriendNotifyItem::setButtonHidden()
{
    if (_agreeButton && _refuseButton) {
        _agreeButton->setHidden(true);
        _refuseButton->setHidden(true);
    }
}

void FriendNotifyItem::slotAgreeButtonClicked()
{
    setButtonHidden();
    _statusText->setHidden(false);
    _data.status = FriendNotifyStatus::Passed;
    this->setStatusText(_data.status);

    emit sigAgreeButtonClicked();
}

void FriendNotifyItem::slotRefuseButtonClicked()
{
    setButtonHidden();
    _statusText->setHidden(false);
    _data.status = FriendNotifyStatus::NotPassed;
    this->setStatusText(_data.status);

    emit sigRefuseButtonClicked();
}
