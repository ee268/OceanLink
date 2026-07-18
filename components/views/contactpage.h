#ifndef CONTACTPAGE_H
#define CONTACTPAGE_H

#include "basepage.h"

#include "ElaSuggestBox.h"
#include "ElaText.h"
#include "ElaComboBox.h"
#include "ElaLineEdit.h"

#include "../controls/displaycard.h"
#include "../controls/icontext.h"
#include "../controls/contactlist.h"
#include "../controls/friendnotifylist.h"
#include "../controls/confirmdialog.h"
#include "../controls/themecolorbutton.h"
#include "../controls/iconbutton.h"

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
    Q_OBJECT
public:
    explicit ContactDetailWid(ContactList* list, QWidget* parent = nullptr);

    void setIndex(const QModelIndex& index);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void initContent();

    void initCard();
    void initAccountInfo();
    void initPersonalInfo();
    void initFriendInfo();

    void updateBlurredBg();

    void updateInfo();

private:
    QWidget* _centralWid;
    QModelIndex _index;
    ContactList* _contactList;

    DisplayCard * _card;

    AvatarWid* _avatar;
    IconText* _account;
    IconText* _status;
    ElaText* _name;

    IconText* _sexText;
    ElaText* _ageText;
    ElaText* _birthText;

    QWidget* _nicknameWid;
    ElaText* _nickname;
    QWidget* _groupWid;
    ElaComboBox* _friendGroup;
    ElaText* _sign;
    ElaLineEdit* _signLineEdit;
    IconButton* _signEditButton;
    bool _signStatus;
    QWidget* _spaceWid;

    ThemeColorButton* _editOrSendButton;

    QPixmap _blurredBg;

private slots:
    void slotUpdateCentralWidStyle();

    void slotChangedGroup(const QString& text);

    void slotEditSignText();
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
