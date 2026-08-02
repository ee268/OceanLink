#ifndef LOGINREGISTERDIALOG_H
#define LOGINREGISTERDIALOG_H

#include "ElaDialog.h"

#include "../controls/icontext.h"
#include "../controls/tiplineedit.h"
#include "../controls/themecolorbutton.h"

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>

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

    void switchPage(int index);

    void checkRegisterInfo();

    void showErrorTip(const QString& text);
    void showNormalTip();

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
    ThemeColorButton* _sendCodeButton;

    QTimer* _countdown_timer;
    int _countdown;

public slots:
    void slotLoginButtonClicked();
    void slotRegisterButtonClicked();

    bool slotUsernameChanged(const QString& text);
    bool slotEmailChanged(const QString& text);
    bool slotPasswordFormatChanged(const QString& text);
    bool slotConfirmPasswordChanged(const QString& text);

signals:
    void sigLoginSuccess();
    void sigLoginFailed();
    void sigRegisterButtonClicked();
    void sigLoginButtonClicked();
};

#endif // LOGINREGISTERDIALOG_H
