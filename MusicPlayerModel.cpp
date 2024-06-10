#include "MusicPlayerModel.h"
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

MusicPlayerModel::MusicPlayerModel(QObject* parent)
    : QObject(parent), mediaPlayer(new QMediaPlayer(this)) {
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MusicPlayerModel::handleMediaStatusChanged);
    connect(mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &MusicPlayerModel::handleError);
}

void MusicPlayerModel::play(const QString& url) {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "playInternal", Qt::QueuedConnection, Q_ARG(QString, url));
        return;
    }
    playInternal(url);
}

void MusicPlayerModel::playInternal(const QString& url) {
    qDebug() << "设置媒体" << url;
    mediaPlayer->setMedia(QUrl(url));
    qDebug() << "Playing media...";
    mediaPlayer->play();
}

void MusicPlayerModel::play() {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "play", Qt::QueuedConnection);
        return;
    }
    mediaPlayer->play();
}

void MusicPlayerModel::pause() {
    if (QCoreApplication::instance()->thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "pause", Qt::QueuedConnection);
        return;
    }
    mediaPlayer->pause();
}

void MusicPlayerModel::handleMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    qDebug() << "Media status:" << status;
}

void MusicPlayerModel::handleError(QMediaPlayer::Error error) {
    qDebug() << "Error:" << error;
}
