#include "MusicViewModel.h"
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

MusicViewModel::MusicViewModel(QObject* parent)
    : QObject(parent), httpClient(new HttpClientModel(this)), musicPlayer(new MusicPlayerModel(this)) {
    connect(httpClient, &HttpClientModel::folderListFetched, this, &MusicViewModel::onFolderListFetched);
    connect(httpClient, &HttpClientModel::musicJsonFetched, this, &MusicViewModel::onMusicJsonFetched);
   // connect(lyricShowModel, &LyricShowModel::lyricsFetched, this, &MusicViewModel::onLyricsFetched);
}

void MusicViewModel::fetchFolderListFromUrl(const QString& url) {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "fetchFolderListFromUrl", Qt::QueuedConnection, Q_ARG(QString, url));
        return;
    }
    httpClient->fetchFolderList(url);
}

void MusicViewModel::fetchMusicJsonFromUrl(const QString& url) {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "fetchMusicJsonFromUrl", Qt::QueuedConnection, Q_ARG(QString, url));
        return;
    }
    httpClient->fetchMusicJson(url);
}

void MusicViewModel::playMusicFromUrl(const QString& url) {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "playMusicFromUrl", Qt::QueuedConnection, Q_ARG(QString, url));
        return;
    }
    musicPlayer->play(url);
    isPaused = false; // 播放新音乐时重置暂停状态
}
//void MusicViewModel::fetchLyricsFromUrl(const QString& url) {
//    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
//        QMetaObject::invokeMethod(this, "fetchLyricsFromUrl", Qt::QueuedConnection, Q_ARG(QString, url));
//        return;
//    }
//   lyricShowModel->showLyricsFromFile(url);
//}

void MusicViewModel::pauseOrResumeMusic() {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "pauseOrResumeMusic", Qt::QueuedConnection);
        return;
    }
    if (isPaused) {
        musicPlayer->play();
    }
    else {
        musicPlayer->pause();
    }
    isPaused = !isPaused;
}

void MusicViewModel::onFolderListFetched(const QStringList& folders) {
    emit folderListFetched(folders);
}

void MusicViewModel::onMusicJsonFetched(const QString& folder, const QString& jsonContent) {
    emit musicJsonFetched(folder, jsonContent);
}
//void MusicViewModel::onLyricsFetched(const QString& lyrics) {
//    emit lyricsFetched(lyrics);
//}