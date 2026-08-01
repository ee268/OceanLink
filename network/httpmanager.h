#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>

#include "ElaSingleton.h"

#include "../global/global.h"

class HttpManager : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE(HttpManager)
public:
    ~HttpManager() = default;

    void postHttpReq(QUrl url, QJsonObject json, RequestID req_id, Modules mod);

private:
    HttpManager();

    QNetworkAccessManager _manager;

signals:
    void sigHttpFinished(RequestID id, Modules mod, QString res, ErrorCodes ec);

    void sigRegModFinished(RequestID id, QString res, ErrorCodes ec);

private slots:
    void slotHttpFinished(RequestID id, Modules mod, QString res, ErrorCodes ec);
};

#endif // HTTPMANAGER_H
