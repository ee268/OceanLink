#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>

#include "../../controls/icontext.h"
#include "../../controls/tiplineedit.h"

class RegisterPage : public QWidget
{
    Q_OBJECT
    friend class AuthPage;
public:
    explicit RegisterPage(QWidget* parent = nullptr);

private:
    void initContent();

    void checkRegisterInfo();

    void showErrorTip(const QString& text);
    void showNormalTip();

    void clearData();

private:
    IconText* _registerText;
    TipLineEdit* _username_edit;
    TipLineEdit* _email_edit;
    TipLineEdit* _register_pwd_edit;
    TipLineEdit* _confirm_pwd_edit;

signals:
    void sigRegisterButtonClicked();

    void sigLoginButtonClicked();

private slots:
    //注册信息检查
    bool slotUsernameChanged(const QString& text);
    bool slotEmailChanged(const QString& text);
    bool slotPasswordFormatChanged(const QString& text);
    bool slotConfirmPasswordChanged(const QString& text);
};

#endif // REGISTERPAGE_H
