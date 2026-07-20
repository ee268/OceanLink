#include "postpage.h"

#include <QVBoxLayout>
#include <QPainter>
#include <QEvent>

#include "ElaTheme.h"
#include "ElaIcon.h"
#include "ElaScrollArea.h"

PostPage::PostPage(QWidget *parent/* = nullptr*/)
    : BasePage(parent)
    , _postDetail(nullptr)
{
    initDefaultLayout();
    initLeftWidget();
    initRightWidget();
}

void PostPage::initLeftWidget()
{
    QWidget* leftWid = this->getLeftWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(leftWid);

    _spaceList = new SpaceList(this);

    EveryoneButton* everyButton = new EveryoneButton(
        "所有人", ElaIcon::getInstance()->getElaIcon(ElaIconType::ListUl), this);
    everyButton->setFixedHeight(35);

    QWidget* btnWid = new QWidget(leftWid);
    QHBoxLayout* btnLayout = new QHBoxLayout(btnWid);
    btnLayout->setContentsMargins(10, 10, 10, 10);
    btnLayout->addWidget(everyButton);
    btnWid->setLayout(btnLayout);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(btnWid);
    mainLayout->addWidget(_spaceList);
    leftWid->setLayout(mainLayout);
}

void PostPage::initRightWidget()
{
    QStackedWidget* stackedWid = this->getStackedWidget();
    QWidget* wid = new QWidget(stackedWid);
    QVBoxLayout* widLayout = new QVBoxLayout(wid);

    _postList = new PostList(wid);

    widLayout->setContentsMargins(0, 0, 0, 0);
    widLayout->setSpacing(0);
    widLayout->addWidget(_postList);

    ElaScrollArea* postScrollArea = new ElaScrollArea(stackedWid);
    QWidget* postWid = new QWidget(this);
    QVBoxLayout* postlayout = new QVBoxLayout(postWid);
    postlayout->setContentsMargins(20, 20, 20, 20);
    postlayout->setSpacing(0);

    _postDetail = new PostItemDetail(PostData(), postScrollArea);
    postlayout->addWidget(_postDetail);
    postWid->setLayout(postlayout);

    postScrollArea->setWidget(postWid);
    postScrollArea->setWidgetResizable(true);

    stackedWid->addWidget(wid);
    stackedWid->addWidget(postScrollArea);
    stackedWid->setCurrentIndex(1);

    connect(_postList, &PostList::sigPostItemClicked, this, &PostPage::slotPostItemClicked);
}

void PostPage::slotPostItemClicked(const PostData &data)
{
    _postDetail->updateData(data);

    QList<ReplyCommentData> comments;
    QStringList names = {"张三", "李四", "王五", "赵六", "钱七",
                         "孙八", "周九", "吴十", "郑十一", "王十二"};
    QStringList contents = {
        "说得好！",
        "支持一下！",
        "写得真不错，继续加油。",
        "有道理，学到了。",
        "哈哈哈，太有趣了。",
        "感谢分享！",
        "这个观点很新颖。",
        "我也这么认为。",
        "很棒的文章！",
        "期待更多更新。"
    };

    for (int i = 0; i < 10; i++) {
        ReplyCommentData comment(names[i], "", "2026-07-20", contents[i]);
        if (i % 3 == 0) {
            ReplyCommentData reply("回复者", "", "2026-07-20", "回复：" + contents[i]);
            comment.replys.push_back(reply);
        }
        comments.append(comment);
    }

    _postDetail->setCommentList(comments);
    this->getStackedWidget()->setCurrentIndex(2);
}

EveryoneButton::EveryoneButton(const QString &text, const QIcon &icon, QWidget *parent)
    : QPushButton(text, parent)
    , _text(text)
    , _icon(icon)
{
    QFont f = this->font();
    f.setPixelSize(13);
    this->setFont(f);
    installEventFilter(this);
}

void EveryoneButton::setFixedSize(int w, int h)
{
    QPushButton::setFixedSize(w, h);
    update();
}

bool EveryoneButton::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this) {
        if (event->type() == QEvent::Enter) {
            setCursor(Qt::PointingHandCursor);
            _state = Hover;
            update();
        } else if (event->type() == QEvent::Leave) {
            unsetCursor();
            _state = Normal;
            update();
        } else if (event->type() == QEvent::MouseButtonPress) {
            _state = Pressed;
            update();
        } else if (event->type() == QEvent::MouseButtonRelease) {
            _state = Hover;
            update();
        }
    }
    return QPushButton::eventFilter(obj, event);
}

void EveryoneButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    int spacing = 10;
    int iconSize = 22;
    QFont f = this->font();
    f.setPixelSize(iconSize - 7);
    this->setFont(f);
    int textWidth = this->fontMetrics().horizontalAdvance(_text);
    int totalWidth = iconSize + spacing + textWidth;
    int startX = (this->width() - totalWidth) / 2;
    int iconY = (this->height() - iconSize) / 2;

    QRect iconRect(startX, iconY, iconSize, iconSize);

    painter.setPen(QPen(ElaThemeColor(eTheme->getThemeMode(), BasicBorder), 1));

    if (_state == Pressed) {
        painter.setBrush(ElaThemeColor(eTheme->getThemeMode(), BasicPress));
    } else if (_state == Hover) {
        painter.setBrush(ElaThemeColor(eTheme->getThemeMode(), BasicHover));
    } else {
        painter.setBrush(ElaThemeColor(eTheme->getThemeMode(), BasicBase));
    }
    painter.drawRoundedRect(this->rect().adjusted(1, 1, -1, -1), 8, 8);

    painter.setBrush(Qt::NoBrush);
    painter.setFont(f);

    QPixmap iconPixmap = _icon.pixmap(iconSize, iconSize);
    QPainter pmPainter(&iconPixmap);
    pmPainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    pmPainter.fillRect(iconPixmap.rect(), ElaThemeColor(eTheme->getThemeMode(), BasicText));
    pmPainter.end();
    painter.drawPixmap(iconRect, iconPixmap);

    QRect textRect(iconRect.right() + spacing,
                   this->rect().top(),
                   textWidth,
                   this->height());
    painter.setPen(ElaThemeColor(eTheme->getThemeMode(), BasicText));
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, _text);
}
