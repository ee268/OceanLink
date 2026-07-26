#include "shorcutkeypage.h"

#include <QVBoxLayout>

ShorcutKeyPage::ShorcutKeyPage(QWidget *parent)
    : BaseCreateOption{parent}
{
    initContent();
}

void ShorcutKeyPage::initContent()
{
    layout()->addWidget(createButtonOption("发送消息", "更改"));
    layout()->addStretch();
}
