#include "groupmgrlist.h"

#include <QPainter>
#include <QEvent>

#include "ElaTheme.h"
#include "ElaScrollBar.h"

GroupMgrList::GroupMgrList(QWidget *parent)
    : ElaScrollArea{parent}
{
    initContent();
}

void GroupMgrList::addData(const FriendGroupData &data)
{
    GroupMgrItem* item = new GroupMgrItem(data, this);
    item->setFixedHeight(60);
    _mainLayout->addWidget(item);

    _dataList.append(data);
    _itemList.append(item);
}

void GroupMgrList::removeData(const FriendGroupData &data)
{
    int i = findData(data);

    if (i >= 0) {
        _dataList.removeAt(i);

        _mainLayout->removeWidget(_itemList[i]);
        _itemList[i]->deleteLater();
        _itemList.removeAt(i);
    }
}

void GroupMgrList::updateData(const FriendGroupData &data)
{
    int i = findData(data);

    if (i >= 0) {
        _itemList[i]->updateData(data);
    }
}

void GroupMgrList::setAllChecked(bool isAllChecked)
{
    for (auto item : _itemList) {
        item->setChecked(isAllChecked);
        item->update();
    }
}

void GroupMgrList::initContent()
{
    QWidget* mainWid = new QWidget(this);
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(0);
    mainWid->setLayout(_mainLayout);

    this->setWidget(mainWid);
    this->setWidgetResizable(true);

    ElaScrollBar* scrollBar = new ElaScrollBar(this->verticalScrollBar(), this);
    scrollBar->setIsAnimation(true);

    QStringList names = {
        "zhangsan", "lisi", "wangwu", "zhaoliu", "qianqi",
        "sunba", "zhoujiu", "wushi", "zhengshiyi", "wangshier",
        "liushisan", "chenshisi", "linshiwu", "huangshiliu", "yangshiqi",
        "shiba", "liushijiu", "ershi", "ershiyi", "ershier"
    };

    QStringList nickNames = {
        "张三", "李四", "王五", "赵六", "钱七",
        "孙八", "周九", "吴十", "郑十一", "王十二",
        "刘十三", "陈十四", "林十五", "黄十六", "杨十七",
        "十八", "刘十九", "二十", "二十一", "二十二"
    };

    QStringList groupNames = {
        "家人", "朋友", "同事", "同学", "好友",
        "家人", "朋友", "同事", "同学", "好友",
        "家人", "朋友", "同事", "同学", "好友",
        "家人", "朋友", "同事", "同学", "好友"
    };

    for (int i = 0; i < 20; i++) {
        FriendGroupData data;
        data.name = names[i];
        data.nickName = nickNames[i];
        data.avatar = (i % 3 == 0) ? QPixmap(":/resource/image/avatar.jpg") : QPixmap();
        data.groupName = groupNames[i];

        addData(data);
    }
}

int GroupMgrList::findData(const FriendGroupData &data)
{
    for (int i = 0; i < _dataList.count(); i++) {
        if (_dataList[i] == data) {
            return i;
        }
    }

    return -1;
}

GroupMgrItem::GroupMgrItem(FriendGroupData data, QWidget *parent)
    : QWidget(parent)
    , _data(data)
    , _state(Normal)
{
    initContent();

    installEventFilter(this);
}

void GroupMgrItem::updateData(FriendGroupData data)
{
    _data = data;

    _avatarWid->setAvatar(_data.avatar);
    _avatarWid->setName(_data.name);

    _name->setText(_data.name);
    _nickName->setText(_data.nickName);

    _comboBox->addItems({"家人", "家人", "家人"});
}

void GroupMgrItem::setBackGroundColor(QColor color)
{
    _bgColor = color;
    update();
}

void GroupMgrItem::setChecked(bool isChecked)
{
    _radioBtn->setChecked(isChecked);
    if (isChecked) {
        _state = Selected;
    }
    else {
        _state = Normal;
    }
}

bool GroupMgrItem::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this) {
        if (event->type() == QEvent::Enter) {
            if (_state != Selected) {
                _state = Hover;
            }
            update();
        }
        else if (event->type() == QEvent::Leave) {
            if (_state != Selected) {
                _state = Normal;
            }
            update();
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            if (_state != Selected) {
                _state = Selected;
                _radioBtn->setChecked(true);
            }
            else {
                _radioBtn->setChecked(false);
                _state = Hover;
            }
            update();
        }
    }

    return QWidget::eventFilter(obj, event);
}

void GroupMgrItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    switch (_state) {
    case Selected:
        painter.fillRect(this->rect(), ElaThemeColor(eTheme->getThemeMode(), BasicPress));

        break;
    case Hover:
        painter.fillRect(this->rect(), ElaThemeColor(eTheme->getThemeMode(), BasicHover));

        break;
    case Normal:
        if (_bgColor.isValid()) {
            painter.fillRect(this->rect(), _bgColor);
        }
        else {
            painter.fillRect(this->rect(), ElaThemeColor(eTheme->getThemeMode(), BasicBase));
        }
    }

    QWidget::paintEvent(event);
}

void GroupMgrItem::initContent()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    _radioBtn = new ElaRadioButton(this);

    connect(_radioBtn, &ElaRadioButton::clicked, this, &GroupMgrItem::slotClickedRadioBtn);

    QWidget* nameWid = new QWidget(this);
    QHBoxLayout* nameLayout = new QHBoxLayout(nameWid);
    _avatarWid = new AvatarWidget(nameWid);
    _avatarWid->setAvatar(_data.avatar);
    _avatarWid->setName(_data.name);
    _avatarWid->setFixedSize(35, 35);
    _name = new ElaText(_data.name, nameWid);
    _name->setTextStyle(ElaTextType::Body);

    nameLayout->setContentsMargins(0, 0, 0, 0);
    nameLayout->setSpacing(7);
    nameLayout->addWidget(_avatarWid);
    nameLayout->addWidget(_name);
    nameWid->setLayout(nameLayout);

    _nickName = new ElaText(_data.nickName, this);
    _nickName->setTextStyle(ElaTextType::Body);

    _comboBox = new ElaComboBox(this);
    _comboBox->addItems({"家人", "家人", "家人"});

    mainLayout->addWidget(_radioBtn, 0, Qt::AlignLeft);
    mainLayout->addWidget(nameWid, 1, Qt::AlignLeft);
    mainLayout->addWidget(_nickName, 1, Qt::AlignLeft);
    mainLayout->addWidget(_comboBox, 2, Qt::AlignRight);

    this->setLayout(mainLayout);
}

void GroupMgrItem::slotClickedRadioBtn()
{
    if (_radioBtn->isChecked()) {
        _state = Selected;
    }
    else {
        _state = Hover;
    }
    update();
}
