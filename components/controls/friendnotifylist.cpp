#include "friendnotifylist.h"

#include <QVBoxLayout>

FriendNotifyList::FriendNotifyList(QWidget *parent)
    : ElaScrollArea(parent)
{
    initContent();
}

void FriendNotifyList::addNotifyItem(const FriendNotifyData &data)
{
    FriendNotifyItem* item = new FriendNotifyItem(data, this->widget());
    item->setFixedHeight(100);
    _notifyList.append(item);

    this->widget()->layout()->addWidget(item);
}

void FriendNotifyList::clear()
{
    for (auto item : _notifyList) {
        this->widget()->layout()->removeWidget(item);
        item->deleteLater();
    }
    _notifyList.clear();
}

void FriendNotifyList::initContent()
{
    QWidget* contentWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(contentWidget);

    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(15);
    mainLayout->setDirection(QVBoxLayout::BottomToTop);

    contentWidget->setLayout(mainLayout);
    this->setWidget(contentWidget);
    this->setWidgetResizable(true);

    contentWidget->setObjectName("FriendContentWidget");
    contentWidget->setStyleSheet("#FriendContentWidget { background-color: transparent; }");
}
