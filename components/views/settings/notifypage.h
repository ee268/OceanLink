#ifndef NOTIFYPAGE_H
#define NOTIFYPAGE_H

#include "basecreateoption.h"

class NotifyPage : public BaseCreateOption
{
    Q_OBJECT
public:
    explicit NotifyPage(QWidget *parent = nullptr);

private:
    void initContent();
};

#endif // NOTIFYPAGE_H
