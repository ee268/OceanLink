#ifndef CONTACTDETAILWID_H
#define CONTACTDETAILWID_H

#include <QWidget>

#include "ElaText.h"
#include "ElaComboBox.h"
#include "ElaLineEdit.h"

#include "../../controls/displaycard.h"
#include "../../controls/icontext.h"
#include "../../controls/themecolorbutton.h"
#include "../../controls/iconbutton.h"
#include "../../controls/avatarwidget.h"
#include "../../controls/contactlist.h"

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

    AvatarWidget* _avatar;
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

#endif // CONTACTDETAILWID_H
