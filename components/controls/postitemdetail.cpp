#include "postitemdetail.h"

#include "avatarwidget.h"
#include "icontext.h"

#include "ElaText.h"

PostItemDetail::PostItemDetail(const PostData &data, QWidget *parent)
    : PostItem(data, parent)
    , _commentEdit(nullptr)
    , _commentListWid(nullptr)
    , _commentListLayout(nullptr)
{
    initCommentArea();
}

void PostItemDetail::setCommentList(const QList<ReplyCommentData> &comments)
{
    _comments = comments;
    updateCommentList();
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

    // 第一行：avatar + ElaLineEdit
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

    inputLayout->addWidget(inputAvatar);
    inputLayout->addWidget(commentEdit);
    inputWid->setLayout(inputLayout);

    commentLayout->addWidget(inputWid);

    // 第二行：评论列表
    _commentListWid = new QWidget(commentArea);
    _commentListLayout = new QVBoxLayout(_commentListWid);
    _commentListLayout->setContentsMargins(0, 0, 0, 0);
    _commentListLayout->setSpacing(10);

    updateCommentList();

    _commentListWid->setLayout(_commentListLayout);

    commentLayout->addWidget(_commentListWid);
    commentArea->setLayout(commentLayout);

    mainLayout->addWidget(commentArea);
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

    for (const ReplyCommentData& comment : _comments) {
        QWidget* commentWidget = createCommentWidget(comment, 0);
        _commentWidgets.append(commentWidget);
        _commentListLayout->addWidget(commentWidget);
    }
    _commentListLayout->addStretch();
}

QWidget* PostItemDetail::createCommentWidget(const ReplyCommentData &comment, int indent)
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(indent * 30, 10, 0, 0);
    layout->setSpacing(4);

    // 第一行：avatar + name + date
    QWidget* topWid = new QWidget(widget);
    QHBoxLayout* topLayout = new QHBoxLayout(topWid);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(8);

    AvatarWidget* avatar = new AvatarWidget(topWid);
    avatar->setFixedSize(40, 40);
    avatar->setPixeSize(15);
    if (!comment.avatar.isEmpty()) {
        avatar->setAvatar(QPixmap(comment.avatar));
    }
    avatar->setName(comment.name);

    QWidget* nameDateWid = new QWidget(topWid);
    QVBoxLayout* nameDateLayout = new QVBoxLayout(nameDateWid);
    nameDateLayout->setContentsMargins(0, 0, 0, 0);
    nameDateLayout->setSpacing(0);

    ElaText* nameText = new ElaText(comment.name, nameDateWid);
    QFont nameFont = nameText->font();
    nameFont.setPixelSize(12);
    nameFont.setBold(true);
    nameText->setFont(nameFont);
    nameText->setIsWrapAnywhere(false);

    IconText* dateText = new IconText(comment.date, nameDateWid);
    dateText->setPixelSize(10);

    nameDateLayout->addWidget(nameText);
    nameDateLayout->addWidget(dateText);
    nameDateWid->setLayout(nameDateLayout);

    topLayout->addWidget(avatar);
    topLayout->addWidget(nameDateWid);
    topLayout->addStretch();
    topWid->setLayout(topLayout);

    layout->addWidget(topWid);

    // 第二行：content（与 name 平行对齐）
    ElaText* contentText = new ElaText(comment.content, widget);
    QFont contentFont = contentText->font();
    contentFont.setPixelSize(12);
    contentText->setFont(contentFont);
    contentText->setWordWrap(true);
    contentText->setIsWrapAnywhere(false);

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(8);
    contentLayout->addSpacing(40 + 8); // avatar 宽度 + spacing
    contentLayout->addWidget(contentText);

    layout->addLayout(contentLayout);

    // 回复列表（递归，indent固定为1）
    for (const ReplyCommentData& reply : comment.replys) {
        QWidget* replyWidget = createCommentWidget(reply, 1);
        _commentWidgets.append(replyWidget);
        layout->addWidget(replyWidget);
    }

    widget->setLayout(layout);
    return widget;
}
