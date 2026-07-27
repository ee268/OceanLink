#include "contactdetailwid.h"

#include <QPainter>
#include <QEvent>
#include <QVBoxLayout>

#include "ElaExponentialBlur.h"
#include "ElaIcon.h"
#include "ElaIconButton.h"

#include "../../controls/splitline.h"
#include "../../../global/global.h"

ContactDetailWid::ContactDetailWid(ContactList* list, QWidget *parent)
    : QWidget(parent)
    , _contactList(list)
    , _avatar(nullptr)
    , _signStatus(false)
{
    initContent();
}

void ContactDetailWid::setIndex(const QModelIndex &index)
{
    _index = index;
    updateInfo();
}

void ContactDetailWid::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    if (!_blurredBg.isNull()) {
        painter.drawPixmap(this->rect(), _blurredBg);
    }

    QWidget::paintEvent(event);
}

void ContactDetailWid::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateBlurredBg();
}

bool ContactDetailWid::eventFilter(QObject *obj, QEvent *event)
{

    if (obj == _spaceWid) {
        if (event->type() == QEvent::Enter) {
            _spaceWid->setCursor(Qt::PointingHandCursor);
        }
        else if (event->type() == QEvent::Leave) {
            _spaceWid->setCursor(Qt::ArrowCursor);
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            // to do
        }
    }

    return QWidget::eventFilter(obj, event);
}

void ContactDetailWid::updateBlurredBg()
{
    QPixmap pixmap(":/resource/image/rupa.jpg");
    if (pixmap.isNull() || this->width() <= 0 || this->height() <= 0)
        return;

    qreal pixmapRatio = (qreal)pixmap.width() / pixmap.height();
    qreal rectRatio = (qreal)this->width() / this->height();
    QRect sourceRect;
    if (pixmapRatio > rectRatio) {
        int srcWidth = pixmap.height() * rectRatio;
        int srcX = (pixmap.width() - srcWidth) / 2;
        sourceRect = QRect(srcX, 0, srcWidth, pixmap.height());
    } else {
        int srcHeight = pixmap.width() / rectRatio;
        int srcY = (pixmap.height() - srcHeight) / 2;
        sourceRect = QRect(0, srcY, pixmap.width(), srcHeight);
    }

    QPixmap cropped = pixmap.copy(sourceRect);
    QPixmap scaled = cropped.scaled(this->size() * devicePixelRatioF(),
                                    Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    _blurredBg = ElaExponentialBlur::doExponentialBlur(scaled.toImage(), 30);
    QPainter overlayPainter(&_blurredBg);
    overlayPainter.fillRect(_blurredBg.rect(), QColor(0, 0, 0, 80));
    overlayPainter.end();
}

void ContactDetailWid::slotUpdateCentralWidStyle()
{
    ElaThemeType::ThemeMode mode = eTheme->getThemeMode();
    _centralWid->setStyleSheet(QString(
                                   "#ContactDetailCentralWid { background-color: %1; border: 1px solid %2; border-radius: 8px; }")
                                   .arg(ElaThemeColor(mode, BasicBase).name())
                                   .arg(ElaThemeColor(mode, BasicBorderDeep).name()));
}

void ContactDetailWid::slotChangedGroup(const QString &text)
{
    _contactList->changeContactGroup(_index, text);
}

void ContactDetailWid::slotEditSignText()
{
    if (!_signStatus) {
        _signStatus = true;
        _sign->setHidden(true);
        _signLineEdit->setHidden(false);
        _signLineEdit->setText(_sign->text());
        _signEditButton->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::FloppyDisk));
    }
    else {
        _signStatus = false;
        _sign->setHidden(false);
        _signLineEdit->setHidden(true);
        _signEditButton->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::PenToSquare));

        ContactData* data = static_cast<ContactData*>(_index.internalPointer());
        data->sign = _signLineEdit->text();
        _sign->setText(_signLineEdit->text());

        _contactList->updateContact(_index);
    }
}

