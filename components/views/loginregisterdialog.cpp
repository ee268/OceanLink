#include "loginregisterdialog.h"

#include "ElaImageCard.h"
#include "ElaIcon.h"
#include "ElaCheckBox.h"
#include "ElaMessageBar.h"

#include <QVBoxLayout>
#include <QAction>
#include <QSpacerItem>
#include <QDebug>
#include <QRegularExpression>
#include <QJsonDocument>

#include "../../network/httpmanager.h"

#include "../../global/global.h"

LoginRegisterDialog::LoginRegisterDialog(QWidget *parent/* = nullptr*/)
    : ElaDialog(parent)
    , _countdown_timer(new QTimer(this))
    , _countdown(180)
{
    initDialog();

    initContent();

    initHandler();
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

void LoginRegisterDialog::checkRegisterInfo()
{
    connect(_username_edit, &TipLineEdit::sigTextChanged, this, &LoginRegisterDialog::slotUsernameChanged);
    connect(_email_edit, &TipLineEdit::sigTextChanged, this, &LoginRegisterDialog::slotEmailChanged);
    connect(_register_pwd_edit, &TipLineEdit::sigTextChanged, this, &LoginRegisterDialog::slotPasswordFormatChanged);
    connect(_confirm_pwd_edit, &TipLineEdit::sigTextChanged, this, &LoginRegisterDialog::slotConfirmPasswordChanged);
}

bool LoginRegisterDialog::slotUsernameChanged(const QString& text)
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

bool LoginRegisterDialog::slotEmailChanged(const QString& text)
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

bool LoginRegisterDialog::slotPasswordFormatChanged(const QString& text)
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

bool LoginRegisterDialog::slotConfirmPasswordChanged(const QString& text)
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

void LoginRegisterDialog::slotConfirmRegBtnClicked()
{
    QString code = _codeEdit->text();
    if (code.isEmpty()) {
        ElaMessageBar::error(ElaMessageBarType::Top, "错误", "请输入验证码", 2000, this);
        return;
    }

    QJsonObject jsonObj;
    jsonObj["username"] = _username_edit->text();
    jsonObj["email"] = _email_edit->text();
    jsonObj["password"] = xorString(_register_pwd_edit->text());
    jsonObj["verifyCode"] = code;

    // HttpManager::getInstance()->postHttpReq()
}

void LoginRegisterDialog::slotRegModFinished(RequestID id, QString res, ErrorCodes ec)
{
    if (_handlers.find(id) == _handlers.end()) {
        return;
    }

    _handlers[id](res, ec);
}

void LoginRegisterDialog::showNormalTip()
{
    if (getCurPage() == CurrentPage::Login) {
        _loginText->setTextColor(QColor());
        _loginText->setTextColorDark(Qt::white);
        _loginText->setTextColorLight(Qt::black);
        _loginText->setText("欢迎回来！请登录您的账户");
    }
    else {
        _registerText->setTextColor(QColor());
        _registerText->setTextColorDark(Qt::white);
        _registerText->setTextColorLight(Qt::black);
        _registerText->setText("创建您的账户，开启高效沟通之旅");
    }
}

void LoginRegisterDialog::sendVeriyCode()
{
    _verifyCodeText->setText("正在发送验证码");
    _verifyCodeText->setTextColor(QColor("#1a6bf8"));

    //test    
    _username_edit->getLineEdit()->setText("yonghu123");
    _email_edit->getLineEdit()->setText("sh33dhl@qq.com");
    _register_pwd_edit->getLineEdit()->setText("8794772034Gkl@");
    _confirm_pwd_edit->getLineEdit()->setText("8794772034Gkl@");

    if (slotUsernameChanged(_username_edit->text()) &&
        slotEmailChanged(_email_edit->text()) &&
        slotPasswordFormatChanged(_register_pwd_edit->text()) &&
        slotConfirmPasswordChanged(_confirm_pwd_edit->text()))
    {
        QJsonObject jsonObj;
        QString route = "/get_verifyCode";

        jsonObj["email"] = _email_edit->text();

        HttpManager::getInstance()->postHttpReq(
            QUrl(ServerUrl + route),
            jsonObj,
            RequestID::ID_GET_VERIFY_CODE,
            Modules::REGISTER);
    }
}

void LoginRegisterDialog::showErrorTip(const QString &text)
{
    if (getCurPage() == CurrentPage::Login) {
        _loginText->setTextColor(Qt::red);
        _loginText->setText(text);
    }
    else {
        _registerText->setTextColor(Qt::red);
        _registerText->setText(text);
    }
}

void LoginRegisterDialog::initDialog()
{
    setMinimumSize(400, 560);
    setMaximumSize(400, 560);
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
    QWidget* verifyCodePage = initVerifyCodePage();

    _stackedWid->addWidget(loginPage);
    _stackedWid->addWidget(registerPage);
    _stackedWid->addWidget(verifyCodePage);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_stackedWid);

    this->setLayout(mainLayout);
    _curPage = CurrentPage::Login;
}

QWidget *LoginRegisterDialog::header()
{
    QImage img(":/resource/image/logo/OceanLink.png");
    double ratio = 0.45;
    int w = img.width() * ratio;
    int h = img.height() * ratio;
    ElaImageCard* logoImg = new ElaImageCard(this);
    logoImg->setCardImage(img);
    logoImg->setFixedSize(QSize(w, h));

    return logoImg;
}

