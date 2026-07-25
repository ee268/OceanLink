#include "messagelistmodel.h"

MessageListModel::MessageListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int MessageListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _itemList.size();
}

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    MessageItemData item = _itemList.at(row);

    switch(role) {
    case Msg:
        return item.msg;
    case Date:
        return item.date;
    case IsSelf:
        return item.isSelf;
    case Avatar:
        return item.avatar;
    case Name:
        return item.name;
    default:
        return QVariant();
    }

    return QVariant();
}

void MessageListModel::addItem(MessageItemData item)
{
    beginInsertRows(QModelIndex(), _itemList.size(), _itemList.size());
    _itemList.append(item);
    endInsertRows();
}
