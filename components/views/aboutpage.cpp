#include "aboutpage.h"
#include "../../global/global.h"
#include "../controls/icontext.h"

#include <QVBoxLayout>
#include <QIcon>
#include <QDateTime>
#include <QSpacerItem>
#include <QPainter>
#include <QDebug>

#include "ElaImageCard.h"
#include "ElaTheme.h"

AboutPage::AboutPage(QWidget *parent)
    : ElaDialog(parent)
{
    initDialog();
}

AboutPage::~AboutPage() {}

void AboutPage::initDialog()
{
    setWindowTitle("");
    setWindowIcon(QIcon());
    setWindowButtonFlags(ElaAppBarType::MinimizeButtonHint | ElaAppBarType::CloseButtonHint);
    setFixedSize(350, 500);
    setSizeGripEnabled(false);
    setIsFixedSize(true);

    QImage logoImg = QImage(":/resource/image/logo/OceanLink.png");
    float ratio = 0.40;
    int w = logoImg.width() * ratio;
    int h = logoImg.height() * ratio;
    ElaImageCard* logoCard = new ElaImageCard(this);
    logoCard->setCardImage(logoImg);
    logoCard->setFixedSize(QSize(w, h));

    QString versionText = QString("版本: %1 (64位)").arg(PROJECT_VERSION);
    AboutInfoWidget* versionInfo = new AboutInfoWidget(
        QIcon(":/resource/image/about/code-slash-outline.png"), versionText, this);
    versionInfo->setFixedSize(this->width() - 40, versionInfo->getImplicitIconSize() + 20);
    versionInfo->setTextBold(true);
    versionInfo->setTextColor(QColor("#1e87ef"));

    QString copyrightText;
    QString beginYear = "2026";
    QString curYear = QString::number(QDateTime::currentDateTime().date().year());
    if (curYear == beginYear) {
        curYear = "now";
    }
    copyrightText = QString("Copyright © %1-%2 ee268.")
                               .arg(beginYear)
                               .arg(curYear);
    AboutInfoWidget* copyrightInfo = new AboutInfoWidget(
        QIcon(":/resource/image/about/easel-outline.png"), copyrightText, this);
    copyrightInfo->setFixedSize(versionInfo->size());

    AboutInfoWidget* licenseInfo = new AboutInfoWidget(
        QIcon(":/resource/image/about/shield-checkmark-outline.png"), "MIT LICENSE", this);
    licenseInfo->setFixedSize(versionInfo->size());
    licenseInfo->setTextColor(QColor("#1e87ef"));

    QWidget* infoWid = new QWidget(this);
    QVBoxLayout* infoWidLayout = new QVBoxLayout(infoWid);

    infoWidLayout->setContentsMargins(0, 0, 0, 0);
    infoWidLayout->setSpacing(10);
    infoWidLayout->addWidget(versionInfo, 0, Qt::AlignHCenter);
    infoWidLayout->addWidget(copyrightInfo, 1, Qt::AlignHCenter);
    infoWidLayout->addWidget(licenseInfo, 2, Qt::AlignHCenter);
    infoWid->setLayout(infoWidLayout);

    IconText* techInfo = new IconText("基于Qt 5.15 Widgets + ElaWidgetTools搭建", this);
    IconText* githubInfo = new IconText(QString("https://github.com/ee268/%1").arg(PROjECT_NAME), this);
    githubInfo->setIcon(QIcon(":/resource/image/about/logo-github.svg"));
    githubInfo->setTextColor(QColor("#2789f3"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 15, 0, 15);
    mainLayout->addWidget(logoCard, 0, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(infoWid);
    mainLayout->addSpacerItem(new QSpacerItem(0, 80, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(techInfo, 4, Qt::AlignHCenter);
    mainLayout->addWidget(githubInfo, 5, Qt::AlignHCenter);
    mainLayout->setSpacing(5);

    this->setLayout(mainLayout);
}

AboutInfoWidget::AboutInfoWidget(const QIcon &icon, const QString &text, QWidget *parent)
    : QWidget(parent)
    , _icon(icon)
    , _text(text)
    , _iconBgColor(QColor(227, 239, 254))
    , _iconColor(QColor("#2896f9"))
    , _iconSize(25)
    , _leftMargin(15)
    , _spacing(15)
    , _borderRadius(10)
{
    _textFont.setPixelSize(13);
}

int AboutInfoWidget::getImplicitIconSize() const
{
    return _iconSize + 5;
}

void AboutInfoWidget::setTextColor(const QColor& color)
{
    _textColor = color;
    update();
}

void AboutInfoWidget::setTextBold(bool isBold)
{
    _textFont.setBold(isBold);
    update();
}

void AboutInfoWidget::paintEvent(QPaintEvent *event)
{
    ElaThemeType::ThemeMode themeMode = eTheme->getThemeMode();
    QPainter painter(this);
    QRect thisRect = this->rect();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    painter.setPen(ElaThemeColor(themeMode, BasicBorder));
    painter.setBrush(ElaThemeColor(themeMode, BasicBase));
    painter.drawRoundedRect(thisRect.adjusted(1, 1, -1, -1), _borderRadius, _borderRadius);

    QRect iconRect(thisRect.left() + _leftMargin,
                   thisRect.top() + (thisRect.height() - (_iconSize + 5)) / 2,
                   _iconSize + 5, _iconSize + 5);
    QPixmap iconPixmap = _icon.pixmap(_iconSize, _iconSize);
    QPainter pmPainter(&iconPixmap);
    pmPainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    pmPainter.fillRect(iconPixmap.rect(), _iconColor);
    pmPainter.end();

    painter.setPen(Qt::NoPen);
    if (themeMode == ElaThemeType::Light) {
        _iconBgColor = QColor(227, 239, 254);
    }
    else {
        _iconBgColor = QColor(30, 55, 90);
    }
    painter.setBrush(_iconBgColor);
    painter.drawRoundedRect(iconRect, _borderRadius - 3, _borderRadius - 3);
    painter.drawPixmap(iconRect.adjusted(5, 5, -5, -5), iconPixmap);

    if (_textColor.isValid()) {
        painter.setPen(_textColor);
    }
    else {
        painter.setPen(ElaThemeColor(themeMode, BasicText));
    }

    QFontMetrics fm(_textFont);
    QRect textRect(iconRect.right() + _spacing,
                   iconRect.top() + (iconRect.height() - fm.height()) / 2,
                   thisRect.right() - iconRect.right() - _leftMargin - _spacing,
                   fm.height());

    painter.setFont(_textFont);
    painter.drawText(textRect, _text);
}

QSize AboutInfoWidget::sizeHint() const
{
    QFontMetrics fm(_textFont);

    int w = _leftMargin + _iconSize + 5 + _spacing + fm.horizontalAdvance(_text);
    int h = _iconSize + 5 + 20;

    return QSize(w, h);
}
