#ifndef GROUPMGRDIALOG_H
#define GROUPMGRDIALOG_H

#include "ElaDialog.h"
#include "ElaNavigationBar.h"
#include "ElaLineEdit.h"
#include "ElaRadioButton.h"

#include "../../controls/contactlist.h"
#include "../../controls/groupmgrlist.h"

class GroupMgrDialog : public ElaDialog
{
    Q_OBJECT
public:
    explicit GroupMgrDialog(ContactList* list, QWidget* parent = nullptr);

private:
    void initContent();
    void initGroupList();

private:
    ContactList* _contactList;

    QWidget* _centralWid;
    ElaNavigationBar* _navigation;
    QString _groupKey;
    QString _addGroupKey;

    ElaDialog* _addGroupDialog;
    ElaLineEdit* _addGroupEdit;

    QWidget* _allFriendGroup;
    QList<QWidget*> _otherGroups;

    GroupMgrList* _groupList;
    ElaRadioButton* _allRadioBtn;

private:
    void slotAddGroupClicked();

    void slotNavigationClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack);

    void slotClickedRadioBtn();
};

#endif // GROUPMGRDIALOG_H
