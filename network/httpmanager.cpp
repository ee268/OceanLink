#include "httpmanager.h"

#include <QNetworkReply>
#include <QJsonDocument>

void HttpManager::postHttpReq(QUrl url, QJsonObject json, RequestID req_id, Modules mod)
{
    QByteArray jsonData = QJsonDocument(json).toJson();
    qDebug() << "POST" << url << jsonData;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = _manager.post(request, jsonData);

    connect(reply, &QNetworkReply::finished, [this, reply, req_id, mod](){
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();

            emit sigHttpFinished(req_id, mod, "", ErrorCodes::NetworkError);
            reply->deleteLater();
            return;
        }

        QString res = reply->readAll();
        emit sigHttpFinished(req_id, mod, res, ErrorCodes::Success);
        reply->deleteLater();
        return;
    });
}

HttpManager::HttpManager() {
    connect(this, &HttpManager::sigHttpFinished, this, &HttpManager::slotHttpFinished);
}

void HttpManager::slotHttpFinished(RequestID id, Modules mod, QString res, ErrorCodes ec)
{
    switch(mod)
    {
    case Modules::REGISTER:
        emit sigRegModFinished(id, res, ec);
        break;
    }
}
