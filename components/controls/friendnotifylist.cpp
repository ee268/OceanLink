#include "friendnotifylist.h"

#include <QVBoxLayout>

#include "ElaScrollBar.h"

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

    mainLayout->setContentsMargins(20, 0, 20, 20);
    mainLayout->setSpacing(20);
    mainLayout->setDirection(QVBoxLayout::BottomToTop);

    contentWidget->setLayout(mainLayout);
    this->setWidget(contentWidget);
    this->setWidgetResizable(true);

    contentWidget->setObjectName("FriendContentWidget");
    contentWidget->setStyleSheet("#FriendContentWidget { background-color: transparent; }");

    ElaScrollBar* scrollBar = new ElaScrollBar(this->verticalScrollBar(), this);
    scrollBar->setIsAnimation(true);
}
