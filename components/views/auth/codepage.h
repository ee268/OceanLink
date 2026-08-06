#ifndef CODEPAGE_H
#define CODEPAGE_H

#include <QWidget>

#include "ElaLineEdit.h"

#include "../../controls/icontext.h"
#include "../../controls/themecolorbutton.h"

#include <QTimer>

class CodePage : public QWidget
{
    Q_OBJECT
    friend class AuthPage;
public:
    explicit CodePage(QWidget* parent = nullptr);

private:
    void initContent();

    void startCountdown();
    void stopCountdown();

private:
    IconText* _verifyCodeText;
    ThemeColorButton* _sendCodeButton;
    ThemeColorButton* _confirmButton;
    ElaPushButton* _backBtn;
    ElaLineEdit* _codeEdit;

    QTimer* _countdown_timer;
    int _countdown;

signals:
    void sigSendBtnClicked();

    void sigConfirmRegBtnClicked();

    void sigBackBtnClicked();
};

#endif // CODEPAGE_H
