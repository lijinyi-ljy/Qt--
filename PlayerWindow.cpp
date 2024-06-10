#include "PlayerWindow.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

PlayerWindow::PlayerWindow(QWidget* parent)
    : QMainWindow(parent), viewModel(new MusicViewModel(this)) {
    ui.setupUi(this);

    connect(ui.onlineMusicButton, &QPushButton::clicked, this, &PlayerWindow::onOnlineMusicClicked);
    connect(ui.playPauseButton, &QPushButton::clicked, this, &PlayerWindow::onPlayPauseButtonClicked);
    connect(ui.previousButton, &QPushButton::clicked, this, &PlayerWindow::onPreviousButtonClicked);
    connect(ui.nextButton, &QPushButton::clicked, this, &PlayerWindow::onNextButtonClicked);
    connect(viewModel, &MusicViewModel::folderListFetched, this, &PlayerWindow::onFolderListFetched);
    connect(viewModel, &MusicViewModel::musicJsonFetched, this, &PlayerWindow::onMusicJsonFetched);
   // connect(viewModel, &MusicViewModel::lyricsFetched, this, &PlayerWindow::onLyricsFetched);
    connect(ui.musicListWidget, &QListWidget::itemClicked, this, &PlayerWindow::onMusicItemClicked);

    loadLocalMusic();
    loadOnlineMusic();
}

void PlayerWindow::loadLocalMusic() {
    // 实现加载本地音乐的逻辑
}

void PlayerWindow::loadOnlineMusic() {
    // 实现加载在线音乐的逻辑
}

void PlayerWindow::updateUI() {
    // 更新UI
}

void PlayerWindow::setAlbumArt(const QPixmap& albumArt) {
    // 设置专辑图片
}

void PlayerWindow::onOnlineMusicClicked() {
    qDebug() << "Online Music button clicked";
    QString url = "https://gitee.com/MarkYangUp/music/";
    viewModel->fetchFolderListFromUrl(url); // 请求文件夹列表
}

void PlayerWindow::onFolderListFetched(const QStringList& folders) {
    qDebug() << "Fetched folders:" << folders;
    ui.musicListWidget->clear(); // 清空当前的音乐列表
    for (const QString& folder : folders) {
        QString jsonUrl = QString("https://gitee.com/MarkYangUp/music/raw/master/%1/music.json").arg(folder);
        viewModel->fetchMusicJsonFromUrl(jsonUrl);
    }
}

void PlayerWindow::onMusicJsonFetched(const QString& folder, const QString& jsonContent) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonContent.toUtf8());
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray listArray = jsonObj["list"].toArray();

        for (const QJsonValue& value : listArray) {
            QJsonObject trackObj = value.toObject();
            QString musicName = trackObj["musicName"].toString();
            QString albumName = trackObj["albumName"].toString();

            QListWidgetItem* listItem = new QListWidgetItem(musicName + " - " + albumName + " (" + folder + ")");
            ui.musicListWidget->addItem(listItem);
        }
    }
    else {
        qDebug() << "Invalid JSON";
    }
}

void PlayerWindow::onMusicItemClicked(QListWidgetItem* item) {
    QString musicInfo = item->text();
    QStringList parts = musicInfo.split(" - ");
    if (parts.size() >= 3) {
        QString folder = parts[1];
        QString encodedFolder = QUrl::toPercentEncoding(folder);
        QString encodedMusicName = QUrl::toPercentEncoding(parts[0]);
        QString mp3Url = QString("https://gitee.com/MarkYangUp/music/raw/master/%1/%2/music.mp3")
            .arg(encodedFolder)
            .arg(encodedMusicName);
        QString lyricsUrl = QString("https://gitee.com/MarkYangUp/music/raw/master/%1/%2/lyric.txt")
            .arg(encodedFolder)
            .arg(encodedMusicName);
        qDebug() << "播放 MP3 文件，URL：" << mp3Url;
        qDebug() << "获取歌词文件，URL：" << lyricsUrl;
        viewModel->playMusicFromUrl(mp3Url);
       // viewModel->fetchLyricsFromUrl(lyricsUrl);
        currentPlayingIndex = ui.musicListWidget->row(item);
    }
    else {
        qDebug() << "无法解析音乐项文本：" << musicInfo;
    }
}

void PlayerWindow::onPlayPauseButtonClicked() {
    viewModel->pauseOrResumeMusic();
}

void PlayerWindow::onPreviousButtonClicked() {
    int itemCount = ui.musicListWidget->count();
    if (itemCount > 0) {
        currentPlayingIndex = (currentPlayingIndex - 1 + itemCount) % itemCount;
        playMusicAtIndex(currentPlayingIndex);
    }
}

void PlayerWindow::onNextButtonClicked() {
    int itemCount = ui.musicListWidget->count();
    if (itemCount > 0) {
        currentPlayingIndex = (currentPlayingIndex + 1) % itemCount;
        playMusicAtIndex(currentPlayingIndex);
    }
}

void PlayerWindow::playMusicAtIndex(int index) {
    QListWidgetItem* item = ui.musicListWidget->item(index);
    if (item) {
        onMusicItemClicked(item);
    }
}
void PlayerWindow::onLyricsFetched(const QString& lyrics) {
    ui.lyricsLabel->setText(lyrics);
}


