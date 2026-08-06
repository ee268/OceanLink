#include "codepage.h"

#include <QVBoxLayout>

#include "ElaIcon.h"

CodePage::CodePage(QWidget *parent)
    : QWidget(parent)
    , _countdown_timer(new QTimer(this))
    , _countdown(180)
{
    initContent();
}

void CodePage::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    int width = 240;

    _verifyCodeText = new IconText(this);
    _verifyCodeText->setText("正在发送验证码");
    _verifyCodeText->setPixelSize(12);
    _verifyCodeText->setTextColor(QColor("#1a6bf8"));

    QWidget* subWid = new QWidget(this);
    QHBoxLayout* subLayout = new QHBoxLayout(subWid);
    subLayout->setContentsMargins(0, 0, 0, 0);
    subLayout->setSpacing(8);

    _codeEdit = new ElaLineEdit(subWid);
    _codeEdit->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::ShieldCheck),
                         QLineEdit::LeadingPosition);
    _codeEdit->setPlaceholderText("输入验证码");
    _codeEdit->setIsClearButtonEnable(false);
    _codeEdit->setFixedWidth((width - 8) * 0.7);

    _sendCodeButton = new ThemeColorButton("重新发送", subWid);\
        _sendCodeButton->setBorderRadius(8);
    _sendCodeButton->setFixedWidth((width - 8) * 0.3);
    QFont f = _sendCodeButton->font();
    f.setPixelSize(13);
    _sendCodeButton->setFont(f);
    _sendCodeButton->setDisabled(true);

    connect(_sendCodeButton, &ThemeColorButton::clicked, this, &CodePage::sigSendBtnClicked);

    connect(_countdown_timer, &QTimer::timeout, this, [this](){
        if (_countdown == 0) {
            stopCountdown();
            return;
        }

        _sendCodeButton->setText(QString::number(_countdown));
        _countdown--;
    });

    subLayout->addWidget(_codeEdit);
    subLayout->addWidget(_sendCodeButton);
    subWid->setLayout(subLayout);

    _confirmButton = new ThemeColorButton("确认", this);
    _confirmButton->setFixedWidth(width);
    _confirmButton->setBorderRadius(8);

    connect(_confirmButton, &ThemeColorButton::clicked, this, &CodePage::sigConfirmRegBtnClicked);

    _backBtn = new ElaPushButton("返回", this);
    _backBtn->setFixedWidth(width);
    _backBtn->setBorderRadius(8);

    connect(_backBtn, &ElaPushButton::clicked, this, &CodePage::sigBackBtnClicked);

    mainLayout->addStretch(1);
    mainLayout->addWidget(_verifyCodeText, 0, Qt::AlignHCenter);
    mainLayout->addWidget(subWid, 0, Qt::AlignHCenter);
    mainLayout->addStretch(1);
    mainLayout->addWidget(_confirmButton, 0, Qt::AlignHCenter);
    mainLayout->addWidget(_backBtn, 0, Qt::AlignHCenter);
    mainLayout->addStretch(1);
    this->setLayout(mainLayout);
}

void CodePage::startCountdown()
{
    _sendCodeButton->setDisabled(true);
    _countdown = 180;
    _countdown_timer->start(1000);
}

void CodePage::stopCountdown()
{
    _countdown_timer->stop();
    _sendCodeButton->setDisabled(false);
    _sendCodeButton->setText("重新发送");
}
