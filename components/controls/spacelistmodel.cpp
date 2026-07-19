#include "spacelistmodel.h"

#include "ElaIcon.h"

SpaceListModel::SpaceListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int SpaceListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _dataList.size();
}

QVariant SpaceListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _dataList.size())
        return QVariant();

    const SpaceListData& item = _dataList.at(index.row());

    switch (role) {
    case Name:
        return item.name;
    case Avatar:
        return item.avatar;
    case LatestTitle:
        return item.latest_title;
    case Account:
        return item.account;
    default:
        return QVariant();
    }
}

void SpaceListModel::addSpace(const SpaceListData& data)
{
    int row = _dataList.size();
    beginInsertRows(QModelIndex(), row, row);
    _dataList.append(data);
    endInsertRows();
}

void SpaceListModel::removeSpace(int row)
{
    if (row < 0 || row >= _dataList.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    _dataList.removeAt(row);
    endRemoveRows();
}
