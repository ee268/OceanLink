#include "chatpage.h"

#include <QVBoxLayout>
#include <QAction>
#include <QApplication>
#include <QClipboard>

#include "../controls/iconbutton.h"

#include "ElaImageCard.h"
#include "ElaPushButton.h"
#include "ElaMessageBar.h"

ChatPage::ChatPage(QWidget *parent/* = nullptr*/)
    : BasePage(parent)
    , _sessionList(nullptr)
    , _suggestBox(nullptr)
    , _sessionItemPopup(nullptr)
    , _msgItemPopup(nullptr)
{
    initDefaultLayout();

    initLeftWidget();

    initRightWidget();
}

ChatPage::~ChatPage() {}

void ChatPage::initLeftWidget()
{
    QWidget* leftWid = this->getLeftWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(leftWid);

    _sessionList = new SessionList(leftWid);
    _suggestBox = new ElaSuggestBox(leftWid);
    _suggestBox->setPlaceholderText("搜索");
    IconButton* addFriendButton = new IconButton(QIcon(":/resource/image/chat/add-outline.png"), leftWid);
    addFriendButton->setFixedSize(40, _suggestBox->height());
    addFriendButton->setBorderRadius(6);
    addFriendButton->setIconSize(20);

    QWidget* subWid = new QWidget(leftWid);
    QHBoxLayout* subLayout = new QHBoxLayout(subWid);
    subLayout->setContentsMargins(10, 10, 10, 10);
    subLayout->setSpacing(6);
    subLayout->addWidget(_suggestBox);
    subLayout->addWidget(addFriendButton);
    subWid->setLayout(subLayout);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(subWid);
    mainLayout->addWidget(_sessionList);
    leftWid->setLayout(mainLayout);

    _sessionItemPopup = new MoreOptPopup(this);
    _topKey = "topKey";
    _copyKey = "copykey";
    _markKey = "markKey";
    _openKey = "openKey";
    _notifyKey = "notifyKey";
    _removeKey = "removeKey";
    _banKey = "deleteKey";

    _sessionItemPopup->addOption("置顶",
                         QIcon(":/resource/image/chat/sessionPoup/arrow-up-circle-outline.png"),
                         _topKey, nullptr);
    _sessionItemPopup->addOption("复制账号",
                         QIcon(":/resource/image/chat/sessionPoup/copy-outline.png"),
                         _copyKey, nullptr);
    _sessionItemPopup->addOption("标记已读",
                         QIcon(":/resource/image/chat/sessionPoup/checkmark-circle-outline.png"),
                         _markKey, nullptr);
    _sessionItemPopup->addOption("打开独立窗口",
                         QIcon(":/resource/image/chat/sessionPoup/browsers-outline.png"),
                         _openKey, nullptr);
    _sessionItemPopup->addOption("设置免打扰",
                         QIcon(":/resource/image/chat/sessionPoup/notifications-off-outline.png"),
                         _notifyKey, nullptr);
    _sessionItemPopup->addOption("从消息列表移除",
                         QIcon(":/resource/image/chat/sessionPoup/trash-outline.png"),
                         _removeKey, nullptr);
    _sessionItemPopup->addOption("屏蔽此人消息",
                         QIcon(":/resource/image/chat/sessionPoup/ban-outline.png"),
                         _banKey, nullptr);
    OptionWidget* banOption = _sessionItemPopup->getOptionWidget(_banKey);
    if (banOption) {
        banOption->setFixedColor(Qt::red);
    }

    connect(_sessionList, &SessionList::sigItemRightClicked, this, &ChatPage::slotSessionItemRightClicked);
    connect(_sessionList, &SessionList::clicked, this, &ChatPage::slotOpenMsgList);
}

