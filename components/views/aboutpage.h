#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include "ElaDialog.h"

class AboutPage : public ElaDialog
{
    Q_OBJECT
public:
    explicit AboutPage(QWidget* parent = nullptr);
    ~AboutPage();

private:
    void initDialog();
};

#endif // ABOUTPAGE_H
