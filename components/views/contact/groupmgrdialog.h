#ifndef GROUPMGRDIALOG_H
#define GROUPMGRDIALOG_H

#include "ElaDialog.h"
#include "ElaNavigationBar.h"
#include "ElaLineEdit.h"

#include "../../controls/contactlist.h"

class GroupMgrDialog : public ElaDialog
{
    Q_OBJECT
public:
    explicit GroupMgrDialog(ContactList* list, QWidget* parent = nullptr);

private:
    void initContent();

private:
    ContactList* _contactList;

    QWidget* _centralWid;
    ElaNavigationBar* _navigation;
    QString _groupKey;
    QString _addGroupKey;

    ElaDialog* _addGroupDialog;
    ElaLineEdit* _addGroupEdit;

private:
    void slotAddGroupClicked();

    void slotNavigationClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack);
};

#endif // GROUPMGRDIALOG_H
