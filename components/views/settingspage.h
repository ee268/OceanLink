#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "ElaDialog.h"

#include "ElaNavigationBar.h"

class SettingsPage : public ElaDialog
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget* parent = nullptr);

private:
    void initContent();

private:
    QWidget* _centralWid;
    ElaNavigationBar* _navigation;
};

#endif // SETTINGSPAGE_H
