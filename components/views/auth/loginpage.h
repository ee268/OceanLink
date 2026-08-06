#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

#include "../../controls/icontext.h"
#include "../../controls/tiplineedit.h"

class LoginPage : public QWidget
{
    Q_OBJECT
    friend class AuthPage;
public:
    explicit LoginPage(QWidget* parent = nullptr);

private:
    void initContent();

    void clearData();

private:
    IconText* _loginText;
    TipLineEdit* _account_edit;
    TipLineEdit* _login_pwd_edit;

signals:
    void sigLoginButtonClicked();

    void sigRegisterButtonClicked();
};

#endif // LOGINPAGE_H
