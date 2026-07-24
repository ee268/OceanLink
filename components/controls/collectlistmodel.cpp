#include "collectlistmodel.h"

CollectListModel::CollectListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

CollectListModel::~CollectListModel()
{
}

int CollectListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _dataList.count();
}

QVariant CollectListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _dataList.count()) {
        return QVariant();
    }

    const CollectData& item = _dataList.at(index.row());

    switch (role) {
    case Date:
        return item.date;
    case Source:
        return item.source;
    case Cover:
        return item.cover;
    case Content:
        return item.content;
    case SourceName:
        return item.sourceName;
    case Type:
        return item.type;
    default:
        return QVariant();
    }
}

void CollectListModel::addCollect(const CollectData &data)
{
    beginInsertRows(QModelIndex(), _dataList.count(), _dataList.count());
    _dataList.append(data);
    endInsertRows();
}

void CollectListModel::removeCollect(int index)
{
    if (index < 0 || index >= _dataList.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    _dataList.removeAt(index);
    endRemoveRows();
}

void CollectListModel::resetCollects(const QList<CollectData> &dataList)
{
    beginResetModel();
    _dataList = dataList;
    endResetModel();
}
