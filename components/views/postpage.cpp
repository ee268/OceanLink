#include "postpage.h"

#include <QVBoxLayout>
#include <QPainter>
#include <QEvent>

#include "ElaTheme.h"
#include "ElaIcon.h"
#include "ElaText.h"

PostPage::PostPage(QWidget *parent/* = nullptr*/)
    : BasePage(parent)
{
    initDefaultLayout();
    initLeftWidget();
}

void PostPage::initLeftWidget()
{
    QWidget* leftWid = this->getLeftWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(leftWid);

    // QWidget* wid = new QWidget(leftWid);
    // QVBoxLayout* widLayout = new QVBoxLayout(wid);

    // ElaText* title = new ElaText("好友空间", wid);
    // title->setTextStyle(ElaTextType::Subtitle);
    // title->setIsWrapAnywhere(false);

    // widLayout->setContentsMargins(15, 15, 15, 15);
    // widLayout->setSpacing(0);
    // widLayout->addWidget(title, 0, Qt::AlignLeft);
    // wid->setLayout(widLayout);

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
    // mainLayout->addWidget(wid);
    mainLayout->addWidget(btnWid);
    mainLayout->addWidget(_spaceList);
    leftWid->setLayout(mainLayout);
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
