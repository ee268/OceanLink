#ifndef POSTPAGE_H
#define POSTPAGE_H

#include "basepage.h"

class PostPage : public BasePage
{
    Q_OBJECT
public:
    explicit PostPage(QWidget* parent = nullptr);
    ~PostPage() = default;

private:
    void initLeftWidget();
};

#endif // POSTPAGE_H
