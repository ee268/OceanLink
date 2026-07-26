#ifndef SHORCUTKEYPAGE_H
#define SHORCUTKEYPAGE_H

#include "basecreateoption.h"

class ShorcutKeyPage : public BaseCreateOption
{
    Q_OBJECT
public:
    explicit ShorcutKeyPage(QWidget *parent = nullptr);

private:
    void initContent();
};

#endif // SHORCUTKEYPAGE_H
