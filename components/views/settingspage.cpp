#include "settingspage.h"

#include "basepage.h"
#include "settings/notifypage.h"
#include "settings/storagepage.h"
#include "settings/shorcutkeypage.h"
#include "settings/permissionpage.h"

#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>

SettingsPage::SettingsPage(QWidget *parent)
    : ElaDialog(parent)
    , _notifyPage(nullptr)
    , _storagePage(nullptr)
    , _shortcutKeyPage(nullptr)
    , _permissionPage(nullptr)
{
    this->setWindowTitle("设置");
    this->setIsStayTop(false);

    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        int screenHeight = screen->availableGeometry().height();
        this->setMinimumSize(800, screenHeight - 50);
    }

    initContent();
}

void SettingsPage::initContent()
{
    _centralWid = new BasePage(this);
    auto centralWid = dynamic_cast<BasePage*>(_centralWid);
    centralWid->initDefaultLayout();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_centralWid);
    this->setLayout(mainLayout);

    auto leftWid = centralWid->getLeftWidget();
    leftWid->setFixedWidth(280);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWid);

    _navigation = new ElaNavigationBar(leftWid);
    _navigation->setUserInfoCardVisible(false);

    _notifyPage = new NotifyPage(this);
    _storagePage = new StoragePage(this);
    _shortcutKeyPage = new ShorcutKeyPage(this);
    _permissionPage = new PermissionPage(this);

    _navigation->addPageNode("通知", _notifyPage, ElaIconType::Bell);
    _navigation->addPageNode("存储", _storagePage, ElaIconType::HardDrive);
    _navigation->addPageNode("快捷键", _shortcutKeyPage, ElaIconType::Keyboard);
    _navigation->addPageNode("权限", _permissionPage, ElaIconType::ShieldCheck);

    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->addWidget(_navigation);
    leftWid->setLayout(leftLayout);

    auto stackedWid = centralWid->getStackedWidget();
    stackedWid->addWidget(_notifyPage);
    stackedWid->addWidget(_storagePage);
    stackedWid->addWidget(_shortcutKeyPage);
    stackedWid->addWidget(_permissionPage);

    stackedWid->setCurrentIndex(Notify_Page);

    connect(_navigation, &ElaNavigationBar::navigationNodeClicked,
            this, [centralWid](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack)
            {
                QStackedWidget* stackedWid = centralWid->getStackedWidget();
                if (nodeType == ElaNavigationType::PageNode) {
                    for (int i = 0; i < stackedWid->count(); i++) {
                        QWidget* page = stackedWid->widget(i);
                        if (page && page->property("ElaPageKey").toString() == nodeKey) {
                            centralWid->setCurrentIndex(i);
                            break;
                        }
                    }
                }
            });
}
