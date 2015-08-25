#ifndef CSRPLAYER_H
#define CSRPLAYER_H

#include "videowidget.h"
#include "ui_csrplayer.h"
#include <QListView>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QSlider;
class QVideoProbe;
class QVideoWidget;
class PlaylistModel;
namespace Ui {
class csrplayer;
}
QT_END_NAMESPACE

//#define ENABLE_PLAYLISTVIEW
#define USE_V4L2sink
#define DEFAULE_W 1920
#define DEFAULT_H 1080

class csrplayer : public QMainWindow
{
    Q_OBJECT

public:
    csrplayer(QWidget *parent = 0);
    ~csrplayer();

signals:

private slots:
    void open();
    void on_closeButton_clicked();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();

    void previousClicked();

    void setOpenEnabled(QMediaPlayer::State state);
    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int progress);
    void videoAvailableChanged(bool available);

    void displayErrorMessage();
    void addToPlaylist(const QStringList &fileNames);

private:
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);

    Ui::csrplayer *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QLabel *coverLabel;

    PlaylistModel *playlistModel;
#ifdef ENABLE_PLAYLISTVIEW
    QListView *playlistView;
#endif
    QString trackInfo;
    QString statusInfo;
    qint64 duration;
};

#endif // CSRPLAYER_H
