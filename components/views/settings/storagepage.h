#ifndef STORAGEPAGE_H
#define STORAGEPAGE_H

#include "basecreateoption.h"

class StoragePage : public BaseCreateOption
{
    Q_OBJECT
public:
    explicit StoragePage(QWidget *parent = nullptr);

private:
    void initContent();
};

#endif // STORAGEPAGE_H
