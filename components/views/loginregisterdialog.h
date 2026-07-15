#ifndef LOGINREGISTERDIALOG_H
#define LOGINREGISTERDIALOG_H

#include "ElaDialog.h"
#include "../controls/tiplineedit.h"
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

class LoginRegisterDialog : public ElaDialog
{
    Q_OBJECT

public:
    enum CurrentPage {
        Login,
        Register
    };

public:
    explicit LoginRegisterDialog(QWidget* parent = nullptr);
    ~LoginRegisterDialog();

    CurrentPage getCurPage() const;

private:
    void initDialog();
    void initContent();
    QWidget* initLoginPage();
    QWidget* initRegisterPage();
    void switchPage(int index);

private:
    QStackedWidget* _stackedWid;
    CurrentPage _curPage;
    //登录
    TipLineEdit* _account_edit;
    TipLineEdit* _login_pwd_edit;

    //注册
    TipLineEdit* _username_edit;
    TipLineEdit* _email_edit;
    TipLineEdit* _register_pwd_edit;
    TipLineEdit* _confirm_pwd_edit;

public slots:
    void slotLoginButtonClicked();
    void slotRegisterButtonClicked();

signals:
    void sigLoginSuccess();
    void sigLoginFailed();
    void sigRegisterButtonClicked();
    void sigLoginButtonClicked();
};

#endif // LOGINREGISTERDIALOG_H
