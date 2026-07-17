#include "contactpage.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QPainterPath>
#include <QPainter>

#include "ElaPushButton.h"
#include "ElaText.h"
#include "ElaIcon.h"

#include "../controls/iconbutton.h"
#include "../controls/contactlistmodel.h"
#include "../controls/splitline.h"

#include "../../global/global.h"

#include "ElaExponentialBlur.h"

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
    checkNotifyButton->setFixedSize(_suggestBox->size());
    checkNotifyButton->setFixedWidth(checkNotifyButton->width() + 40 + 6);
    checkNotifyButton->setFixedHeight(checkNotifyButton->height() + 2);
    checkNotifyButton->setBorderRadius(6);

    connect(checkNotifyButton, &ElaPushButton::clicked, this, &ContactPage::slotNotifyButtonClicked);

    subLayout->setContentsMargins(10, 10, 10, 10);
    subLayout->setSpacing(10);
    subLayout->addWidget(subWid2);
    subLayout->addWidget(checkNotifyButton);
    subWid->setLayout(subLayout);

    _contactList = new ContactList(leftWid);

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

    subWidLayout->setContentsMargins(0, 0, 0, 0);
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

    mainLayout->setContentsMargins(20, 20, 20, 20);
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
    _detailWid = new ContactDetailWid(this);

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

ContactDetailWid::ContactDetailWid(QWidget *parent)
    : QWidget(parent)
    , _avatar(nullptr)
{
    initContent();
}

void ContactDetailWid::setIndex(const QModelIndex &index)
{
    _index = index;
    updateInfo();
}

void ContactDetailWid::initContent()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);

    _centralWid = new QWidget(this);
    _centralWid->setFixedWidth(600);

    vLayout->setContentsMargins(0, 30, 0, 0);
    vLayout->addWidget(_centralWid, 0, Qt::AlignCenter);
    vLayout->addStretch();
    this->setLayout(vLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout(_centralWid);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(8);

    SplitLine* line1 = new SplitLine(_centralWid);
    line1->setFixedHeight(1);

    initCard();

    initAccountInfo();

    mainLayout->addWidget(line1);

    initPersonalInfo();

    initFriendInfo();

    mainLayout->addStretch();

    _centralWid->setLayout(mainLayout);
}

void ContactDetailWid::initCard()
{
    _card = new DisplayCard(_centralWid);
    _card->setBorderRadius(5);
    _card->setCardPixmap(QPixmap(":/resource/image/rupa.jpg"));
    _card->setFixedHeight(250);

    _centralWid->layout()->addWidget(_card);
}

void ContactDetailWid::initAccountInfo()
{
    //头像、昵称、账号、状态
    _avatar = new AvatarWid(_centralWid);
    _avatar->setFixedSize(62, 62);

    QWidget* subWid = new QWidget(_centralWid);
    QVBoxLayout* subLayout = new QVBoxLayout(subWid);

    _name = new ElaText(_centralWid);
    QFont f = _name->font();
    f.setBold(true);
    f.setPixelSize(14);
    _name->setFont(f);

    _account = new IconText(_centralWid);
    _account->setPixelSize(12);

    _status = new IconText(_centralWid);
    _status->setPixelSize(13);
    _status->setIconThemeColor(false);
    _status->setTextColorLight(Qt::black);
    _status->setTextColorDark(Qt::white);

    subLayout->setContentsMargins(0, 0, 0, 0);
    subLayout->setSpacing(0);
    subLayout->addWidget(_name);
    subLayout->addWidget(_account);
    subLayout->addWidget(_status);
    subWid->setLayout(subLayout);
    subWid->setFixedHeight(_avatar->height());

    QWidget* baseInfoWid = new QWidget(_centralWid);
    QHBoxLayout* baseInfoLayout = new QHBoxLayout(baseInfoWid);
    baseInfoLayout->setContentsMargins(0, 5, 0, 5);
    baseInfoLayout->setSpacing(6);
    baseInfoLayout->addWidget(_avatar);
    baseInfoLayout->addWidget(subWid);
    baseInfoWid->setLayout(baseInfoLayout);
    baseInfoWid->setFixedHeight(_avatar->height() + 10);

    _centralWid->layout()->addWidget(baseInfoWid);
}

