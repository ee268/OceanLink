#ifndef SPACELISTMODEL_H
#define SPACELISTMODEL_H

#include <QAbstractListModel>
#include <QPixmap>
#include <QVector>

struct SpaceListData {
    QString name;
    QPixmap avatar;
    QString latest_title;
    QString account;
};

class SpaceListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SpaceRole {
        Name = Qt::ToolTipRole,
        Avatar,
        LatestTitle,
        Account,
    };

    explicit SpaceListModel(QObject *parent = nullptr);
    ~SpaceListModel() = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addSpace(const SpaceListData& data);
    void removeSpace(int row);

private:
    QVector<SpaceListData> _dataList;
};

#endif // SPACELISTMODEL_H
