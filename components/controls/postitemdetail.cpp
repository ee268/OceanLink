#include "postitemdetail.h"

#include <QScrollArea>
#include <QMouseEvent>
#include <QDebug>
#include <QEvent>

#include "avatarwidget.h"
#include "icontext.h"
#include "themecolorbutton.h"

#include "ElaText.h"

CommentWidget::CommentWidget(std::shared_ptr<ReplyCommentData> data, int indent, QWidget *parent)
    : QWidget(parent)
    , _replyText(nullptr)
    , _data(data)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(indent * 30, 10, 0, 0);
    layout->setSpacing(4);

    //头像、姓名、日期、回复
    QWidget* topWid = new QWidget(this);
    QHBoxLayout* topLayout = new QHBoxLayout(topWid);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(8);

    AvatarWidget* avatar = new AvatarWidget(topWid);
    avatar->setFixedSize(40, 40);
    avatar->setPixeSize(15);
    if (!data->avatar.isNull()) {
        avatar->setAvatar(data->avatar);
    }
    avatar->setName(data->name);

    QWidget* nameDateWid = new QWidget(topWid);
    QVBoxLayout* nameDateLayout = new QVBoxLayout(nameDateWid);
    nameDateLayout->setContentsMargins(0, 0, 0, 0);
    nameDateLayout->setSpacing(0);

    ElaText* nameText = new ElaText(data->name, nameDateWid);
    QFont nameFont = nameText->font();
    nameFont.setPixelSize(12);
    nameFont.setBold(true);
    nameText->setFont(nameFont);
    nameText->setIsWrapAnywhere(false);

    QWidget* dateReplyWid = new QWidget(topWid);
    QHBoxLayout* dateReplyLayout = new QHBoxLayout(dateReplyWid);
    dateReplyLayout->setContentsMargins(0, 0, 0, 0);
    dateReplyLayout->setSpacing(8);


    IconText* dateText = new IconText(data->date, nameDateWid);
    dateText->setPixelSize(10);

    IconText* replyText = new IconText("回复", topWid);
    _replyText = replyText;
    replyText->setPixelSize(10);
    replyText->setVisible(false);

    dateReplyLayout->addWidget(dateText);
    dateReplyLayout->addWidget(replyText);
    dateReplyLayout->addStretch();
    dateReplyWid->setLayout(dateReplyLayout);

    nameDateLayout->addWidget(nameText);
    nameDateLayout->addWidget(dateReplyWid);
    nameDateWid->setLayout(nameDateLayout);

    topLayout->addWidget(avatar);
    topLayout->addWidget(nameDateWid);
    topWid->setLayout(topLayout);

    layout->addWidget(topWid);

    //评论内容
    ElaText* contentText = new ElaText(data->content, this);
    QFont contentFont = contentText->font();
    contentFont.setPixelSize(13);
    contentText->setFont(contentFont);
    contentText->setWordWrap(true);
    contentText->setIsWrapAnywhere(true);

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(8);
    contentLayout->addSpacing(40 + 8); //avatar大小 + spacing
    contentLayout->addWidget(contentText);

    layout->addLayout(contentLayout);

    setLayout(layout);

    this->installEventFilter(this);
}

