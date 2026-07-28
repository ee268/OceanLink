#include "groupmgrdialog.h"

#include <QVBoxLayout>
#include <QDebug>

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

    _allFriendGroup = new  QWidget(this);
    _allFriendGroup->setProperty("GroupName", "AllFriend");

    _groupList = new GroupMgrList(this);

    QVBoxLayout* allFriendLayout = new QVBoxLayout(_allFriendGroup);
    allFriendLayout->setContentsMargins(0, 0, 0, 0);
    allFriendLayout->setSpacing(0);
    allFriendLayout->addWidget(_groupList);
    _allFriendGroup->setLayout(allFriendLayout);

    centralWid->getStackedWidget()->addWidget(_allFriendGroup);

    initGroupList();

    _navigation->addPageNode("所有好友", _allFriendGroup, ElaIconType::Users);
    _navigation->addCategoryNode("分组", _groupKey);

    QStringList groupNames = _contactList->getGroupNames();
    for (auto& name : groupNames) {
        QWidget* widget = new QWidget(this);
        widget->setProperty("GroupName", name);
        _otherGroups.append(widget);
        _navigation->addPageNode(name, widget, ElaIconType::User);
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

    centralWid->getStackedWidget()->setCurrentIndex(1);

    this->setLayout(mainLayout);
}

void GroupMgrDialog::initGroupList()
{
    auto centralWid = dynamic_cast<BasePage*>(_centralWid);
    QWidget* headerWid = new QWidget(this);
    QHBoxLayout* headerLayout = new QHBoxLayout(this);
    headerLayout->setContentsMargins(5, 5, 5, 5);

    _allRadioBtn = new ElaRadioButton(this);

    connect(_allRadioBtn, &ElaRadioButton::clicked, this, &GroupMgrDialog::slotClickedRadioBtn);

    ElaText* name = new ElaText("昵称", this);
    name->setTextStyle(ElaTextType::Body);

    ElaText* nickName = new ElaText("备注", this);
    nickName->setTextStyle(ElaTextType::Body);

    ElaText* groupName = new ElaText("分组", this);
    groupName->setTextStyle(ElaTextType::Body);
    groupName->setFixedWidth(80);

    headerLayout->addWidget(_allRadioBtn, 0, Qt::AlignLeft);
    headerLayout->addWidget(name, 1, Qt::AlignLeft);
    headerLayout->addWidget(nickName, 1, Qt::AlignLeft);
    headerLayout->addWidget(groupName, 2, Qt::AlignRight);

    headerWid->setLayout(headerLayout);

    auto vLayout = dynamic_cast<QVBoxLayout*>(centralWid->getStackedWidget()->widget(1)->layout());
    vLayout->insertWidget(0, headerWid);
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
    if (nodeType == ElaNavigationType::FooterNode)
    {
        if (nodeKey == _addGroupKey) {
            _addGroupDialog->open();
        }
    }
    else if (nodeType == ElaNavigationType::PageNode) {
        for (auto widget : _otherGroups) {
            if (widget->property("ElaPageKey").toString() == nodeKey) {
                QString groupName = widget->property("GroupName").toString();
                qDebug() << "clicked group: " << groupName;
            }
        }
    }
}

void GroupMgrDialog::slotClickedRadioBtn()
{
    _groupList->setAllChecked(_allRadioBtn->isChecked());
}
