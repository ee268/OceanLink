#include "loginregisterdialog.h"

#include "ElaImageCard.h"
#include "ElaIcon.h"
#include "ElaText.h"
#include "ElaCheckBox.h"

#include "../controls/themecolorbutton.h"
#include "../controls/icontext.h"

#include <QVBoxLayout>
#include <QAction>
#include <QSpacerItem>
#include <QDebug>

LoginRegisterDialog::LoginRegisterDialog(QWidget *parent/* = nullptr*/)
    : ElaDialog(parent)
{
    initDialog();

    initContent();
}

LoginRegisterDialog::~LoginRegisterDialog() {}

LoginRegisterDialog::CurrentPage LoginRegisterDialog::getCurPage() const
{
    return _curPage;
}

void LoginRegisterDialog::switchPage(int index)
{
    if (index == _stackedWid->currentIndex() || index < 0 || index >= _stackedWid->count()) {
        return;
    }

    QWidget* currentPage = _stackedWid->currentWidget();
    QWidget* nextPage = _stackedWid->widget(index);

    int offset = _stackedWid->width();
    int duration = 200;

    if (index > _stackedWid->currentIndex()) {
        offset = offset;
    } else {
        offset = -offset;
    }

    nextPage->setGeometry(offset, 0, nextPage->width(), nextPage->height());
    nextPage->show();

    QPropertyAnimation* anim1 = new QPropertyAnimation(currentPage, "pos");
    anim1->setDuration(duration);
    anim1->setEndValue(QPoint(-offset, 0));

    QPropertyAnimation* anim2 = new QPropertyAnimation(nextPage, "pos");
    anim2->setDuration(duration);
    anim2->setEndValue(QPoint(0, 0));

    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(anim1);
    group->addAnimation(anim2);

    connect(group, &QParallelAnimationGroup::finished, [=]() {
        _stackedWid->setCurrentIndex(index);
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginRegisterDialog::initDialog()
{
    setFixedSize(400, 560);
    setWindowButtonFlags(ElaAppBarType::MinimizeButtonHint | ElaAppBarType::CloseButtonHint);
    setSizeGripEnabled(false);
    setIsFixedSize(true);
    setWindowTitle("");
}

void LoginRegisterDialog::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    _stackedWid = new QStackedWidget(this);

    QWidget* loginPage = initLoginPage();
    QWidget* registerPage = initRegisterPage();

    _stackedWid->addWidget(loginPage);
    _stackedWid->addWidget(registerPage);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_stackedWid);

    this->setLayout(mainLayout);
    _curPage = CurrentPage::Login;
}

QWidget* LoginRegisterDialog::initLoginPage()
{
    QWidget* loginWid = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(loginWid);
    int width = this->width() * 0.6;

    QImage img(":/resource/image/logo/OceanLink.png");
    double ratio = 0.45;
    int w = img.width() * ratio;
    int h = img.height() * ratio;
    ElaImageCard* logoImg = new ElaImageCard(loginWid);
    logoImg->setCardImage(img);
    logoImg->setFixedSize(QSize(w, h));

    ElaText* loginText = new ElaText(loginWid);
    loginText->setText("欢迎回来！请登录您的账户");
    loginText->setTextStyle(ElaTextType::BodyStrong);
    loginText->setIsWrapAnywhere(false);

    //账号输入框
    _account_edit = new TipLineEdit(loginWid);
    _account_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::User),
                             QLineEdit::LeadingPosition);
    _account_edit->getLineEdit()->setPlaceholderText("邮箱或账号");
    _account_edit->getLineEdit()->setIsClearButtonEnable(true);
    _account_edit->getLineEdit()->setBorderRadius(8);
    _account_edit->setFixedWidth(width);

    //密码输入框
    _login_pwd_edit = new TipLineEdit(loginWid);
    _login_pwd_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::LockKeyhole),
                         QLineEdit::LeadingPosition);
    _login_pwd_edit->getLineEdit()->setPlaceholderText("密码");
    _login_pwd_edit->getLineEdit()->setIsClearButtonEnable(false);
    _login_pwd_edit->setFixedWidth(width);
    _login_pwd_edit->setEchoModePassword();


    QWidget* forgetWid = new QWidget(loginWid);
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
    ThemeColorButton* loginButton = new ThemeColorButton("登录", loginWid);
    ElaPushButton* registerButton = new ElaPushButton("还没有账户？立即注册", loginWid);
    QFont f = loginButton->font();
    f.setPixelSize(13);

    loginButton->setFont(f);
    loginButton->setFixedSize(width,
                              _login_pwd_edit->getLineEdit()->height() + 6);
    loginButton->setBorderRadius(8);

    registerButton->setFont(f);
    registerButton->setFixedSize(loginButton->size());
    registerButton->setBorderRadius(8);

    connect(loginButton, &QPushButton::clicked, this, &LoginRegisterDialog::slotLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginRegisterDialog::slotRegisterButtonClicked);

    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(logoImg, 0, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 120, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(loginText, 1, Qt::AlignCenter);
    mainLayout->addWidget(_account_edit, 2, Qt::AlignCenter);
    mainLayout->addWidget(_login_pwd_edit, 3, Qt::AlignCenter);
    mainLayout->addWidget(forgetWid, 4, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 110, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(loginButton, 5, Qt::AlignCenter);
    mainLayout->addWidget(registerButton, 6, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 120, QSizePolicy::Minimum, QSizePolicy::Expanding));

    loginWid->setLayout(mainLayout);

    return loginWid;
}