bool CommentWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this) {
        if (event->type() == QEvent::Enter) {
            _replyText->setVisible(true);
            _replyText->setCursor(Qt::PointingHandCursor);
        }
        else if (event->type() == QEvent::Leave) {
            _replyText->setVisible(false);
            _replyText->setCursor(Qt::ArrowCursor);
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mEvent = dynamic_cast<QMouseEvent*>(event);
            if (mEvent->button() == Qt::LeftButton) {
                emit sigReplyButtonClicked(_data);
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

PostItemDetail::PostItemDetail(const PostData &data, QWidget *parent)
    : PostItem(data, parent)
    , _commentEdit(nullptr)
    , _commentListWid(nullptr)
    , _commentListLayout(nullptr)
    , _replyDrawer(nullptr)
{
    initCommentArea();
}

void PostItemDetail::setCommentList(std::vector<std::shared_ptr<ReplyCommentData>> comments)
{
    _comments = std::move(comments);
    updateCommentList();
}

void PostItemDetail::addComment(std::shared_ptr<ReplyCommentData> comment)
{
    _comments.push_back(comment);
    CommentWidget* commentWidget = createCommentWidget(comment, 0);
    _commentWidgets.append(commentWidget);
    _commentListLayout->insertWidget(0, commentWidget);
}

void PostItemDetail::updateData(const PostData &data)
{
    PostItem::updateData(data);
    _commentEdit->clear();
    for (QWidget* widget : _commentWidgets) {
        _commentListLayout->removeWidget(widget);
        widget->deleteLater();
    }
    _commentWidgets.clear();
}

void PostItemDetail::initCommentArea()
{
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (!mainLayout)
        return;

    QWidget* commentArea = new QWidget(this);
    QVBoxLayout* commentLayout = new QVBoxLayout(commentArea);
    commentLayout->setContentsMargins(15, 10, 15, 10);
    commentLayout->setSpacing(12);

    //头像、输入框、发送按钮
    QWidget* inputWid = new QWidget(commentArea);
    QHBoxLayout* inputLayout = new QHBoxLayout(inputWid);
    inputLayout->setContentsMargins(0, 0, 0, 0);
    inputLayout->setSpacing(10);

    AvatarWidget* inputAvatar = new AvatarWidget(inputWid);
    inputAvatar->setFixedSize(40, 40);
    inputAvatar->setPixeSize(15);

    ElaLineEdit* commentEdit = new ElaLineEdit(inputWid);
    _commentEdit = commentEdit;
    commentEdit->setPlaceholderText("写下你的评论...");

    ThemeColorButton* sendBtn = new ThemeColorButton("发送", inputWid);

    connect(sendBtn, &ThemeColorButton::clicked, this, &PostItemDetail::slotSendBtnClicked);

    inputLayout->addWidget(inputAvatar);
    inputLayout->addWidget(commentEdit);
    inputLayout->addWidget(sendBtn);
    inputWid->setLayout(inputLayout);

    commentLayout->addWidget(inputWid);

    //评论列表
    _commentListWid = new QWidget(commentArea);
    _commentListLayout = new QVBoxLayout(_commentListWid);
    _commentListLayout->setContentsMargins(0, 0, 0, 0);
    _commentListLayout->setSpacing(10);

    updateCommentList();

    _commentListWid->setLayout(_commentListLayout);

    commentLayout->addWidget(_commentListWid);
    commentArea->setLayout(commentLayout);

    mainLayout->addWidget(commentArea);

    _replyDrawer = new ElaDrawerArea(this);
    _replyDrawer->collapse();
}

void PostItemDetail::updateCommentList()
{
    if (_commentListLayout->count() > 0) {
        for (QWidget* widget : _commentWidgets) {
            _commentListLayout->removeWidget(widget);
            widget->deleteLater();
        }
        _commentWidgets.clear();
    }

    for (const auto& comment : _comments) {
        CommentWidget* commentWidget = createCommentWidget(comment, 0);
        _commentWidgets.append(commentWidget);
        _commentListLayout->addWidget(commentWidget);
    }
    _commentListLayout->addStretch();
}

CommentWidget* PostItemDetail::createCommentWidget(std::shared_ptr<ReplyCommentData> comment, int indent)
{
    CommentWidget* widget = new CommentWidget(comment, indent, this);

    connect(widget, &CommentWidget::sigReplyButtonClicked, this, &PostItemDetail::slotReplyButtonClicked);

    //评论回复列表
    for (const auto& reply : comment->replys) {
        CommentWidget* replyWidget = createCommentWidget(reply, 1);
        _commentWidgets.append(replyWidget);
        widget->layout()->addWidget(replyWidget);
    }

    return widget;
}

void PostItemDetail::slotSendBtnClicked()
{
    QString text = _commentEdit->text();
    if (text.isEmpty()) {
        return;
    }

    auto data = std::make_shared<ReplyCommentData>(
        "ee268",
        QPixmap(":/resource/image/avatar.jpg"),
        "刚刚",
        text
    );

    _commentEdit->clear();
    addComment(data);

    emit sigSendCommentSuccess();
}

void PostItemDetail::slotReplyButtonClicked(std::shared_ptr<ReplyCommentData> data)
{
    qDebug() << "expand";
    _replyDrawer->expand();
}
