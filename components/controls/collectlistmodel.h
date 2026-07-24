#ifndef COLLECTLISTMODEL_H
#define COLLECTLISTMODEL_H

#include <QAbstractListModel>
#include <QPixmap>

#include "../../global/global.h"

struct CollectData {
    QString date;
    QString source;
    QPixmap cover;
    QString content;
    QString sourceName;
    CollectType type;

    CollectData() : type(None) {}

    CollectData(const QString& date, const QString& source, const QPixmap& cover,
                const QString& content, const QString& sourceName, CollectType type)
        : date(date), source(source), cover(cover)
        , content(content), sourceName(sourceName), type(type) {}
};

class CollectListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CollectListModel(QObject* parent = nullptr);
    ~CollectListModel();

    enum {
        Date = Qt::ToolTipRole + 1,
        Source,
        Cover,
        Content,
        SourceName,
        Type
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void addCollect(const CollectData& data);
    void removeCollect(int index);
    void resetCollects(const QList<CollectData>& dataList);

private:
    QList<CollectData> _dataList;
};

#endif // COLLECTLISTMODEL_H
