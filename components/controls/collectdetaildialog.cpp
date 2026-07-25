#include "collectdetaildialog.h"

#include <QVBoxLayout>

#include "postitemdetail.h"

#include "ElaImageCard.h"

CollectDetailDialog::CollectDetailDialog(const CollectDetailData &data, QWidget *parent)
    : ElaDialog(parent)
    , _data(data)
    , _avatar(nullptr)
    , _name(nullptr)
    , _date(nullptr)
    , _deleteBtn(nullptr)
    , _scrollArea(nullptr)
    , _replyEditArea(nullptr)
{
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint |
                         ElaAppBarType::MinimizeButtonHint |
                         ElaAppBarType::MaximizeButtonHint |
                         ElaAppBarType::StayTopButtonHint);

    initContent();
}

CollectDetailDialog::~CollectDetailDialog()
{
}

void CollectDetailDialog::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* headerRow = new QWidget(this);
    QHBoxLayout* headerLayout = new QHBoxLayout(headerRow);
    headerLayout->setContentsMargins(20, 15, 20, 15);
    headerLayout->setSpacing(10);

    _avatar = new AvatarWidget(headerRow);
    _avatar->setFixedSize(40, 40);
    _avatar->setPixeSize(15);

    QWidget* nameDateWid = new QWidget(headerRow);
    QVBoxLayout* nameDateLayout = new QVBoxLayout(nameDateWid);
    nameDateLayout->setContentsMargins(0, 0, 0, 0);
    nameDateLayout->setSpacing(2);

    _name = new ElaText(nameDateWid);
    QFont nameFont = _name->font();
    nameFont.setPixelSize(14);
    nameFont.setBold(true);
    _name->setFont(nameFont);

    _date = new IconText(nameDateWid);
    _date->setPixelSize(11);

    nameDateLayout->addWidget(_name);
    nameDateLayout->addWidget(_date);
    nameDateWid->setLayout(nameDateLayout);

    _deleteBtn = new ElaPushButton("删除", headerRow);
    _deleteBtn->setBorderRadius(8);
    _deleteBtn->setFixedWidth(80);

    headerLayout->addWidget(_avatar);
    headerLayout->addWidget(nameDateWid, 1);
    headerLayout->addWidget(_deleteBtn);
    headerRow->setLayout(headerLayout);

    _scrollArea = new ElaScrollArea(this);

    mainLayout->addWidget(headerRow);
    mainLayout->addWidget(_scrollArea, 1);
    setLayout(mainLayout);

    _replyEditArea = new ReplyEditArea(this);

    updateContent();
}

void CollectDetailDialog::updateContent()
{
    if (!_data.avatar.isNull()) {
        _avatar->setAvatar(_data.avatar);
    }
    _avatar->setName(_data.name);

    _name->setText(_data.name);
    _date->setText("收藏于: " + _data.date + "  来自: " + _data.name);
}

void CollectDetailDialog::initChatMsgDetail()
{
    if (_scrollArea->widget()) {
        _scrollArea->widget()->deleteLater();
    }

    QWidget* scrollContent = new QWidget(_scrollArea);
    scrollContent->setStyleSheet("background-color: transparent");
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setContentsMargins(20, 10, 20, 10);
    scrollLayout->setSpacing(10);
    scrollContent->setLayout(scrollLayout);

    ElaText* contentText = new ElaText(_data.content, scrollContent);
    contentText->setTextStyle(ElaTextType::Body);

    scrollLayout->addWidget(contentText);

    for (int i = 0; i < _data.imgList.count(); i++) {
        ElaImageCard* imgCard = new ElaImageCard(scrollContent);
        imgCard->setCardImage(_data.imgList[i].toImage());
        scrollLayout->addWidget(imgCard);
    }

    _scrollArea->setWidget(scrollContent);
    _scrollArea->setWidgetResizable(true);
}

void CollectDetailDialog::initPostDetail()
{
    if (_scrollArea->widget()) {
        _scrollArea->widget()->deleteLater();
    }

    QWidget* scrollContent = new QWidget(_scrollArea);
    scrollContent->setStyleSheet("background-color: transparent");
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setContentsMargins(20, 0, 20, 20);
    scrollLayout->setSpacing(0);
    scrollContent->setLayout(scrollLayout);

    PostData data(
        "ee268",
        "123123",
        "today",
        QPixmap(),
        {QPixmap(":/resource/image/avatar.jpg"), QPixmap(":/resource/image/rupa.jpg"), QPixmap(":/resource/image/rupa.jpg")},
        "今天天气真不错！！！！！！！！！！！！！！！！！！！",
        99, 99 ,99, "123"
        );

    PostItemDetail* postDetail = new PostItemDetail(data, scrollContent);
    scrollLayout->addWidget(postDetail);

    connect(postDetail, &PostItemDetail::sigReplyButtonClicked, this, [this](auto commentData){
        _replyEditArea->showAnimation();
        _replyEditArea->setReplyCommentData(commentData);
    });

    connect(_replyEditArea, &ReplyEditArea::sigSendReplyBtnClicked, postDetail, &PostItemDetail::slotSendReply);

    _scrollArea->setWidget(scrollContent);
    _scrollArea->setWidgetResizable(true);
}

void CollectDetailDialog::updateData(const CollectDetailData &data)
{
    _data = data;

    updateContent();
}

void CollectDetailDialog::resizeEvent(QResizeEvent *event)
{
    if (_replyEditArea->getIsShow()) {
        _replyEditArea->setIsShow(false);
        _replyEditArea->showAnimation();
    }

    ElaDialog::resizeEvent(event);
}

void CollectDetailDialog::showEvent(QShowEvent * e)
{
    if (_data.type == CollectType::ChatMessage) {
        initChatMsgDetail();
    }
    else if (_data.type == CollectType::SpacePost) {
        initPostDetail();
    }

    ElaDialog::showEvent(e);
}
