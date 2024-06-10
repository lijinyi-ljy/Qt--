#include "HttpClientModel.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QUrl>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

HttpClientModel::HttpClientModel(QObject* parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this)) {
    connect(networkManager, &QNetworkAccessManager::finished, this, &HttpClientModel::onReplyFinished);
}

void HttpClientModel::fetchFolderList(const QString& url) {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "fetchFolderListInternal", Qt::QueuedConnection, Q_ARG(QString, url));
        return;
    }
    fetchFolderListInternal(url);
}

void HttpClientModel::fetchFolderListInternal(const QString& url) {
    qDebug() << "Sending request to:" << url;
    QNetworkRequest request(url);
    networkManager->get(request);
}

void HttpClientModel::fetchMusicJson(const QString& url) {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "fetchMusicJsonInternal", Qt::QueuedConnection, Q_ARG(QString, url));
        return;
    }
    fetchMusicJsonInternal(url);
}

void HttpClientModel::fetchMusicJsonInternal(const QString& url) {
    qDebug() << "Sending request to:" << url;
    QNetworkRequest request(url);
    networkManager->get(request);
    qDebug() << "发送请求至" << url;
}

void HttpClientModel::onReplyFinished(QNetworkReply* reply) {
    qDebug() << "Reply received, URL:" << reply->url();
    QString url = reply->url().toString();
    if (reply->error() == QNetworkReply::NoError) {
        QString htmlContent = reply->readAll();
        if (url == "https://gitee.com/MarkYangUp/music/") {
            QStringList folders = parseHtmlForFolders(htmlContent);
            emit folderListFetched(folders);
        }
        else if (url.contains("music.json")) {
            QString folder = QUrl(url).path().split('/').at(5);
            emit musicJsonFetched(folder, htmlContent);
        }
    }
    else {
        qDebug() << "Network error:" << reply->errorString();
    }
    reply->deleteLater();
}

QStringList HttpClientModel::parseHtmlForFolders(const QString& htmlContent) {
    qDebug() << "开始解析HTML";
    QStringList folders;
    QRegularExpression re("<a[^>]* href=\"/MarkYangUp/music/tree/master/([^\"]*)\"[^>]*>");
    QRegularExpressionMatchIterator i = re.globalMatch(htmlContent);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString folderName = match.captured(1);
        if (!folderName.isEmpty()) {
            folderName = QUrl::fromPercentEncoding(folderName.toUtf8());
            folders << folderName;
        }
    }
    return folders;
}
