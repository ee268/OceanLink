#ifndef FRIENDNOTIFYLIST_H
#define FRIENDNOTIFYLIST_H

#include "ElaScrollArea.h"

#include "friendnotifyitem.h"

class FriendNotifyList : public ElaScrollArea
{
    Q_OBJECT
public:
    explicit FriendNotifyList(QWidget* parent = nullptr);
    ~FriendNotifyList() = default;

    void addNotifyItem(const FriendNotifyData& data);

    void clear();

private:
    void initContent();

private:
    QList<FriendNotifyItem*> _notifyList;
};

#endif // FRIENDNOTIFYLIST_H
