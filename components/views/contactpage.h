#ifndef CONTACTPAGE_H
#define CONTACTPAGE_H

#include "basepage.h"

#include "ElaSuggestBox.h"
#include "ElaText.h"

#include "../controls/displaycard.h"
#include "../controls/icontext.h"
#include "../controls/contactlist.h"
#include "../controls/friendnotifylist.h"
#include "../controls/confirmdialog.h"

class AvatarWid : public QWidget
{
public:
    explicit AvatarWid(QWidget* parent = nullptr);

    void setAvatar(const QPixmap& pixmap);
    void setName(const QString& name);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap _avatar;
    QString _name;
};

class ContactDetailWid : public QWidget
{
public:
    explicit ContactDetailWid(QWidget* parent = nullptr);

    void setIndex(const QModelIndex& index);

private:
    void initContent();

    void initCard();
    void initAccountInfo();
    void initPersonalInfo();

    void updateInfo();

private:
    QWidget* _centralWid;
    QModelIndex _index;

    DisplayCard * _card;

    AvatarWid* _avatar;
    IconText* _account;
    IconText* _status;
    ElaText* _name;

    IconText* _sexText;
    ElaText* _ageText;
    ElaText* _birthText;
};

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

private slots:
    void slotClearButtonClicked();

    void slotClearAllNotify();

    void slotNotifyButtonClicked();

    void slotToContactDetail(const QModelIndex& index);
};

#endif // CONTACTPAGE_H
