#include "notifypage.h"

#include <QVBoxLayout>

NotifyPage::NotifyPage(QWidget *parent)
    : BaseCreateOption{parent}
{
    initContent();
}

void NotifyPage::initContent()
{
    layout()->addWidget(createOption("好友消息提醒弹窗"));
    layout()->addWidget(createOption("消息提示音"));
    layout()->addStretch();
}
