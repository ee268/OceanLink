#include "mainwindow.h"
#include "../global/global.h"

#include <QScreen>
#include <QApplication>

#include "../components/controls/confirmdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
    , _loginRegisterDialog(new LoginRegisterDialog(nullptr))
    , _settingsPage(new SettingsPage(nullptr))
{
    initWindow();

    initContent();
}

MainWindow::~MainWindow()
{
    if (_loginRegisterDialog) {
        _loginRegisterDialog->close();
        delete _loginRegisterDialog;
        _loginRegisterDialog = nullptr;
    }

    if (_settingsPage) {
        _settingsPage->close();
        delete _settingsPage;
        _settingsPage = nullptr;
    }
}

void MainWindow::initWindow()
{
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(1100, 640);
    setIsDefaultClosed(false);

    setUserInfoCardPixmap(QPixmap(":/resource/image/avatar.jpg"));
    setUserInfoCardTitle("ee268");
    setUserInfoCardSubTitle("inthebluesky268@gmail.com");
    setWindowTitle(PROjECT_NAME);

    setNavigationBarDisplayMode(ElaNavigationType::Compact);
    setStackSwitchMode(ElaWindowType::StackSwitchMode::Scale);

    setWindowButtonFlags(ElaAppBarType::StayTopButtonHint |
                         ElaAppBarType::ThemeChangeButtonHint |
                         ElaAppBarType::MinimizeButtonHint |
                         ElaAppBarType::MaximizeButtonHint |
                         ElaAppBarType::CloseButtonHint);

    this->hide();
    _loginRegisterDialog->open();

    connect(_loginRegisterDialog, &LoginRegisterDialog::sigLoginSuccess, this, &MainWindow::slotLoginSuccess);
    connect(_loginRegisterDialog, &LoginRegisterDialog::sigLoginFailed, this, &MainWindow::slotLoginFailed);
    connect(_loginRegisterDialog, &LoginRegisterDialog::closeButtonClicked, this, &MainWindow::close);
    connect(_loginRegisterDialog, &LoginRegisterDialog::sigRegisterButtonClicked,
            this, &MainWindow::slotShowRegisterPage);
    connect(_loginRegisterDialog, &LoginRegisterDialog::sigLoginButtonClicked,
            this, &MainWindow::slotShowLoginPage);
}

void MainWindow::initContent()
{
    _closeConfirmDialog = new ConfirmDialog(this);
    connect(this, &MainWindow::closeButtonClicked, this, &MainWindow::slotShowConfirmDialog);
    connect(_closeConfirmDialog, &ConfirmDialog::rightButtonClicked, this, &MainWindow::close);

    _chatPage = new ChatPage(this);
    _contactPage = new ContactPage(this);
    _postPage = new PostPage(this);
    _aboutPage = new AboutPage(this);
    _collectPage = new CollectPage(this);
    _moreOptPopup = new MoreOptPopup(this);
    _aboutKey = "aboutKey";
    _collectKey = "collectKey";
    _moreOptKey = "moreOptKey";
    _settingKey = "settingKey";
    _logoutKey = "logoutKey";

    addPageNode("聊天", _chatPage, ElaIconType::CommentDots);
    addPageNode("联系人", _contactPage, ElaIconType::UserGroup);
    addPageNode("空间", _postPage, ElaIconType::Blog);

    addFooterNode("关于", nullptr, _aboutKey, 0, ElaIconType::CircleInfo);
    addFooterNode("收藏", _collectPage, _collectKey, 0, ElaIconType::Bookmark);
    addFooterNode("更多", nullptr, _moreOptKey, 0, ElaIconType::Ellipsis);

    _moreOptPopup->addOption("设置", QIcon(":/resource/image/more/settings-outline.svg"),
                             _settingKey, nullptr);
    _moreOptPopup->addOption("退出账号", QIcon(":/resource/image/more/power-outline.svg"),
                             _logoutKey, nullptr);

    _logoutConfirmDialog = new ConfirmDialog(this);
    _logoutConfirmDialog->setTitleText("退出账号");
    _logoutConfirmDialog->setSubTitleText("确定要退出账号吗？");
    _logoutConfirmDialog->setCheckBoxHidden(true);

    connect(_moreOptPopup, &MoreOptPopup::sigOptionClicked, this, &MainWindow::slotMoreOptionClicked);
    connect(_logoutConfirmDialog, &ConfirmDialog::rightButtonClicked,
            this, &MainWindow::slotSwitchToLogin);

    connect(this, &MainWindow::navigationNodeClicked, [this](auto nodeType, QString nodeKey){
        if (nodeKey == _aboutKey) {
            _aboutPage->open();
        }
        else if (nodeKey == _moreOptKey) {
            _moreOptPopup->show();
        }
    });
}

void MainWindow::slotLoginSuccess()
{
    _loginRegisterDialog->close();
    this->show();
}

void MainWindow::slotLoginFailed()
{

}

void MainWindow::slotMoreOptionClicked(const QString& key)
{
    if (key == _logoutKey) {
        _logoutConfirmDialog->show();
    }
    else if (key == _settingKey) {
        _settingsPage->show();
    }
}

void MainWindow::slotSwitchToLogin()
{
    _loginRegisterDialog->open();
    this->hide();
}

void MainWindow::slotShowRegisterPage()
{
    // to do
}

void MainWindow::slotShowLoginPage()
{
    //to do
}

void MainWindow::slotShowConfirmDialog()
{
    _closeConfirmDialog->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (_loginRegisterDialog) {
        _loginRegisterDialog->close();
    }
    if (_settingsPage) {
        _settingsPage->close();
    }
    ElaWindow::closeEvent(event);
}

