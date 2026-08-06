#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ElaWindow.h"
#include "views/chatpage.h"
#include "views/contactpage.h"
#include "views/postpage.h"
#include "views/aboutpage.h"
#include "views/collectpage.h"
#include "controls/moreoptpopup.h"
#include "views/authpage.h"
#include "controls/confirmdialog.h"
#include "views/settingspage.h"

class MainWindow : public ElaWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initWindow();

    void initContent();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    ConfirmDialog* _closeConfirmDialog;
    ConfirmDialog* _logoutConfirmDialog;

    AuthPage* _authPage;
    ChatPage* _chatPage;
    ContactPage* _contactPage;
    PostPage* _postPage;

    AboutPage* _aboutPage;
    CollectPage* _collectPage;
    MoreOptPopup* _moreOptPopup;
    SettingsPage* _settingsPage;

    QString _aboutKey;
    QString _collectKey;
    QString _moreOptKey;
    QString _settingKey;
    QString _logoutKey;

public slots:
    void slotLoginSuccess();

    void slotMoreOptionClicked(const QString& key);
    void slotSwitchToLogin();

    void slotShowConfirmDialog();
};

#endif // MAINWINDOW_H
