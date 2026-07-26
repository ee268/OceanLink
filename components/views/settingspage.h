#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "ElaDialog.h"
#include "ElaNavigationBar.h"

class SettingsPage : public ElaDialog
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget* parent = nullptr);

    enum {
        Notify_Page = 1,
        Storage_Page,
        ShortcutKey_Page,
        Permission_Page
    };

private:
    void initContent();

private:
    QWidget* _centralWid;
    ElaNavigationBar* _navigation;

    QWidget* _notifyPage;
    QWidget* _storagePage;
    QWidget* _shortcutKeyPage;
    QWidget* _permissionPage;
};

#endif // SETTINGSPAGE_H