void ChatPage::initRightWidget()
{
    auto stackedWid = this->getStackedWidget();
    QWidget* rightWid = new QWidget(stackedWid);
    QVBoxLayout* mainLayout = new QVBoxLayout(rightWid);

    QWidget* wid = new QWidget(rightWid);
    QHBoxLayout* widLayout = new QHBoxLayout(wid);
    //标题区域
    _name = new ElaText(wid);
    _statusIcon = new ElaImageCard(wid);
    _statusText = new ElaText(wid);
    //聊天信息列表
    _msgList = new MessageList(rightWid);

    QFont f = _name->font();
    f.setPixelSize(16);
    f.setBold(true);

    _name->setFont(f);
    _name->setIsWrapAnywhere(false);
    _name->setText("用户1231313");

    _statusIcon->setCardImage(QImage(":/resource/image/chat/ellipse-green.svg").
                              scaled(14, 14, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    _statusIcon->setFixedSize(14, 14);

    f.setPixelSize(12);
    f.setBold(false);
    _statusText->setFont(f);
    _statusText->setText("在线");

    widLayout->setContentsMargins(15, 15, 15, 15);
    widLayout->setSpacing(8);
    widLayout->addWidget(_name, 0, Qt::AlignRight | Qt::AlignVCenter);
    widLayout->addWidget(_statusIcon, 1, Qt::AlignVCenter);
    widLayout->addWidget(_statusText, 2 ,Qt::AlignVCenter);
    wid->setLayout(widLayout);

    //信息编辑区
    QWidget* msgEditWid = new QWidget(rightWid);
    msgEditWid->setFixedHeight(200);

    QVBoxLayout* msgEditWidLayout = new QVBoxLayout(msgEditWid);

    //功能区
    QWidget* funcWid = new QWidget(msgEditWid);
    QHBoxLayout* funcWidLayout = new QHBoxLayout(funcWid);
    int iconSize = 22;
    //表情
    IconButton* emoIcon = new IconButton(
        QIcon(":/resource/image/chat/function/happy-outline.png"), funcWid);
    emoIcon->setFixedSize(35, 35);
    emoIcon->setIconSize(iconSize);
    emoIcon->setTransparentBackground(true);
    //图片
    IconButton* imgIcon = new IconButton(
        QIcon(":/resource/image/chat/function/image-outline.png"), funcWid);
    imgIcon->setFixedSize(emoIcon->size());
    imgIcon->setTransparentBackground(true);
    imgIcon->setIconSize(iconSize);
    //文件
    IconButton* fileIcon = new IconButton(
        QIcon(":/resource/image/chat/function/folder-outline.png"), funcWid);
    fileIcon->setFixedSize(emoIcon->size());
    fileIcon->setTransparentBackground(true);
    fileIcon->setIconSize(iconSize);
    //历史
    IconButton* historyIcon = new IconButton(
        QIcon(":/resource/image/chat/function/time-outline.png"), funcWid);
    historyIcon->setFixedSize(emoIcon->size());
    historyIcon->setTransparentBackground(true);
    historyIcon->setIconSize(iconSize);

    funcWidLayout->setContentsMargins(8, 8, 8, 8);
    funcWidLayout->setSpacing(6);
    funcWidLayout->addWidget(emoIcon, 0, Qt::AlignVCenter);
    funcWidLayout->addWidget(imgIcon, 1, Qt::AlignVCenter);
    funcWidLayout->addWidget(fileIcon, 2, Qt::AlignVCenter);
    funcWidLayout->addStretch();
    funcWidLayout->addWidget(historyIcon, 3, Qt::AlignVCenter);
    funcWid->setLayout(funcWidLayout);

    //输入区
    _msgTextEdit = new ElaPlainTextEdit(msgEditWid);
    _msgTextEdit->setObjectName("ChatMsgTextEdit");
    _msgTextEdit->setStyleSheet("#ChatMsgTextEdit{ padding: 0 8 0 8; border: none; background-color: transparent; }");

    //绑定快捷键
    QAction* sendAction = new QAction(_msgTextEdit);
    sendAction->setShortcuts(
        {
            QKeySequence(Qt::CTRL + Qt::Key_Return),//主键盘enter
            QKeySequence(Qt::CTRL + Qt::Key_Enter)//小键盘enter
        }
    );
    _msgTextEdit->addAction(sendAction);

    //发送区
    QWidget* sendWid = new QWidget(msgEditWid);
    QHBoxLayout* sendWidLayout = new QHBoxLayout(sendWid);
    ElaPushButton* sendButton = new ElaPushButton("发送", msgEditWid);
    sendButton->setFixedSize(100, 40);
    sendButton->setLightDefaultColor(QColor("#1a6bf8"));
    sendButton->setLightHoverColor(QColor("#4d8efb"));
    sendButton->setLightPressColor(QColor("#0d5ce0"));
    sendButton->setDarkDefaultColor(QColor("#1a6bf8"));
    sendButton->setDarkHoverColor(QColor("#4d8efb"));
    sendButton->setDarkPressColor(QColor("#0d5ce0"));
    sendButton->setLightTextColor(Qt::white);
    sendButton->setDarkTextColor(Qt::white);

    //快捷键发送
    connect(sendAction, &QAction::triggered, this, &ChatPage::slotClickedSendButton);
    //点击发送
    connect(sendButton, &ElaPushButton::clicked, this, &ChatPage::slotClickedSendButton);

    sendWidLayout->setContentsMargins(5, 5, 5, 5);
    sendWidLayout->addStretch();
    sendWidLayout->addWidget(sendButton);
    sendWid->setLayout(sendWidLayout);

    msgEditWidLayout->setContentsMargins(0, 0, 0, 0);
    msgEditWidLayout->setSpacing(0);
    msgEditWidLayout->addWidget(funcWid);
    msgEditWidLayout->addWidget(_msgTextEdit);
    msgEditWidLayout->addWidget(sendWid);
    msgEditWid->setLayout(msgEditWidLayout);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(wid);
    mainLayout->addWidget(_msgList);
    mainLayout->addWidget(msgEditWid);
    rightWid->setLayout(mainLayout);

    stackedWid->addWidget(rightWid);

    initRightWidPopup();
}

void ChatPage::initRightWidPopup()
{
    _msgItemPopup = new MoreOptPopup(this);
    _msgCopyKey = "msgCopyKey";
    _msgShareKey = "msgShareKey";
    _msgCollectKey = "msgCollectKey";

    _msgItemPopup->addOption("复制",
                             QIcon(":/resource/image/chat/sessionPoup/copy-outline.png"),
                             _msgCopyKey,
                             [](const QModelIndex& index){
                                 QString msg = index.data(MessageListModel::Msg).toString();
                                 QApplication::clipboard()->setText(msg);
                                 ElaMessageBar::success(ElaMessageBarType::Top, "Success", "复制成功", 2000);
                             });
    _msgItemPopup->addOption("转发",
                             QIcon(":/resource/image/chat/msgPopup/arrow-redo-outline.png"),
                             _msgShareKey,
                             [](const QModelIndex& index){
                                 // TODO: 转发
                             });
    _msgItemPopup->addOption("收藏",
                             QIcon(":/resource/image/chat/msgPopup/bookmark-outline.png"),
                             _msgCollectKey,
                             [](const QModelIndex& index){
                                 // TODO: 收藏
                             });

    connect(_msgList, &MessageList::sigItemRightClicked,
            this, &ChatPage::slotMsgItemRightClicked);
}

void ChatPage::slotSessionItemRightClicked(const QModelIndex &index)
{
    _sessionItemPopup->show();
}

void ChatPage::slotOpenMsgList(const QModelIndex &index)
{
    QString name = index.data(MessageListModel::Name).toString();
    _name->setText(name);
    this->getStackedWidget()->setCurrentIndex(1);
}

void ChatPage::slotClickedSendButton()
{
    QString text = _msgTextEdit->toPlainText();

    if (text > 0) {
        auto msgModel = _msgList->getModel();
        MessageItem item;
        item.isSelf = true;
        item.msg = text;
        item.name = "XunLink";
        msgModel->addItem(item);

        _msgTextEdit->clear();
    }
}

void ChatPage::slotMsgItemRightClicked(const QModelIndex &index)
{
    _msgItemPopup->setCurrentIndex(index);
    _msgItemPopup->show();
}
