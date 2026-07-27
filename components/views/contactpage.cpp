#include "contactpage.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QPainterPath>
#include <QPainter>
#include <QDebug>

#include "ElaPushButton.h"
#include "ElaText.h"

#include "../../global/global.h"

ContactPage::ContactPage(QWidget *parent/* = nullptr*/)
    : BasePage(parent)
{
    initDefaultLayout();

    initLeftWidget();
    initRightWidget();
}

ContactPage::~ContactPage() {}

void ContactPage::initLeftWidget()
{
    QWidget* leftWid = this->getLeftWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(leftWid);

    _suggestBox = new ElaSuggestBox(leftWid);
    _suggestBox->setPlaceholderText("搜索");
    _suggestBox->setMinimumWidth(280);
    _suggestBox->setMaximumWidth(QWIDGETSIZE_MAX);
    IconButton* addFriendButton = new IconButton(QIcon(":/resource/image/chat/add-outline.png"), leftWid);
    addFriendButton->setFixedSize(40, _suggestBox->height());
    addFriendButton->setBorderRadius(6);
    addFriendButton->setIconSize(20);

    QWidget* subWid = new QWidget(leftWid);
    QVBoxLayout* subLayout = new QVBoxLayout(subWid);
    QWidget* subWid2 = new QWidget(subWid);
    QHBoxLayout* subHLayout = new QHBoxLayout(subWid2);
    subHLayout->setContentsMargins(0, 0, 0, 0);
    subHLayout->setSpacing(6);
    subHLayout->addWidget(_suggestBox);
    subHLayout->addWidget(addFriendButton);
    subWid2->setLayout(subHLayout);

    ElaPushButton* checkNotifyButton = new ElaPushButton("好友通知", subWid);
    checkNotifyButton->setFixedHeight(checkNotifyButton->height() + 2);
    checkNotifyButton->setBorderRadius(6);

    connect(checkNotifyButton, &ElaPushButton::clicked, this, &ContactPage::slotNotifyButtonClicked);

    ElaPushButton* openGroupButton = new ElaPushButton("分组管理", subWid);
    openGroupButton->setFixedHeight(checkNotifyButton->height() + 2);
    openGroupButton->setBorderRadius(6);

    _contactList = new ContactList(leftWid);

    _groupDialog = new GroupMgrDialog(_contactList, this);

    connect(openGroupButton, &ElaPushButton::clicked, this, &ContactPage::slotOpenGroupClicked);

    subLayout->setContentsMargins(10, 10, 10, 10);
    subLayout->setSpacing(10);
    subLayout->addWidget(subWid2);
    subLayout->addWidget(checkNotifyButton);
    subLayout->addWidget(openGroupButton);
    subWid->setLayout(subLayout);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(subWid);
    mainLayout->addWidget(_contactList);
    leftWid->setLayout(mainLayout);
}

void ContactPage::initRightWidget()
{
    QStackedWidget* stackedWid = this->getStackedWidget();
    QWidget* rightWid = new QWidget(stackedWid);
    QVBoxLayout* mainLayout = new QVBoxLayout(rightWid);

    QWidget* subWid = new QWidget(rightWid);
    QHBoxLayout* subWidLayout = new QHBoxLayout(subWid);

    ElaText* title = new ElaText("好友通知", subWid);
    QFont f = title->font();
    f.setPixelSize(20);
    f.setBold(true);
    title->setFont(f);
    title->setIsWrapAnywhere(false);

    IconButton* clearButton = new IconButton(QIcon(":/resource/image/chat/sessionPoup/trash-outline.png"), subWid);
    clearButton->setFixedSize(35, 35);

    connect(clearButton, &IconButton::clicked, this, &ContactPage::slotClearButtonClicked);

    subWidLayout->setContentsMargins(20, 20, 20, 0);
    subWidLayout->addWidget(title);
    subWidLayout->addStretch();
    subWidLayout->addWidget(clearButton, 1, Qt::AlignVCenter);
    subWid->setLayout(subWidLayout);

    _notifyList = new FriendNotifyList(rightWid);

    //测试
    for (int i = 0; i < 5; i++) {
        FriendNotifyData data(
            QPixmap(":/resource/image/avatar.jpg"),
            "123123123",
            "2024/10/21",
            "你好",
            i % 2 == 0,
            FriendNotifyStatus::Passed);
        _notifyList->addNotifyItem(data);
    }
    for (int i = 0; i < 5; i++) {
        FriendNotifyData data(
            QPixmap(":/resource/image/avatar.jpg"),
            "123123123",
            "2024/10/21",
            "你好",
            i % 2 == 0,
            FriendNotifyStatus::NotPassed);
        _notifyList->addNotifyItem(data);
    }
    for (int i = 0; i < 5; i++) {
        FriendNotifyData data(
            QPixmap(),
            "123123123",
            "2024/10/21",
            "你好",
            i % 2 == 0,
            FriendNotifyStatus::WaitingPassed);
        _notifyList->addNotifyItem(data);
    }

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(subWid);
    mainLayout->addWidget(_notifyList);
    rightWid->setLayout(mainLayout);

    stackedWid->addWidget(rightWid);

    _clearConfirmDialog = new ConfirmDialog(this);
    _clearConfirmDialog->setTitleText("清空好友通知");
    _clearConfirmDialog->setSubTitleText("确定要清空所有好友通知吗？");
    _clearConfirmDialog->setCheckBoxHidden(true);
    _clearConfirmDialog->setMiddleButtonHidden(true);

    connect(_clearConfirmDialog, &ConfirmDialog::rightButtonClicked,
            this, &ContactPage::slotClearAllNotify);

    initContactDetailWid();
}

void ContactPage::initContactDetailWid()
{
    QStackedWidget* stackedWid = this->getStackedWidget();
    _detailWid = new ContactDetailWid(_contactList, this);

    connect(_contactList, &ContactList::sigContactClicked, this, &ContactPage::slotToContactDetail);

    stackedWid->addWidget(_detailWid);
}

void ContactPage::slotClearButtonClicked()
{
    _clearConfirmDialog->show();
}

void ContactPage::slotClearAllNotify()
{
    _notifyList->clear();
}

void ContactPage::slotNotifyButtonClicked()
{
    this->getStackedWidget()->setCurrentIndex(1);
}

void ContactPage::slotToContactDetail(const QModelIndex &index)
{
    _detailWid->setIndex(index);
    this->getStackedWidget()->setCurrentIndex(2);
}

void ContactPage::slotOpenGroupClicked()
{
    _groupDialog->open();
}

