#include "groupmgrdialog.h"

#include <QVBoxLayout>

#include "../basepage.h"
#include "../../controls/themecolorbutton.h"

GroupMgrDialog::GroupMgrDialog(ContactList* list, QWidget *parent)
    : ElaDialog(parent)
    , _contactList(list)
{
    setWindowTitle("分组管理");
    setMinimumSize(900, 600);

    initContent();
}

void GroupMgrDialog::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    _centralWid = new BasePage(this);
    auto centralWid = dynamic_cast<BasePage*>(_centralWid);
    centralWid->initDefaultLayout();

    QWidget* leftWid = centralWid->getLeftWidget();
    leftWid->setFixedWidth(280);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWid);

    _navigation = new ElaNavigationBar(leftWid);
    _groupKey = "groupKey";
    _addGroupKey = "addGroupKey";

    _navigation->setUserInfoCardVisible(false);

    _navigation->addPageNode("所有好友", new QWidget(this), ElaIconType::Users);
    _navigation->addCategoryNode("分组", _groupKey);

    QStringList groupNames = _contactList->getGroupNames();
    for (auto& name : groupNames) {
        _navigation->addPageNode(name, new QWidget(this), ElaIconType::User);
    }

    _navigation->addFooterNode("添加分组", nullptr, _addGroupKey, 0, ElaIconType::Plus);

    _addGroupDialog = new ElaDialog(this);
    QHBoxLayout* addGroupLayout = new QHBoxLayout(_addGroupDialog);
    _addGroupEdit = new ElaLineEdit(_addGroupDialog);
    ThemeColorButton* addBtn = new ThemeColorButton("添加", _addGroupDialog);

    addGroupLayout->setContentsMargins(15, 0, 15, 0);
    addGroupLayout->addWidget(_addGroupEdit);
    addGroupLayout->addWidget(addBtn);

    _addGroupDialog->setLayout(addGroupLayout);
    _addGroupDialog->setMinimumSize(150, 100);
    _addGroupDialog->setMaximumSize(QWIDGETSIZE_MAX, 100);
    _addGroupDialog->setIsFixedSize(true);
    _addGroupDialog->setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    _addGroupDialog->setWindowTitle("添加分组");

    connect(_navigation, &ElaNavigationBar::navigationNodeClicked, this, &GroupMgrDialog::slotNavigationClicked);

    connect(addBtn, &ThemeColorButton::clicked, this, &GroupMgrDialog::slotAddGroupClicked);

    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    leftLayout->addWidget(_navigation);
    leftWid->setLayout(leftLayout);

    mainLayout->addWidget(_centralWid);

    this->setLayout(mainLayout);
}

void GroupMgrDialog::slotAddGroupClicked()
{
    QString text = _addGroupEdit->text();
    if (text.isEmpty()) {
        return;
    }

    _addGroupEdit->clear();
    _contactList->addGroup(text);
    _navigation->addPageNode(text, new QWidget(this), ElaIconType::User);
}

void GroupMgrDialog::slotNavigationClicked(ElaNavigationType::NavigationNodeType nodeType,
                                           QString nodeKey, bool isRouteBack)
{
    if (nodeType == ElaNavigationType::FooterNode &&
        nodeKey == _addGroupKey)
    {
        _addGroupDialog->open();
    }
}
