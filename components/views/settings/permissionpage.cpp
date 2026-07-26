#include "permissionpage.h"

#include <QVBoxLayout>

PermissionPage::PermissionPage(QWidget *parent)
    : BaseCreateOption(parent)
{
    initContent();
}

void PermissionPage::initContent()
{
    layout()->addWidget(createComboBoxOption("加我为好友时的验证方式",
                                             {"允许任何人", "需要验证信息", "需要回答问题"}));

    layout()->addStretch();
}
