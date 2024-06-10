#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QMediaPlayer>

class MusicPlayerModel : public QObject {
    Q_OBJECT
public:
    explicit MusicPlayerModel(QObject* parent = nullptr);
    void play(const QString& url);
    void play();
    void pause();

private slots:
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void handleError(QMediaPlayer::Error error);

private:
    void playInternal(const QString& url);

    QMediaPlayer* mediaPlayer;
};

#endif // MUSICPLAYER_H
