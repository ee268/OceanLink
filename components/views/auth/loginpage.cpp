#include "loginpage.h"

#include <QVBoxLayout>

#include "../../controls/themecolorbutton.h"

#include "ElaIcon.h"
#include "ElaCheckBox.h"

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
{
    initContent();
}

void LoginPage::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    int width = 240;

    _loginText = new IconText(this);
    _loginText->setText("欢迎回来！请登录您的账户");
    _loginText->setPixelSize(12);
    _loginText->setTextColorDark(Qt::white);
    _loginText->setTextColorLight(Qt::black);

    //账号输入框
    _account_edit = new TipLineEdit(this);
    _account_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::User),
                                            QLineEdit::LeadingPosition);
    _account_edit->getLineEdit()->setPlaceholderText("邮箱或账号");
    _account_edit->getLineEdit()->setIsClearButtonEnable(true);
    _account_edit->getLineEdit()->setBorderRadius(8);
    _account_edit->setFixedWidth(width);

    //密码输入框
    _login_pwd_edit = new TipLineEdit(this);
    _login_pwd_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::LockKeyhole),
                                              QLineEdit::LeadingPosition);
    _login_pwd_edit->getLineEdit()->setPlaceholderText("密码");
    _login_pwd_edit->getLineEdit()->setIsClearButtonEnable(false);
    _login_pwd_edit->setFixedWidth(width);
    _login_pwd_edit->setEchoModePassword();

    QWidget* forgetWid = new QWidget(this);
    forgetWid->setFixedWidth(width);
    QHBoxLayout* forgetLayout = new QHBoxLayout(forgetWid);
    forgetLayout->setContentsMargins(0, 0, 0, 0);

    //忘记密码
    IconText* forgetText = new IconText(forgetWid);
    forgetText->setText("忘记密码？");
    forgetText->setPixelSize(13);
    forgetText->setTextColor(QColor("#1a6bf8"));
    QFontMetrics fm(forgetText->font());
    forgetText->setFixedSize(fm.horizontalAdvance(forgetText->text()), fm.height());

    //记住密码勾选框
    ElaCheckBox* rememberCheckBox = new ElaCheckBox(forgetWid);
    rememberCheckBox->setText("记住我");
    rememberCheckBox->setFont(forgetText->font());

    forgetLayout->addWidget(rememberCheckBox, 0, Qt::AlignVCenter);
    forgetLayout->addStretch();
    forgetLayout->addWidget(forgetText, 1, Qt::AlignVCenter);
    forgetWid->setLayout(forgetLayout);


    //登录注册按钮
    ThemeColorButton* loginButton = new ThemeColorButton("登录", this);
    ElaPushButton* registerButton = new ElaPushButton("还没有账户？立即注册", this);
    QFont f = loginButton->font();
    f.setPixelSize(13);

    loginButton->setFont(f);
    loginButton->setFixedSize(width,
                              _login_pwd_edit->getLineEdit()->height() + 6);
    loginButton->setBorderRadius(8);

    registerButton->setFont(f);
    registerButton->setFixedSize(loginButton->size());
    registerButton->setBorderRadius(8);

    connect(loginButton, &QPushButton::clicked, this, &LoginPage::sigLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginPage::sigRegisterButtonClicked);

    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->setSpacing(5);
    mainLayout->addSpacerItem(new QSpacerItem(0, 120, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(_loginText, 1, Qt::AlignCenter);
    mainLayout->addWidget(_account_edit, 2, Qt::AlignCenter);
    mainLayout->addWidget(_login_pwd_edit, 3, Qt::AlignCenter);
    mainLayout->addWidget(forgetWid, 4, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 110, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(loginButton, 5, Qt::AlignCenter);
    mainLayout->addWidget(registerButton, 6, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 120, QSizePolicy::Minimum, QSizePolicy::Expanding));

    this->setLayout(mainLayout);
}

void LoginPage::clearData()
{
    _account_edit->getLineEdit()->clear();
    _login_pwd_edit->getLineEdit()->clear();
}
