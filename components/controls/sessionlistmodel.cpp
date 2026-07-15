#include "sessionlistmodel.h"

SessionListModel::SessionListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

SessionListModel::~SessionListModel()
{

}

int SessionListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _itemList.size();
}

QVariant SessionListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    UserItem item = _itemList.at(row);

    switch(role) {
    case Name:
        return item.name;
    case LastestDate:
        return item.latest_date;
    case LastestMsg:
        return item.latest_msg;
    case UnreadCnt:
        return item.unread_cnt;
    case Avatar:
        return item.avatar;
    case Qt::ToolTipRole:
        return item.name;
    default:
        return QVariant();
    }

    return QVariant();
}

void SessionListModel::addItem(UserItem item)
{
    beginInsertRows(QModelIndex(), _itemList.size(), _itemList.size());
    _itemList.append(item);
    endInsertRows();
}
