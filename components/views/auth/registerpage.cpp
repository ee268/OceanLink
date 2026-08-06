#include "registerpage.h"

#include <QVBoxLayout>
#include <QRegularExpression>

#include "../../controls/themecolorbutton.h"

#include "ElaIcon.h"

RegisterPage::RegisterPage(QWidget *parent)
    : QWidget(parent)
{
    initContent();
}

void RegisterPage::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    int width = 240;

    _registerText = new IconText(this);
    _registerText->setText("创建您的账户，开启高效沟通之旅");
    _registerText->setPixelSize(12);
    _registerText->setTextColorDark(Qt::white);
    _registerText->setTextColorLight(Qt::black);

    //昵称输入框
    _username_edit = new TipLineEdit(this);
    _username_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::User),
                                             QLineEdit::LeadingPosition);
    _username_edit->getLineEdit()->setPlaceholderText("昵称");
    _username_edit->getLineEdit()->setFixedWidth(width);
    _username_edit->setTipText("至少1个字符，不能超过12个字符");

    //密码输入框
    _register_pwd_edit = new TipLineEdit(this);
    _register_pwd_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::LockKeyhole),
                                                 QLineEdit::LeadingPosition);
    _register_pwd_edit->getLineEdit()->setPlaceholderText("设置密码");
    _register_pwd_edit->getLineEdit()->setIsClearButtonEnable(false);
    _register_pwd_edit->getLineEdit()->setFixedWidth(width);
    _register_pwd_edit->setTipText("8到12位，必须包含大小写字母、数字、特殊字符");
    _register_pwd_edit->setEchoModePassword();

    _confirm_pwd_edit = new TipLineEdit(this);
    _confirm_pwd_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::LockKeyhole),
                                                QLineEdit::LeadingPosition);
    _confirm_pwd_edit->getLineEdit()->setPlaceholderText("确认密码");
    _confirm_pwd_edit->getLineEdit()->setIsClearButtonEnable(false);
    _confirm_pwd_edit->getLineEdit()->setFixedWidth(width);
    _confirm_pwd_edit->setTipText("请再次输入密码");
    _confirm_pwd_edit->setEchoModePassword();

    //邮箱输入框
    _email_edit = new TipLineEdit(this);
    _email_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Envelope),
                                          QLineEdit::LeadingPosition);
    _email_edit->getLineEdit()->setPlaceholderText("邮箱");
    _email_edit->getLineEdit()->setFixedWidth(width);
    _email_edit->setTipText("请输入有效的邮箱地址");


    //登录注册按钮
    QWidget* bottomWid = new QWidget(this);
    QVBoxLayout* bottomLayout = new QVBoxLayout(bottomWid);
    ThemeColorButton* registerButton = new ThemeColorButton("注册", this);
    ElaPushButton* loginButton = new ElaPushButton("已有账户？立即登录", this);
    QFont f = loginButton->font();
    f.setPixelSize(13);

    registerButton->setFont(f);
    registerButton->setFixedSize(_register_pwd_edit->getLineEdit()->width(),
                                 _register_pwd_edit->getLineEdit()->height() + 6);
    registerButton->setBorderRadius(8);

    loginButton->setFont(f);
    loginButton->setFixedSize(registerButton->size());
    loginButton->setBorderRadius(8);

    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(10);
    bottomLayout->addWidget(registerButton);
    bottomLayout->addWidget(loginButton);
    bottomWid->setLayout(bottomLayout);

    connect(loginButton, &QPushButton::clicked, this, &RegisterPage::sigLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &RegisterPage::sigRegisterButtonClicked);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(15);
    mainLayout->addSpacerItem(new QSpacerItem(0, 100, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(_registerText, 1, Qt::AlignCenter);
    mainLayout->addWidget(_username_edit, 2, Qt::AlignCenter);
    mainLayout->addWidget(_register_pwd_edit, 3, Qt::AlignCenter);
    mainLayout->addWidget(_confirm_pwd_edit, 4, Qt::AlignCenter);
    mainLayout->addWidget(_email_edit, 5, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 90, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(bottomWid, 7, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 130, QSizePolicy::Minimum, QSizePolicy::Expanding));

    this->setLayout(mainLayout);

    checkRegisterInfo();
}

void RegisterPage::checkRegisterInfo()
{
    connect(_username_edit, &TipLineEdit::sigTextChanged, this, &RegisterPage::slotUsernameChanged);
    connect(_email_edit, &TipLineEdit::sigTextChanged, this, &RegisterPage::slotEmailChanged);
    connect(_register_pwd_edit, &TipLineEdit::sigTextChanged, this, &RegisterPage::slotPasswordFormatChanged);
    connect(_confirm_pwd_edit, &TipLineEdit::sigTextChanged, this, &RegisterPage::slotConfirmPasswordChanged);
}

void RegisterPage::showErrorTip(const QString &text)
{
    _registerText->setTextColor(Qt::red);
    _registerText->setText(text);
}

void RegisterPage::showNormalTip()
{
    _registerText->setTextColor(QColor());
    _registerText->setTextColorDark(Qt::white);
    _registerText->setTextColorLight(Qt::black);
    _registerText->setText("创建您的账户，开启高效沟通之旅");
}

void RegisterPage::clearData()
{
    _username_edit->getLineEdit()->clear();
    _email_edit->getLineEdit()->clear();
    _register_pwd_edit->getLineEdit()->clear();
    _confirm_pwd_edit->getLineEdit()->clear();
}


bool RegisterPage::slotUsernameChanged(const QString& text)
{
    int length = text.length();
    if (length <= 0 || length > 12) {
        showErrorTip("至少1个字符，不能超过12个字符");
        return false;
    }
    else {
        showNormalTip();
        return true;
    }
}

bool RegisterPage::slotEmailChanged(const QString& text)
{
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!regex.match(text).hasMatch()) {
        showErrorTip("请输入有效的邮箱地址");
        return false;
    }
    else {
        showNormalTip();
        return true;
    }
}

bool RegisterPage::slotPasswordFormatChanged(const QString& text)
{
    QRegularExpression regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#]).{8,16}$");
    if (!regex.match(text).hasMatch()) {
        showErrorTip("8到16位，必须包含大小写字母、数字、特殊字符(!@#)");
        return false;
    }
    else {
        showNormalTip();
        return true;
    }
}

bool RegisterPage::slotConfirmPasswordChanged(const QString& text)
{
    if (text != _register_pwd_edit->text()) {
        showErrorTip("两次输入的密码不一致");
        return false;
    }
    else {
        showNormalTip();
        return true;
    }
}
