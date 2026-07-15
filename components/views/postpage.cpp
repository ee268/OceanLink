#include "postpage.h"

#include <QVBoxLayout>

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

    QWidget* wid = new QWidget(leftWid);
    QVBoxLayout* widLayout = new QVBoxLayout(wid);

    ElaText* title = new ElaText("好友空间", wid);
    title->setTextStyle(ElaTextType::Subtitle);
    title->setIsWrapAnywhere(false);

    widLayout->setContentsMargins(15, 15, 15, 15);
    widLayout->setSpacing(0);
    widLayout->addWidget(title, 0, Qt::AlignLeft);
    wid->setLayout(widLayout);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(wid);
    mainLayout->addStretch();
    leftWid->setLayout(mainLayout);
}
