#include "storagepage.h"

#include <QVBoxLayout>

StoragePage::StoragePage(QWidget *parent)
    : BaseCreateOption{parent}
{
    initContent();
}

void StoragePage::initContent()
{
    layout()->addWidget(createButtonOption("清理所有数据", "清理", true));

    layout()->addWidget(createButtonOption("聊天数据保存到", "更改"));
    layout()->addWidget(createButtonOption("接收的文件保存到", "更改"));
    layout()->addStretch();
}
