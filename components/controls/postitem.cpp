#include "postitem.h"

#include <QVBoxLayout>
#include <QFontMetrics>
#include <QPainter>
#include <QTextDocument>

#include "displaycard.h"

#include "ElaIcon.h"

PostItem::PostItem(const PostData &data, QWidget *parent)
    : QWidget{parent}
    , _data(data)
    , _avatar(nullptr)
    , _nameText(nullptr)
    , _dateText(nullptr)
    , _contentText(nullptr)
    , _promotionView(nullptr)
    , _likeText(nullptr)
    , _commentText(nullptr)
    , _collectText(nullptr)
{
    initContent();
}

void PostItem::updateData(const PostData &data)
{
    _data = data;
    _avatar->setAvatar(_data.avatar);
    _avatar->setName(_data.name);
    _nameText->setText(_data.name);
    _dateText->setText(_data.date);
    _contentText->setText(_data.content);
    _likeText->setText(QString::number(_data.likeCnt));
    _commentText->setText(QString::number(_data.commentCnt));
    _collectText->setText(QString::number(_data.collectCnt));

    if (_promotionView) {
        _promotionView->deleteLater();
        _promotionView = nullptr;
    }
    if (!_data.imgList.isEmpty()) {
        _promotionView = new ElaPromotionView(this);
        _promotionView->setFixedHeight(300);
        for (const QPixmap& img : _data.imgList) {
            DisplayCard* card = new DisplayCard(_promotionView);
            card->setCardPixmap(img);
            card->setBorderRadius(8);
            _promotionView->appendPromotionCard(card);
        }
        qobject_cast<QVBoxLayout*>(this->layout())->insertWidget(2, _promotionView, Qt::AlignHCenter);
    }

    update();
}

void PostItem::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    mainLayout->setSpacing(8);

    // 第一行：avatar + name（左），date（右）
    QWidget* topWid = new QWidget(this);
    QHBoxLayout* topLayout = new QHBoxLayout(topWid);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(8);

    AvatarWidget* avatar = new AvatarWidget(topWid);
    _avatar = avatar;
    avatar->setFixedSize(45, 45);
    avatar->setAvatar(_data.avatar);
    avatar->setName(_data.name);

    ElaText* nameText = new ElaText(_data.name, topWid);
    _nameText = nameText;
    QFont nameFont = nameText->font();
    nameFont.setPixelSize(14);
    nameFont.setBold(true);
    nameText->setFont(nameFont);
    nameText->setIsWrapAnywhere(false);

    IconText* dateText = new IconText(_data.date, topWid);
    _dateText = dateText;
    dateText->setPixelSize(12);

    topLayout->addWidget(avatar);
    topLayout->addWidget(nameText);
    topLayout->addStretch();
    topLayout->addWidget(dateText);
    topWid->setLayout(topLayout);

    mainLayout->addWidget(topWid);

    // 第二行：content（固定高度，超出省略）+ imgList
    QWidget* midWid = new QWidget(this);
    QVBoxLayout* midLayout = new QVBoxLayout(midWid);
    midLayout->setContentsMargins(0, 0, 0, 0);
    midLayout->setSpacing(8);

    ElaText* contentText = new ElaText(midWid);
    _contentText = contentText;
    contentText->setFixedHeight(80);
    QFont contentFont = contentText->font();
    contentFont.setPixelSize(13);
    contentText->setFont(contentFont);
    contentText->setWordWrap(true);
    contentText->setText(_data.content);

    midLayout->addWidget(contentText);

    if (!_data.imgList.isEmpty()) {
        ElaPromotionView* promotionView = new ElaPromotionView(midWid);
        _promotionView = promotionView;
        promotionView->setFixedHeight(300);
        for (const QPixmap& img : _data.imgList) {
            DisplayCard* card = new DisplayCard(promotionView);
            card->setCardPixmap(img);
            card->setBorderRadius(8);
            promotionView->appendPromotionCard(card);
        }
        midLayout->addWidget(promotionView);
    }
    midWid->setLayout(midLayout);

    mainLayout->addWidget(midWid);

    // 第三行：likeCnt、commentCnt、collectCnt
    QWidget* bottomWid = new QWidget(this);
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWid);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(20);

    IconText* likeText = new IconText(QString::number(_data.likeCnt), bottomWid);
    _likeText = likeText;
    likeText->setPixelSize(12);
    likeText->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Heart));
    likeText->setTextColorLight(Qt::black);
    likeText->setTextColorDark(Qt::white);

    IconText* commentText = new IconText(QString::number(_data.commentCnt), bottomWid);
    _commentText = commentText;
    commentText->setPixelSize(12);
    commentText->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Comment));
    commentText->setTextColorLight(Qt::black);
    commentText->setTextColorDark(Qt::white);
    connect(commentText, &IconText::sigClicked, this, [this]() {
        emit sigPostItemClicked(_data);
    });

    IconText* collectText = new IconText(QString::number(_data.collectCnt), bottomWid);
    _collectText = collectText;
    collectText->setPixelSize(12);
    collectText->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Bookmark));
    collectText->setTextColorLight(Qt::black);
    collectText->setTextColorDark(Qt::white);

    bottomLayout->addStretch();
    bottomLayout->addWidget(likeText);
    bottomLayout->addWidget(commentText);
    bottomLayout->addWidget(collectText);
    bottomWid->setLayout(bottomLayout);

    mainLayout->addWidget(bottomWid);

    this->setLayout(mainLayout);
}

void PostItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(eTheme->getThemeMode(), BasicBase));
    painter.drawRoundedRect(this->rect(), 8, 8);

    QWidget::paintEvent(event);
}

void PostItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QFont font = _contentText->font();
    QFontMetrics fm(font);
    int availableWidth = this->width();
    int availableHeight = _contentText->height();

    QTextDocument doc;
    doc.setDefaultFont(font);
    doc.setTextWidth(availableWidth);
    doc.setPlainText(_data.content);

    if (doc.size().height() > availableHeight) {
        QString text = _data.content;
        for (int i = text.length() - 1; i >= 0; i--) {
            doc.setPlainText(text.left(i) + "...");
            if (doc.size().height() <= availableHeight) {
                _contentText->setText(text.left(i) + "...");
                return;
            }
        }
    }
    _contentText->setText(_data.content);
}
