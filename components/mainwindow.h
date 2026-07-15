#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ElaWindow.h"
#include "views/chatpage.h"
#include "views/contactpage.h"
#include "views/postpage.h"
#include "views/aboutpage.h"
#include "views/collectpage.h"
#include "controls/moreoptpopup.h"
#include "views/loginregisterdialog.h"
#include "controls/confirmdialog.h"

class MainWindow : public ElaWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initWindow();

    void initContent();

private:
    ConfirmDialog* _closeConfirmDialog;
    ConfirmDialog* _logoutConfirmDialog;
    LoginRegisterDialog* _loginRegisterDialog;

    ChatPage* _chatPage;
    ContactPage* _contactPage;
    PostPage* _postPage;

    AboutPage* _aboutPage;
    CollectPage* _collectPage;
    MoreOptPopup* _moreOptPopup;

    QString _aboutKey;
    QString _collectKey;
    QString _moreOptKey;
    QString _settingKey;
    QString _logoutKey;

public slots:
    void slotLoginSuccess();
    void slotLoginFailed();

    void slotLogout(const QString& key);
    void slotSwitchToLogin();

    void slotShowRegisterPage();
    void slotShowLoginPage();

    void slotShowConfirmDialog();
};

#endif // MAINWINDOW_H
