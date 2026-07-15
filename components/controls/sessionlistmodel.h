#ifndef SESSIONLISTMODEL_H
#define SESSIONLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QPixmap>

struct UserItem {
    QString name;
    QString latest_msg;
    QString latest_date;
    qint8 unread_cnt;
    QPixmap avatar;
};

class SessionListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SessionListModel(QObject *parent = nullptr);
    ~SessionListModel();

    enum {
        Name = Qt::ToolTipRole + 1,
        LastestMsg,
        LastestDate,
        UnreadCnt,
        Avatar
    };

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addItem(UserItem item);

private:
    QList<UserItem> _itemList;
};

#endif // SESSIONLISTMODEL_H
