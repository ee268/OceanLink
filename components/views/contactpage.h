#ifndef CONTACTPAGE_H
#define CONTACTPAGE_H

#include "basepage.h"

#include "ElaSuggestBox.h"

#include "../controls/contactlist.h"
#include "../controls/friendnotifylist.h"
#include "../controls/confirmdialog.h"
#include "contact/contactdetailwid.h"
#include "contact/groupmgrdialog.h"

class ContactPage : public BasePage
{
    Q_OBJECT
public:
    explicit ContactPage(QWidget* parent = nullptr);
    ~ContactPage();

private:
    void initLeftWidget();
    void initRightWidget();
    void initContactDetailWid();

private:
    ElaSuggestBox* _suggestBox;

    ContactList* _contactList;

    FriendNotifyList* _notifyList;

    ConfirmDialog* _clearConfirmDialog;

    ContactDetailWid* _detailWid;

    GroupMgrDialog* _groupDialog;

private slots:
    void slotClearButtonClicked();

    void slotClearAllNotify();

    void slotNotifyButtonClicked();

    void slotToContactDetail(const QModelIndex& index);

    void slotOpenGroupClicked();
};

#endif // CONTACTPAGE_H
