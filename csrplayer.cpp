#include "csrplayer.h"
#include "ui_csrplayer.h"
#include "playercontrols.h"
#include "playlistmodel.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QMediaMetaData>
#include <QtWidgets>

csrplayer::csrplayer(QWidget *parent)
    : QMainWindow(parent)
    , videoWidget(0)
    , coverLabel(0)
    //, slider(0)
	, ui(new Ui::csrplayer)
{
	ui->setupUi(this);
//! [create-objs]
    player = new QMediaPlayer(this);
    // owned by PlaylistModel
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
//! [create-objs]

    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
    connect(playlist, SIGNAL(currentIndexChanged(int)), SLOT(playlistPositionChanged(int)));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
    connect(player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(videoAvailableChanged(bool)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));

//! [2]
    videoWidget = new VideoWidget(this);
    player->setVideoOutput(videoWidget);

    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
//! [2]

    //playlistView = new QListView(this);
    ui->playlistView->setModel(playlistModel);
    ui->playlistView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));

    connect(ui->playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(jump(QModelIndex)));

    //slider = new QSlider(Qt::Horizontal, this);
    ui->slider->setRange(0, player->duration() / 1000);

    //labelDuration = new QLabel(this);
    connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));


    //QPushButton *openButton = new QPushButton(tr("Open"), this);

    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(open()));

    PlayerControls *controls = new PlayerControls(this);
    controls->setState(player->state());
    controls->setVolume(player->volume());
    controls->setMuted(controls->isMuted());

    connect(controls, SIGNAL(play()), player, SLOT(play()));
    connect(controls, SIGNAL(pause()), player, SLOT(pause()));
    connect(controls, SIGNAL(stop()), player, SLOT(stop()));
    connect(controls, SIGNAL(next()), playlist, SLOT(next()));
    connect(controls, SIGNAL(previous()), this, SLOT(previousClicked()));
    connect(controls, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
    connect(controls, SIGNAL(changeMuting(bool)), player, SLOT(setMuted(bool)));

    //connect(ui->controls, SIGNAL(stop()), ui->videoWidget, SLOT(update()));
    connect(controls, SIGNAL(stop()), videoWidget, SLOT(update()));

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            controls, SLOT(setState(QMediaPlayer::State)));
    connect(player, SIGNAL(volumeChanged(int)), controls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)), controls, SLOT(setMuted(bool)));

    //QBoxLayout *displayLayout = new QHBoxLayout;
    ui->displayLayout->addWidget(videoWidget, 2);
    //ui->displayLayout->addWidget(ui->playlistView);

    //QBoxLayout *controlLayout = new QHBoxLayout;
    //ui->controlLayout->setMargin(0);
    //ui->controlLayout->addWidget(ui->openButton);
    //ui->controlLayout->addStretch(1);
    ui->controlLayout->addWidget(controls);
    //ui->controlLayout->addStretch(1);

    //QBoxLayout *layout = new QVBoxLayout;
    //ui->layout->addLayout(ui->displayLayout);
    //QHBoxLayout *hLayout = new QHBoxLayout;
    //ui->hLayout->addWidget(ui->slider);
    //ui->hLayout->addWidget(ui->labelDuration);
    //ui->layout->addLayout(ui->hLayout);
    //ui->layout->addLayout(ui->controlLayout);

    //setLayout(ui->layout);

    if (!player->isAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        controls->setEnabled(false);
        ui->playlistView->setEnabled(false);
        ui->openButton->setEnabled(false);
    }

    metaDataChanged();

    QStringList arguments = qApp->arguments();
    arguments.removeAt(0);
    addToPlaylist(arguments);
}

csrplayer::~csrplayer()
{
}

void csrplayer::open()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"));
    addToPlaylist(fileNames);
}

void csrplayer::on_closeButton_clicked()
{
	close();
}

void csrplayer::addToPlaylist(const QStringList& fileNames)
{
    foreach (QString const &argument, fileNames) {
        QFileInfo fileInfo(argument);
        if (fileInfo.exists()) {
            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
            if (fileInfo.suffix().toLower() == QLatin1String("m3u")) {
                playlist->load(url);
            } else
                playlist->addMedia(url);
        } else {
            QUrl url(argument);
            if (url.isValid()) {
                playlist->addMedia(url);
            }
        }
    }
}

void csrplayer::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    ui->slider->setMaximum(duration / 1000);
}

void csrplayer::positionChanged(qint64 progress)
{
    if (!ui->slider->isSliderDown()) {
        ui->slider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void csrplayer::metaDataChanged()
{
    if (player->isMetaDataAvailable()) {
        setTrackInfo(QString("%1 - %2")
                .arg(player->metaData(QMediaMetaData::AlbumArtist).toString())
                .arg(player->metaData(QMediaMetaData::Title).toString()));

        if (coverLabel) {
            QUrl url = player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            coverLabel->setPixmap(!url.isEmpty()
                    ? QPixmap(url.toString())
                    : QPixmap());
        }
    }
}

void csrplayer::previousClicked()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if(player->position() <= 5000)
        playlist->previous();
    else
        player->setPosition(0);
}

void csrplayer::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        player->play();
    }
}

void csrplayer::playlistPositionChanged(int currentItem)
{
    ui->playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));
}

void csrplayer::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

void csrplayer::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Media Stalled"));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void csrplayer::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void csrplayer::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}

void csrplayer::videoAvailableChanged(bool available)
{
}

void csrplayer::setTrackInfo(const QString &info)
{
    trackInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

void csrplayer::setStatusInfo(const QString &info)
{
    statusInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

void csrplayer::displayErrorMessage()
{
    setStatusInfo(player->errorString());
}

void csrplayer::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    ui->labelDuration->setText(tStr);
}