QWidget* LoginRegisterDialog::initRegisterPage()
{
    QWidget* registerWid = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(registerWid);

    QImage img(":/resource/image/logo/OceanLink.png");
    double ratio = 0.45;
    int w = img.width() * ratio;
    int h = img.height() * ratio;
    ElaImageCard* logoImg = new ElaImageCard(registerWid);
    logoImg->setCardImage(img);
    logoImg->setFixedSize(QSize(w, h));

    ElaText* loginText = new ElaText(registerWid);
    loginText->setText("创建您的账户，开启高效沟通之旅");
    loginText->setTextStyle(ElaTextType::BodyStrong);
    loginText->setIsWrapAnywhere(false);

    //昵称输入框
    _username_edit = new TipLineEdit(registerWid);
    _username_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::User),
                                             QLineEdit::LeadingPosition);
    _username_edit->getLineEdit()->setPlaceholderText("昵称");
    _username_edit->getLineEdit()->setFixedWidth(this->width() * 0.6);
    _username_edit->setTipText("至少1个字符，不能超过12个字符");

    //密码输入框
    _register_pwd_edit = new TipLineEdit(registerWid);
    _register_pwd_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::LockKeyhole),
                                        QLineEdit::LeadingPosition);
    _register_pwd_edit->getLineEdit()->setPlaceholderText("设置密码");
    _register_pwd_edit->getLineEdit()->setIsClearButtonEnable(false);
    _register_pwd_edit->getLineEdit()->setFixedWidth(this->width() * 0.6);
    _register_pwd_edit->setTipText("8到12位，必须包含大小写字母、数字、特殊字符");
    _register_pwd_edit->setEchoModePassword();

    _confirm_pwd_edit = new TipLineEdit(registerWid);
    _confirm_pwd_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::LockKeyhole),
                                                QLineEdit::LeadingPosition);
    _confirm_pwd_edit->getLineEdit()->setPlaceholderText("确认密码");
    _confirm_pwd_edit->getLineEdit()->setIsClearButtonEnable(false);
    _confirm_pwd_edit->getLineEdit()->setFixedWidth(this->width() * 0.6);
    _confirm_pwd_edit->setTipText("请再次输入密码");
    _confirm_pwd_edit->setEchoModePassword();

    //邮箱输入框
    _email_edit = new TipLineEdit(registerWid);
    _email_edit->getLineEdit()->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Envelope),
                                          QLineEdit::LeadingPosition);
    _email_edit->getLineEdit()->setPlaceholderText("邮箱");
    _email_edit->getLineEdit()->setFixedWidth(this->width() * 0.6);
    _email_edit->setTipText("请输入有效的邮箱地址");


    //登录注册按钮
    QWidget* bottomWid = new QWidget(registerWid);
    QVBoxLayout* bottomLayout = new QVBoxLayout(bottomWid);
    ThemeColorButton* registerButton = new ThemeColorButton("注册", registerWid);
    ElaPushButton* loginButton = new ElaPushButton("已有账户？立即登录", registerWid);
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

    connect(loginButton, &QPushButton::clicked, this, &LoginRegisterDialog::slotLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginRegisterDialog::slotRegisterButtonClicked);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(15);
    mainLayout->addWidget(logoImg, 0, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 100, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(loginText, 1, Qt::AlignCenter);
    mainLayout->addWidget(_username_edit, 2, Qt::AlignCenter);
    mainLayout->addWidget(_register_pwd_edit, 3, Qt::AlignCenter);
    mainLayout->addWidget(_confirm_pwd_edit, 4, Qt::AlignCenter);
    mainLayout->addWidget(_email_edit, 5, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 90, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(bottomWid, 7, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 130, QSizePolicy::Minimum, QSizePolicy::Expanding));

    registerWid->setLayout(mainLayout);

    return registerWid;
}

void LoginRegisterDialog::slotLoginButtonClicked()
{
    switch (_curPage) {
    case CurrentPage::Login:
        emit sigLoginSuccess();

        break;
    case CurrentPage::Register:
        emit sigLoginButtonClicked();
        _curPage = CurrentPage::Login;
        switchPage(CurrentPage::Login);

        break;
    default:
        break;
    }
}

void LoginRegisterDialog::slotRegisterButtonClicked()
{
    switch (_curPage) {
    case CurrentPage::Login:
        emit sigRegisterButtonClicked();
        _curPage = CurrentPage::Register;
        switchPage(CurrentPage::Register);

        break;
    case CurrentPage::Register:
        // to do

        break;
    default:
        break;
    }
}
