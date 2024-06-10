#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>

class HttpClientModel : public QObject {
    Q_OBJECT

public:
    explicit HttpClientModel(QObject* parent = nullptr);
    void fetchFolderList(const QString& url);
    void fetchMusicJson(const QString& url);

signals:
    void folderListFetched(const QStringList& folders);
    void musicJsonFetched(const QString& folder, const QString& jsonContent);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    void fetchFolderListInternal(const QString& url);
    void fetchMusicJsonInternal(const QString& url);

    QNetworkAccessManager* networkManager;
    QStringList parseHtmlForFolders(const QString& htmlContent);
};

#endif // HTTPCLIENT_H
