#ifndef MUSICVIEWMODEL_H
#define MUSICVIEWMODEL_H

#include <QObject>
#include "HttpClientModel.h"
#include "MusicPlayerModel.h"
#include "LyricShowModel.h"
class MusicViewModel : public QObject {
    Q_OBJECT

public:
    explicit MusicViewModel(QObject* parent = nullptr);
    void fetchFolderListFromUrl(const QString& url);
    void fetchMusicJsonFromUrl(const QString& url);
    void playMusicFromUrl(const QString& url);
  //  void fetchLyricsFromUrl(const QString& url);
    void pauseOrResumeMusic();

signals:
    void folderListFetched(const QStringList& folders);
    void musicJsonFetched(const QString& folder, const QString& jsonContent);
   // void lyricsFetched(const QString& lyrics);
private slots:
    void onFolderListFetched(const QStringList& folders);
    void onMusicJsonFetched(const QString& folder, const QString& jsonContent);
  //  void onLyricsFetched(const QString& lyrics);
private:
    HttpClientModel* httpClient;
    MusicPlayerModel* musicPlayer;
   // LyricShowModel* lyricShowModel;
    bool isPaused = false; // 跟踪音乐是否被暂停
};

#endif // MUSICVIEWMODEL_H