void ContactDetailWid::initPersonalInfo()
{
    //性别，年龄，生日
    QWidget* pWid = new QWidget(_centralWid);
    QHBoxLayout* pLayout = new QHBoxLayout(pWid);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(5);

    _sexText = new IconText(_centralWid);
    _sexText->setTextColorLight(Qt::black);
    _sexText->setTextColorDark(Qt::white);
    _sexText->setIconThemeColor(false);
    _sexText->setPixelSize(12);

    _ageText = new ElaText(_centralWid);
    _ageText->setTextPixelSize(12);

    _birthText = new ElaText(_centralWid);
    _birthText->setTextPixelSize(12);

    QFont f = _sexText->font();
    QFontMetrics fm(f);
    SplitLine* vLine1 = new SplitLine(_centralWid);
    vLine1->setFixedSize(1, fm.height());
    SplitLine* vLine2 = new SplitLine(_centralWid);
    vLine2->setFixedSize(vLine1->size());

    pLayout->addWidget(_sexText);
    pLayout->addWidget(vLine1);
    pLayout->addWidget(_ageText);
    pLayout->addWidget(vLine2);
    pLayout->addWidget(_birthText);
    pLayout->addStretch();
    pWid->setLayout(pLayout);

    _centralWid->layout()->addWidget(pWid);
}

void ContactDetailWid::initFriendInfo()
{
    // 备注、分组、签名、空间
    QWidget* fWid = new QWidget(_centralWid);
    QVBoxLayout* fLayout = new QVBoxLayout(fWid);
    fLayout->setContentsMargins(0, 10, 0, 0);
    fLayout->setSpacing(12);

    QWidget* nicknameWid = new QWidget(fWid);
    QHBoxLayout* nicknameLayout = new QHBoxLayout(nicknameWid);
    nicknameLayout->setContentsMargins(0, 0, 0, 0);

    IconText* nicknameTitle  = new IconText("备注", nicknameWid);
    nicknameTitle->setIcon(QIcon(":/resource/image/contact/detail/create-outline.png"));
    nicknameTitle->setTextColorLight(Qt::black);
    nicknameTitle->setTextColorDark(Qt::white);
    nicknameTitle->setPixelSize(13);
    _nickname = new ElaText(nicknameWid);
    _nickname->setFont(nicknameTitle->font());

    nicknameLayout->addWidget(nicknameTitle);
    nicknameLayout->addStretch();
    nicknameLayout->addWidget(_nickname);
    nicknameWid->setLayout(nicknameLayout);

    QWidget* groupWid = new QWidget(fWid);
    QHBoxLayout* groupLayout = new QHBoxLayout(groupWid);
    groupLayout->setContentsMargins(0, 0, 0, 0);

    IconText* groupTitle  = new IconText("好友分组", groupWid);
    groupTitle->setIcon(QIcon(":/resource/image/contact/detail/people-outline.png"));
    groupTitle->setTextColorLight(Qt::black);
    groupTitle->setTextColorDark(Qt::white);
    groupTitle->setPixelSize(13);
    _friendGroup = new ElaText(groupWid);
    _friendGroup->setFont(groupTitle->font());

    groupLayout->addWidget(groupTitle);
    groupLayout->addStretch();
    groupLayout->addWidget(_friendGroup);
    groupWid->setLayout(groupLayout);

    QWidget* signWid = new QWidget(fWid);
    QHBoxLayout* signLayout = new QHBoxLayout(signWid);
    signLayout->setContentsMargins(0, 0, 0, 0);

    IconText* signTitle  = new IconText("签名", signWid);
    signTitle->setIcon(QIcon(":/resource/image/contact/detail/pencil-outline.png"));
    signTitle->setTextColorLight(Qt::black);
    signTitle->setTextColorDark(Qt::white);
    signTitle->setPixelSize(13);
    _sign = new ElaText(signWid);
    _sign->setFont(signTitle->font());

    signLayout->addWidget(signTitle);
    signLayout->addStretch();
    signLayout->addWidget(_sign);
    signWid->setLayout(signLayout);

    QWidget* spaceWid = new QWidget(fWid);
    QHBoxLayout* spaceLayout = new QHBoxLayout(spaceWid);
    spaceLayout->setContentsMargins(0, 0, 0, 0);

    IconText* spaceTitle  = new IconText("空间", spaceWid);
    spaceTitle->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Blog));
    spaceTitle->setTextColorLight(Qt::black);
    spaceTitle->setTextColorDark(Qt::white);
    spaceTitle->setPixelSize(13);
    _space = new ElaText(spaceWid);
    _space->setFont(spaceTitle->font());

    spaceLayout->addWidget(spaceTitle);
    spaceLayout->addStretch();
    spaceLayout->addWidget(_space);
    spaceWid->setLayout(spaceLayout);

    fLayout->addWidget(nicknameWid);
    fLayout->addWidget(groupWid);
    fLayout->addWidget(signWid);
    fLayout->addWidget(spaceWid);
    fWid->setLayout(fLayout);

    _centralWid->layout()->addWidget(fWid);
}

