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
    float ratio = 0.45;
    int w = logoImg.width() * ratio;
    int h = logoImg.height() * ratio;
    ElaImageCard* logoCard = new ElaImageCard(this);
    logoCard->setCardImage(logoImg);
    logoCard->setFixedSize(QSize(w, h));

    IconText* versionInfo = new IconText(this);
    versionInfo->setText(QString("版本: %1 (64位)").arg(PROJECT_VERSION));
    versionInfo->setPixelSize(16);
    versionInfo->setTextColor(QColor(17, 141, 240));
    versionInfo->setTextBold(true);

    IconText* copyrightInfo = new IconText(this);

    QString beginYear = "2026";
    QString curYear = QString::number(QDateTime::currentDateTime().date().year());
    if (curYear == beginYear) {
        curYear = "now";
    }
    copyrightInfo->setText(QString("Copyright © %1-%2 ee268.")
                               .arg(beginYear)
                               .arg(curYear));

    IconText* licenseInfo = new IconText("MIT LICENSE", this);
    licenseInfo->setIcon(QIcon(":/resource/image/about/document-text-outline.svg"));

    IconText* techInfo = new IconText("基于Qt 5.15 Widgets + ElaWidgetTools搭建", this);
    IconText* githubInfo = new IconText("https://github.com/ee268/XunLink", this);
    githubInfo->setIcon(QIcon(":/resource/image/about/logo-github.svg"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 15);
    mainLayout->addWidget(logoCard, 0, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addWidget(versionInfo, 1, Qt::AlignHCenter);
    mainLayout->addWidget(copyrightInfo, 2, Qt::AlignHCenter);
    mainLayout->addWidget(licenseInfo, 3, Qt::AlignHCenter);
    mainLayout->addSpacerItem(new QSpacerItem(0, 150, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addWidget(techInfo, 4, Qt::AlignHCenter);
    mainLayout->addWidget(githubInfo, 5, Qt::AlignHCenter);
    mainLayout->setSpacing(5);

    this->setLayout(mainLayout);
}