QWidget* LoginRegisterDialog::initLoginPage()
{
    QWidget* loginWid = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(loginWid);
    int width = this->width() * 0.6;

    _loginText = new IconText(loginWid);
    _loginText->setText("欢迎回来！请登录您的账户");
    _loginText->setPixelSize(12);
    _loginText->setTextColorDark(Qt::white);
    _loginText->setTextColorLight(Qt::black);

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
    mainLayout->addWidget(header(), 0, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 120, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(_loginText, 1, Qt::AlignCenter);
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

    _registerText = new IconText(registerWid);
    _registerText->setText("创建您的账户，开启高效沟通之旅");
    _registerText->setPixelSize(12);
    _registerText->setTextColorDark(Qt::white);
    _registerText->setTextColorLight(Qt::black);

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
    mainLayout->addWidget(header(), 0, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 100, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(_registerText, 1, Qt::AlignCenter);
    mainLayout->addWidget(_username_edit, 2, Qt::AlignCenter);
    mainLayout->addWidget(_register_pwd_edit, 3, Qt::AlignCenter);
    mainLayout->addWidget(_confirm_pwd_edit, 4, Qt::AlignCenter);
    mainLayout->addWidget(_email_edit, 5, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 90, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(bottomWid, 7, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 130, QSizePolicy::Minimum, QSizePolicy::Expanding));

    registerWid->setLayout(mainLayout);

    checkRegisterInfo();

    return registerWid;
}

QWidget *LoginRegisterDialog::initVerifyCodePage()
{
    QWidget* wid = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(wid);
    mainLayout->setSpacing(20);

    int width = this->width() * 0.6;

    _verifyCodeText = new IconText(wid);
    _verifyCodeText->setText("正在发送验证码");
    _verifyCodeText->setPixelSize(12);
    _verifyCodeText->setTextColor(QColor("#1a6bf8"));

    QWidget* subWid = new QWidget(wid);
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

    connect(_sendCodeButton, &ThemeColorButton::clicked, this, [this](){
        sendVeriyCode();
    });

    connect(_countdown_timer, &QTimer::timeout, this, [this](){
        if (_countdown == 0) {
            _countdown_timer->stop();
            _sendCodeButton->setDisabled(false);
            _sendCodeButton->setText("重新发送");
            return;
        }

        _sendCodeButton->setText(QString::number(_countdown));
        _countdown--;
    });

    subLayout->addWidget(_codeEdit);
    subLayout->addWidget(_sendCodeButton);
    subWid->setLayout(subLayout);

    ThemeColorButton* confirmButton = new ThemeColorButton("确认", wid);
    confirmButton->setFixedWidth(width);
    confirmButton->setBorderRadius(8);

    connect(confirmButton, &ThemeColorButton::clicked, this, &LoginRegisterDialog::slotConfirmRegBtnClicked);

    ElaPushButton* backButton = new ElaPushButton("返回", wid);
    backButton->setFixedWidth(width);
    backButton->setBorderRadius(8);

    connect(backButton, &ElaPushButton::clicked, this, [this](){
        _countdown_timer->stop();
        switchPage(CurrentPage::Register);
    });

    mainLayout->addWidget(header(), 1, Qt::AlignHCenter);
    mainLayout->addStretch(1);
    mainLayout->addWidget(_verifyCodeText, 0, Qt::AlignHCenter);
    mainLayout->addWidget(subWid, 0, Qt::AlignHCenter);
    mainLayout->addStretch(1);
    mainLayout->addWidget(confirmButton, 0, Qt::AlignHCenter);
    mainLayout->addWidget(backButton, 0, Qt::AlignHCenter);
    mainLayout->addStretch(1);
    wid->setLayout(mainLayout);

    return wid;
}

void LoginRegisterDialog::initHandler()
{
    _handlers[RequestID::ID_GET_VERIFY_CODE] = [this](QString res, ErrorCodes ec){
        QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
        QJsonObject jsonObj = doc.object();

        if (ec != ErrorCodes::Success ||
            !jsonObj.contains("error") ||
            jsonObj["error"].toInt() != ErrorCodes::Success)
        {
            _verifyCodeText->setText("请重新发送");
            _verifyCodeText->setTextColor(Qt::red);
            ElaMessageBar::error(ElaMessageBarType::Top, "错误", "验证发送失败", 2000, this);
            _countdown_timer->stop();
            _sendCodeButton->setDisabled(false);
            return;
        }

        _verifyCodeText->setText("验证码已发送至您的邮箱，三分钟内有效");
        _verifyCodeText->setTextColorDark(Qt::white);
        _verifyCodeText->setTextColorLight(Qt::black);
        ElaMessageBar::success(ElaMessageBarType::Top, "成功", "验证码已发送", 2000, this);

        qDebug() << res;
        qDebug() << ec;

        _countdown_timer->start(1000);
        _countdown = 180;
        _sendCodeButton->setDisabled(true);
    };

    connect(HttpManager::getInstance(), &HttpManager::sigRegModFinished,
            this, &LoginRegisterDialog::slotRegModFinished);
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

        _username_edit->getLineEdit()->clear();
        _email_edit->getLineEdit()->clear();
        _register_pwd_edit->getLineEdit()->clear();
        _confirm_pwd_edit->getLineEdit()->clear();

        switchPage(CurrentPage::Register);

        break;
    case CurrentPage::Register:
        switchPage(CurrentPage::VerifyCode);
        sendVeriyCode();

        break;
    default:
        break;
    }
}
