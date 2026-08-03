#ifndef LOGINREGISTERDIALOG_H
#define LOGINREGISTERDIALOG_H

#include "ElaDialog.h"

#include "../controls/icontext.h"
#include "../controls/tiplineedit.h"
#include "../controls/themecolorbutton.h"

#include "../../global/global.h"

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <QMap>

class LoginRegisterDialog : public ElaDialog
{
    Q_OBJECT

public:
    enum CurrentPage {
        Login,
        Register,
        VerifyCode
    };

public:
    explicit LoginRegisterDialog(QWidget* parent = nullptr);
    ~LoginRegisterDialog();

    CurrentPage getCurPage() const;

private:
    void initDialog();
    void initContent();

    QWidget* header();
    QWidget* initLoginPage();
    QWidget* initRegisterPage();
    QWidget* initVerifyCodePage();

    void initHandler();

    void switchPage(int index);

    void checkRegisterInfo();

    void showErrorTip(const QString& text);
    void showNormalTip();

    void sendVeriyCode();

private:
    QStackedWidget* _stackedWid;
    CurrentPage _curPage;
    //登录
    IconText* _loginText;
    TipLineEdit* _account_edit;
    TipLineEdit* _login_pwd_edit;

    //注册
    IconText* _registerText;
    TipLineEdit* _username_edit;
    TipLineEdit* _email_edit;
    TipLineEdit* _register_pwd_edit;
    TipLineEdit* _confirm_pwd_edit;

    //验证码
    IconText* _verifyCodeText;
    ThemeColorButton* _sendCodeButton;
    ElaLineEdit* _codeEdit;

    QTimer* _countdown_timer;
    int _countdown;

    //服务器回包响应
    QMap<RequestID, RspHandler> _handlers;

public slots:
    void slotLoginButtonClicked();
    void slotRegisterButtonClicked();

    bool slotUsernameChanged(const QString& text);
    bool slotEmailChanged(const QString& text);
    bool slotPasswordFormatChanged(const QString& text);
    bool slotConfirmPasswordChanged(const QString& text);

    void slotConfirmRegBtnClicked();

    void slotRegModFinished(RequestID id, QString res, ErrorCodes ec);

signals:
    void sigLoginSuccess();
    void sigLoginFailed();
    void sigRegisterButtonClicked();
    void sigLoginButtonClicked();
};

#endif // LOGINREGISTERDIALOG_H
