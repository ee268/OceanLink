#include "authpage.h"

#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include <QJsonDocument>

#include "ElaImageCard.h"
#include "ElaMessageBar.h"

#include "../../network/httpmanager.h"

AuthPage::AuthPage(QWidget *parent)
    : ElaDialog(parent)
{
    qDebug() << "AuthPage: start";
    setMinimumSize(400, 560);
    setMaximumSize(400, 560);
    setWindowButtonFlags(ElaAppBarType::MinimizeButtonHint | ElaAppBarType::CloseButtonHint);
    setSizeGripEnabled(false);
    setIsFixedSize(true);
    setWindowTitle("");

    initContent();

    initHandler();
}

AuthPage::~AuthPage()
{

}

void AuthPage::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    _loginPage = new LoginPage(this);
    _regPage = new RegisterPage(this);
    _codePage = new CodePage(this);

    _stackedWid = new QStackedWidget(this);
    _curPage = CurrentPage::Login;

    _stackedWid->addWidget(_loginPage);
    _stackedWid->addWidget(_regPage);
    _stackedWid->addWidget(_codePage);

    for (int i = 0; i < _stackedWid->count(); i++) {
        auto wid = _stackedWid->widget(i);
        auto layout = dynamic_cast<QVBoxLayout*>(wid->layout());
        layout->insertWidget(0, header(), Qt::AlignHCenter);
    }

    mainLayout->addWidget(_stackedWid);

    _stackedWid->setCurrentIndex(_curPage);

    connect(_loginPage, &LoginPage::sigLoginButtonClicked, this, &AuthPage::slotLoginBtnClicked);
    connect(_loginPage, &LoginPage::sigRegisterButtonClicked, this, [this](){
        switchPage(CurrentPage::Register);
        _regPage->clearData();
    });

    connect(_regPage, &RegisterPage::sigRegisterButtonClicked, this, &AuthPage::slotRegBtnClicked);
    connect(_regPage, &RegisterPage::sigLoginButtonClicked, this, [this](){
        switchPage(CurrentPage::Login);
        _loginPage->clearData();
    });

    connect(_codePage, &CodePage::sigSendBtnClicked, this, &AuthPage::slotSendCodeBtnClicked);
    connect(_codePage, &CodePage::sigConfirmRegBtnClicked, this, &AuthPage::slotConfirmRegClicked);
    connect(_codePage, &CodePage::sigBackBtnClicked, this, [this](){
        switchPage(CurrentPage::Register);
    });
}

QWidget *AuthPage::header()
{
    QImage img(":/resource/image/logo/OceanLink.png");
    double ratio = 0.45;
    int w = img.width() * ratio;
    int h = img.height() * ratio;

    QWidget* wid = new QWidget(this);
    QVBoxLayout* widLayout = new QVBoxLayout(wid);
    widLayout->setContentsMargins(0, 0, 0, 0);
    widLayout->setSpacing(0);

    ElaImageCard* logoImg = new ElaImageCard(wid);
    logoImg->setCardImage(img);
    logoImg->setFixedSize(QSize(w, h));

    widLayout->addWidget(logoImg, 0, Qt::AlignCenter);

    return wid;
}

void AuthPage::initHandler()
{
    _handlers[RequestID::ID_GET_VERIFY_CODE] = [this](QString res, ErrorCodes ec){
        QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
        QJsonObject jsonObj = doc.object();

        _codePage->_backBtn->setDisabled(false);
        _codePage->_confirmButton->setDisabled(false);

        if (ec != ErrorCodes::Success ||
            !jsonObj.contains("error") ||
            jsonObj["error"].toInt() != ErrorCodes::Success)
        {
            _codePage->_verifyCodeText->setText("请重新发送");
            _codePage->_verifyCodeText->setTextColor(Qt::red);
            ElaMessageBar::error(ElaMessageBarType::Top, "错误", "验证发送失败", 2000, this);
            _codePage->stopCountdown();
            return;
        }

        _codePage->_verifyCodeText->setText("验证码已发送至您的邮箱，三分钟内有效");
        _codePage->_verifyCodeText->setTextColorDark(Qt::white);
        _codePage->_verifyCodeText->setTextColorLight(Qt::black);
        ElaMessageBar::success(ElaMessageBarType::Top, "成功", "验证码已发送", 2000, this);

        qDebug() << res;
        qDebug() << ec;

        _codePage->startCountdown();
    };

    _handlers[RequestID::ID_REG_USER] = [this](QString res, ErrorCodes ec){
        QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
        QJsonObject jsonObj = doc.object();

        if (ec != ErrorCodes::Success ||
            !jsonObj.contains("error") ||
            jsonObj["error"].toInt() != ErrorCodes::Success)
        {
            ElaMessageBar::error(ElaMessageBarType::Top, "错误", "网络错误", 2000, this);
            return;
        }

        ElaMessageBar::success(ElaMessageBarType::Top, "成功", "注册完成", 2000, this);

        switchPage(CurrentPage::Login);
        _codePage->_codeEdit->clear();
        _curPage = CurrentPage::Login;

        qDebug() << res;
        qDebug() << ec;

        _codePage->_countdown_timer->stop();
    };

    connect(HttpManager::getInstance(), &HttpManager::sigRegModFinished,
            this, &AuthPage::slotRegModFinished);
}

