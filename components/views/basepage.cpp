#include "basepage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "ElaImageCard.h"
#include "ElaTheme.h"

BasePage::BasePage(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
    , _leftWid(nullptr)
    , _rightWid(nullptr)
    , _stackedWidget(nullptr)
{

}

BasePage::~BasePage() {}

QWidget *BasePage::getLeftWidget() const
{
    return _leftWid;
}

QWidget *BasePage::getRightWidget() const
{
    return _rightWid;
}

QStackedWidget *BasePage::getStackedWidget() const
{
    return _stackedWidget;
}

void BasePage::switchToPlaceHolderPage()
{
    _stackedWidget->setCurrentIndex(0);
}

void BasePage::setCurrentIndex(int index)
{
    if (index == _stackedWidget->currentIndex() || index < 0 || index >= _stackedWidget->count()) {
        return;
    }

    QWidget* currentPage = _stackedWidget->currentWidget();
    QWidget* nextPage = _stackedWidget->widget(index);

    int offset = _stackedWidget->width();
    int duration = 200;

    if (index > _stackedWidget->currentIndex()) {
        offset = offset;
    } else {
        offset = -offset;
    }

    nextPage->setGeometry(offset, 0, nextPage->width(), nextPage->height());
    nextPage->show();

    QPropertyAnimation* anim1 = new QPropertyAnimation(currentPage, "pos");
    anim1->setDuration(duration);
    anim1->setEndValue(QPoint(-offset, 0));

    QPropertyAnimation* anim2 = new QPropertyAnimation(nextPage, "pos");
    anim2->setDuration(duration);
    anim2->setEndValue(QPoint(0, 0));

    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(anim1);
    group->addAnimation(anim2);

    connect(group, &QParallelAnimationGroup::finished, [=]() {
        _stackedWidget->setCurrentIndex(index);
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void BasePage::initDefaultLayout()
{
    _leftWid = new QWidget(this);
    _rightWid = new QWidget(this);
    _stackedWidget = new QStackedWidget(_rightWid);
    _stackedWidget->resize(_rightWid->size());

    QVBoxLayout* rightLayout = new QVBoxLayout(_rightWid);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    rightLayout->addWidget(_stackedWidget);
    _rightWid->setLayout(rightLayout);

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
    mainLayout->addWidget(_rightWid);
    mainLayout->setSpacing(0);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 3);

    this->setLayout(mainLayout);
}

void BasePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // if (_stackedWidget && _rightWid) {
    //     _stackedWidget->resize(_rightWid->size());
    // }
}
