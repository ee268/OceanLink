#ifndef AUTHPAGE_H
#define AUTHPAGE_H

#include "ElaDialog.h"

#include <QStackedWidget>

#include "./auth/loginpage.h"
#include "./auth/registerpage.h"
#include "./auth/codepage.h"

#include "../../global/global.h"

class AuthPage : public ElaDialog
{
    Q_OBJECT
public:
    explicit AuthPage(QWidget* parent = nullptr);
    ~AuthPage();

    enum CurrentPage {
        Login,
        Register,
        VerifyCode
    };

private:
    void initContent();
    QWidget* header();

    void initHandler();

    void switchPage(CurrentPage page);

    void sendCode();

private:
    QStackedWidget* _stackedWid;
    CurrentPage _curPage;

    LoginPage* _loginPage;
    RegisterPage* _regPage;
    CodePage* _codePage;

    //服务器回包响应
    QMap<RequestID, RspHandler> _handlers;

signals:
    void sigLoginSuccess();

private slots:
    void slotLoginBtnClicked();

    void slotRegBtnClicked();

    void slotRegModFinished(RequestID id, QString res, ErrorCodes ec);

    void slotSendCodeBtnClicked();

    void slotConfirmRegClicked();
};

#endif // AUTHPAGE_H