void AuthPage::switchPage(CurrentPage page)
{
    QWidget* currentPage = _stackedWid->currentWidget();
    QWidget* nextPage = _stackedWid->widget(page);

    int offset = _stackedWid->width();
    int duration = 200;

    if (page > _stackedWid->currentIndex()) {
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
        _stackedWid->setCurrentIndex(page);
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);

    _curPage = page;
}

void AuthPage::sendCode()
{
    _codePage->_backBtn->setDisabled(true);
    _codePage->_confirmButton->setDisabled(true);

    QJsonObject jsonObj;
    QString route = "/get_verifyCode";

    jsonObj["email"] = _regPage->_email_edit->text();

    HttpManager::getInstance()->postHttpReq(
        QUrl(ServerUrl + route),
        jsonObj,
        RequestID::ID_GET_VERIFY_CODE,
        Modules::Auth);
}

void AuthPage::slotLoginBtnClicked()
{
    // to do...
    emit sigLoginSuccess();
}

void AuthPage::slotRegBtnClicked()
{
    _codePage->_verifyCodeText->setText("正在发送验证码");
    _codePage->_verifyCodeText->setTextColor(QColor("#1a6bf8"));
    _codePage->_sendCodeButton->setDisabled(true);

    //test
    _regPage->_username_edit->getLineEdit()->setText("yonghu123");
    _regPage->_email_edit->getLineEdit()->setText("sh33dhl@qq.com");
    _regPage->_register_pwd_edit->getLineEdit()->setText("8794772034Gkl@");
    _regPage->_confirm_pwd_edit->getLineEdit()->setText("8794772034Gkl@");

    if (_regPage->slotUsernameChanged(_regPage->_username_edit->text()) &&
        _regPage->slotEmailChanged(_regPage->_email_edit->text()) &&
        _regPage->slotPasswordFormatChanged(_regPage->_register_pwd_edit->text()) &&
        _regPage->slotConfirmPasswordChanged(_regPage->_confirm_pwd_edit->text()))
    {
        sendCode();
        switchPage(CurrentPage::VerifyCode);
    }
}

void AuthPage::slotRegModFinished(RequestID id, QString res, ErrorCodes ec)
{
    if (_handlers.find(id) == _handlers.end()) {
        return;
    }

    _handlers[id](res, ec);
}

void AuthPage::slotSendCodeBtnClicked()
{
    _codePage->_verifyCodeText->setText("正在发送验证码");
    _codePage->_verifyCodeText->setTextColor(QColor("#1a6bf8"));
    _codePage->_sendCodeButton->setDisabled(true);

    sendCode();
}

void AuthPage::slotConfirmRegClicked()
{
    QString code = _codePage->_codeEdit->text();
    if (code.isEmpty()) {
        ElaMessageBar::error(ElaMessageBarType::Top, "错误", "请输入验证码", 2000, this);
        return;
    }

    QJsonObject jsonObj;
    jsonObj["username"] = _regPage->_username_edit->text();
    jsonObj["email"] = _regPage->_email_edit->text();
    jsonObj["password"] = xorString(_regPage->_register_pwd_edit->text());
    jsonObj["verifyCode"] = code;

    QString route = "/reg_user";

    HttpManager::getInstance()->postHttpReq(
        QUrl(ServerUrl + route),
        jsonObj,
        RequestID::ID_REG_USER,
        Modules::Auth);
}
