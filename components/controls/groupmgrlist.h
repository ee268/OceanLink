#ifndef GROUPMGRLIST_H
#define GROUPMGRLIST_H

#include <QWidget>
#include <QHBoxLayout>

#include "avatarwidget.h"

#include "ElaComboBox.h"
#include "ElaRadioButton.h"
#include "ElaText.h"
#include "ElaScrollArea.h"

struct FriendGroupData
{
    QString name;
    QString nickName;
    QPixmap avatar;
    QString groupName;

    bool operator==(const FriendGroupData &other) const {
        return name == other.name
            && nickName == other.nickName
               && groupName == other.groupName;
    }
};

class GroupMgrItem : public QWidget
{
    Q_OBJECT
public:
    explicit GroupMgrItem(FriendGroupData data, QWidget *parent = nullptr);

    void updateData(FriendGroupData data);

    void setBackGroundColor(QColor color);

    enum ItemState {
        Selected,
        Hover,
        Normal
    };

    void setChecked(bool isChecked);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void initContent();

private:
    FriendGroupData _data;

    ElaRadioButton* _radioBtn;
    AvatarWidget* _avatarWid;
    ElaText* _name;
    ElaText* _nickName;
    ElaComboBox* _comboBox;

    QColor _bgColor;
    ItemState _state;

private slots:
    void slotClickedRadioBtn();
};

class GroupMgrList : public ElaScrollArea
{
    Q_OBJECT
public:
    explicit GroupMgrList(QWidget *parent = nullptr);

    void addData(const FriendGroupData& data);
    void removeData(const FriendGroupData& data);
    void updateData(const FriendGroupData& data);

    void setAllChecked(bool isAllChecked);

private:
    void initContent();

    int findData(const FriendGroupData& data);

private:
    QVBoxLayout* _mainLayout;

    QStringList _header;
    QList<FriendGroupData> _dataList;
    QList<GroupMgrItem*> _itemList;
};

#endif // GROUPMGRLIST_H
