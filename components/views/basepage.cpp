#include "basepage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#include "ElaImageCard.h"
#include "ElaTheme.h"

BasePage::BasePage(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
    , _leftWid(nullptr)
    , _stackedWidget(nullptr)
{

}

BasePage::~BasePage() {}

QWidget *BasePage::getLeftWidget() const
{
    return _leftWid;
}

QStackedWidget *BasePage::getStackedWidget() const
{
    return _stackedWidget;
}

void BasePage::switchToPlaceHolderPage()
{
    _stackedWidget->setCurrentIndex(0);
}

void BasePage::initDefaultLayout()
{
    _leftWid = new QWidget(this);
    _stackedWidget = new QStackedWidget(this);

    _leftWid->setObjectName("LeftWidget");
    _leftWid->setStyleSheet("#LeftWidget{border-right: 2px solid #d1d1d1;}");
    connect(eTheme, &ElaTheme::themeModeChanged, [this](ElaThemeType::ThemeMode mode){
        if (mode == ElaThemeType::Dark) {
            _leftWid->setStyleSheet("#LeftWidget{border-right: 2px solid #454545;}");
            return;
        }
        _leftWid->setStyleSheet("#LeftWidget{border-right: 2px solid #d1d1d1;}");
    });

    //占位页面
    QWidget* placeholderPage = new QWidget(_stackedWidget);
    ElaImageCard* imgCard = new ElaImageCard(placeholderPage);
    imgCard->setCardImage(QImage(":/resource/image/logo/Ocean_gray.png"));
    imgCard->setFixedSize(90, 90);

    QVBoxLayout* placeholderLayout = new QVBoxLayout(placeholderPage);
    placeholderLayout->addWidget(imgCard, 0, Qt::AlignCenter);
    placeholderLayout->setContentsMargins(0, 0, 0, 0);
    placeholderPage->setLayout(placeholderLayout);

    _stackedWidget->addWidget(placeholderPage);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(_leftWid);
    mainLayout->addWidget(_stackedWidget);
    mainLayout->setSpacing(0);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 3);

    this->setLayout(mainLayout);
}
