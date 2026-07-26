#ifndef PERMISSIONPAGE_H
#define PERMISSIONPAGE_H

#include "basecreateoption.h"

class PermissionPage : public BaseCreateOption
{
public:
    explicit PermissionPage(QWidget* parent = nullptr);

private:
    void initContent();
};

#endif // PERMISSIONPAGE_H
