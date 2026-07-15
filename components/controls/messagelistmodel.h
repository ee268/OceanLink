#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QPixmap>

struct MessageItem {
    bool isSelf;
    QString msg;
    QString date;
    QPixmap avatar;
    QString name;
};

class MessageListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MessageListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addItem(MessageItem item);

public:
    enum {
        Msg,
        Date,
        IsSelf,
        Avatar,
        Name
    };

private:
    QList<MessageItem> _itemList;
};

#endif // MESSAGELISTMODEL_H