void ContactDetailWid::updateInfo()
{
    QPixmap avatar = qvariant_cast<QPixmap>(_index.data(ContactListModel::Avatar));
    QString name = _index.data(ContactListModel::Name).toString();
    QString account = _index.data(ContactListModel::Account).toString();
    QString sign = _index.data(ContactListModel::Sign).toString();
    bool status = _index.data(ContactListModel::Status).toBool();
    int sex = _index.data(ContactListModel::Sex).toInt();
    int age = _index.data(ContactListModel::Age).toInt();
    QString birthday = _index.data(ContactListModel::Birthday).toString();

    _avatar->setAvatar(avatar);
    _avatar->setName(name);

    _name->setText(name);

    _account->setText("账号: " + account);

    if (status) {
        _status->setText("在线");
        _status->setIcon(QIcon(":/resource/image/chat/ellipse-green.svg"));
    }
    else {
        _status->setText("离线");
        _status->setIcon(QIcon(":/resource/image/chat/ellipse-gray.svg"));
    }

    switch (sex) {
    case Sex::Female:
        _sexText->setText("女");
        _sexText->setIcon(QIcon(":/resource/image/contact/detail/female-outline.png"));
        break;
    case Sex::Male:
        _sexText->setText("男");
        _sexText->setIcon(QIcon(":/resource/image/contact/detail/male-outline.png"));
        break;
    case Sex::Secret:
        _sexText->setText("保密");
        _sexText->setIcon(QIcon(":/resource/image/contact/detail/male-female-outline.png"));
        break;
    default:
        break;
    }

    _ageText->setText(QString("%1岁").arg(age));
    _birthText->setText(birthday);
}

AvatarWid::AvatarWid(QWidget *parent)
    : QWidget(parent)
{

}

void AvatarWid::setAvatar(const QPixmap &pixmap)
{
    _avatar = pixmap;
    update();
}

void AvatarWid::setName(const QString &name)
{
    _name = name;
    update();
}

void AvatarWid::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    if (_avatar.isNull()) {
        painter.setPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorderDeep));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(this->rect().adjusted(1, 1, -1, -1));

        QFont f;
        f.setBold(true);
        f.setPixelSize(18);
        painter.setFont(f);
        painter.drawText(this->rect(), Qt::AlignCenter, _name.at(0));
    }
    else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);
        QPainterPath path;
        path.addEllipse(this->rect());
        painter.setClipPath(path);
        painter.drawPixmap(this->rect(), _avatar);
    }
}
