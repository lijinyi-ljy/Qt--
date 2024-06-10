#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_PlayerWindow.h"
#include "MusicViewModel.h"
#include <QTimer>
#include <QTime>
class PlayerWindow : public QMainWindow {
    Q_OBJECT

public:
    PlayerWindow(QWidget* parent = Q_NULLPTR);

private slots:
    void onOnlineMusicClicked();
    void onFolderListFetched(const QStringList& folders);
    void onMusicJsonFetched(const QString& folder, const QString& jsonContent);
    void onMusicItemClicked(QListWidgetItem* item);
    void onPlayPauseButtonClicked();
    void onPreviousButtonClicked();
    void onNextButtonClicked();
    void onLyricsFetched(const QString& lyrics);
private:
    Ui::PlayerWindowClass ui;
    MusicViewModel* viewModel;
    QTime startTime;
    QTimer* stopwatchTimer;
    int currentPlayingIndex = -1;

    void loadLocalMusic();
    void loadOnlineMusic();
    void updateUI();
    void setAlbumArt(const QPixmap& albumArt);
    void playMusicAtIndex(int index);
};

#endif // PLAYERWINDOW_H