void ContactDetailWid::initContent()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);

    _centralWid = new QWidget(this);
    _centralWid->setFixedWidth(600);
    _centralWid->setObjectName("ContactDetailCentralWid");

    slotUpdateCentralWidStyle();
    connect(eTheme, &ElaTheme::themeModeChanged, this, &ContactDetailWid::slotUpdateCentralWidStyle);

    vLayout->setContentsMargins(0, 30, 0, 0);
    vLayout->addWidget(_centralWid, 0, Qt::AlignCenter);
    vLayout->addStretch();
    this->setLayout(vLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout(_centralWid);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(8);

    SplitLine* line1 = new SplitLine(_centralWid);
    line1->setFixedHeight(1);

    initCard();

    initAccountInfo();

    mainLayout->addWidget(line1);

    initPersonalInfo();

    initFriendInfo();

    _editOrSendButton = new ThemeColorButton(_centralWid);
    _editOrSendButton->setFixedSize(120, 40);

    mainLayout->addWidget(_editOrSendButton, 0, Qt::AlignHCenter);
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
    _avatar = new AvatarWidget(_centralWid);
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

    _nicknameWid = new QWidget(fWid);
    QHBoxLayout* nicknameLayout = new QHBoxLayout(_nicknameWid);
    nicknameLayout->setContentsMargins(0, 0, 0, 0);

    IconText* nicknameTitle  = new IconText("备注", _nicknameWid);
    nicknameTitle->setIcon(QIcon(":/resource/image/contact/detail/create-outline.png"));
    nicknameTitle->setTextColorLight(Qt::black);
    nicknameTitle->setTextColorDark(Qt::white);
    nicknameTitle->setPixelSize(13);
    _nickname = new ElaText(_nicknameWid);
    _nickname->setFont(nicknameTitle->font());

    nicknameLayout->addWidget(nicknameTitle);
    nicknameLayout->addStretch();
    nicknameLayout->addWidget(_nickname);
    _nicknameWid->setLayout(nicknameLayout);

    _groupWid = new QWidget(fWid);
    QHBoxLayout* groupLayout = new QHBoxLayout(_groupWid);
    groupLayout->setContentsMargins(0, 0, 0, 0);

    IconText* groupTitle  = new IconText("好友分组", _groupWid);
    groupTitle->setIcon(QIcon(":/resource/image/contact/detail/people-outline.png"));
    groupTitle->setTextColorLight(Qt::black);
    groupTitle->setTextColorDark(Qt::white);
    groupTitle->setPixelSize(13);
    _friendGroup = new ElaComboBox(_groupWid);
    _friendGroup->setFont(groupTitle->font());

    connect(_friendGroup, &ElaComboBox::currentTextChanged, this, &ContactDetailWid::slotChangedGroup);

    groupLayout->addWidget(groupTitle);
    groupLayout->addStretch();
    groupLayout->addWidget(_friendGroup);
    _groupWid->setLayout(groupLayout);

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
    _signLineEdit = new ElaLineEdit(signWid);
    _signEditButton = new IconButton(signWid);
    _signEditButton->setFixedSize(25, 25);

    _signLineEdit->setFixedHeight(_signEditButton->height());

    connect(_signEditButton, &IconButton::clicked, this, &ContactDetailWid::slotEditSignText);

    signLayout->addWidget(signTitle);
    signLayout->addStretch();
    signLayout->addWidget(_sign);
    signLayout->addWidget(_signLineEdit);
    signLayout->addWidget(_signEditButton);
    signWid->setLayout(signLayout);

    SplitLine* line1 = new SplitLine(_centralWid);
    line1->setFixedHeight(1);

    _spaceWid = new QWidget(fWid);
    QHBoxLayout* spaceLayout = new QHBoxLayout(_spaceWid);
    spaceLayout->setContentsMargins(0, 0, 0, 0);

    IconText* spaceTitle  = new IconText("空间", _spaceWid);
    spaceTitle->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Blog));
    spaceTitle->setTextColorLight(Qt::black);
    spaceTitle->setTextColorDark(Qt::white);
    spaceTitle->setPixelSize(13);
    ElaIconButton* space = new ElaIconButton(ElaIconType::ArrowRightToBracket, _spaceWid);
    space->setFixedWidth(18);
    space->setLightHoverColor(Qt::transparent);
    space->setDarkHoverColor(Qt::transparent);

    spaceLayout->addWidget(spaceTitle);
    spaceLayout->addStretch();
    spaceLayout->addWidget(space);
    _spaceWid->setLayout(spaceLayout);
    _spaceWid->installEventFilter(this);

    fLayout->addWidget(_nicknameWid);
    fLayout->addWidget(_groupWid);
    fLayout->addWidget(signWid);
    fLayout->addWidget(line1);
    fLayout->addWidget(_spaceWid);
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
    QString nickname = _index.data(ContactListModel::Nickname).toString();
    QString group = _index.data(ContactListModel::Group).toString();

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

    _nickname->setText(nickname);

    _friendGroup->blockSignals(true);
    _friendGroup->clear();
    _friendGroup->addItems(_contactList->getGroupNames());
    _friendGroup->setCurrentText(group);
    _friendGroup->blockSignals(false);

    _sign->setText(sign);

    if (group == "我") {
        _editOrSendButton->changeToBasicColor();
        _editOrSendButton->setText("编辑资料");
        _groupWid->setHidden(true);
        _nicknameWid->setHidden(true);
        _signEditButton->setHidden(false);
    }
    else {
        _editOrSendButton->recoverDefault();
        _editOrSendButton->setText("发消息");
        _groupWid->setHidden(false);
        _nicknameWid->setHidden(false);
        _signEditButton->setHidden(true);
    }

    _signStatus = false;
    _signLineEdit->setHidden(true);
    _sign->setHidden(false);
    _signEditButton->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::PenToSquare));
}
