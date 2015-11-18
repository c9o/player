#include "csrplayer.h"
#include "ui_csrplayer.h"
#include "playercontrols.h"
#include "playlistmodel.h"
#include "filedialog.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QMediaContent>

#ifdef DEBUG_OPEN
#include <QDebug>
#endif

csrplayer::csrplayer(QWidget *parent)
    : QMainWindow(parent)
	, ui(new Ui::csrplayer)
    , coverLabel(0)
{
#ifdef DEBUG_OPEN
	qDebug() << "player created";
#endif
	ui->setupUi(this);
//! [create-objs]
    player = new QMediaPlayer(this);
    // owned by PlaylistModel
    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
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
    player->setVideoOutput(ui->videoWidget);

    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
//! [2]

#ifdef ENABLE_PLAYLISTVIEW
    playlistView = new QListView();
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHeightForWidth(playlistView->sizePolicy().hasHeightForWidth());
    playlistView->setObjectName(QStringLiteral("playlistView"));
    playlistView->setSizePolicy(sizePolicy1);
    playlistView->setMinimumSize(QSize(0, 0));
    playlistView->setMaximumSize(QSize(16777215, 16777215));
    playlistView->setStyleSheet(QStringLiteral(""));
    playlistView->setModel(playlistModel);
    playlistView->setStyleSheet ("font: 24pt \"Courier\";");
    playlistView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));
    ui->displayLayout->addWidget(playlistView);

    connect(playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(jump(QModelIndex)));
#endif

    ui->slider->setRange(0, player->duration() / 1000);

    connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

    ui->openButton->setIcon(QIcon(":/icons/resources/ic_open.png"));
    ui->openButton->setIconSize(QSize(40, 40));
	ui->openButton->setEnabled(true);
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(setOpenEnabled(QMediaPlayer::State)));
    ui->minimizeButton->setIcon(QIcon(":/icons/resources/ic_minimize.png"));
    ui->minimizeButton->setIconSize(QSize(40, 40));
    ui->minimizeButton->setEnabled(true);
    connect(ui->minimizeButton, SIGNAL(clicked()), this, SLOT(playerMinimize()));
    ui->closeButton->setIcon(QIcon(":/icons/resources/ic_close.png"));
    ui->closeButton->setIconSize(QSize(40, 40));
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(open()));

    ui->controls->setState(player->state());
    ui->controls->setVolume(player->volume());
    ui->controls->setMuted(ui->controls->isMuted());

    connect(ui->controls, SIGNAL(play()), player, SLOT(play()));
    connect(ui->controls, SIGNAL(pause()), player, SLOT(pause()));
    connect(ui->controls, SIGNAL(stop()), player, SLOT(stop()));
    connect(ui->controls, SIGNAL(next()), playlist, SLOT(next()));
    connect(ui->controls, SIGNAL(previous()), this, SLOT(previousClicked()));
    connect(ui->controls, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
    connect(ui->controls, SIGNAL(changeMuting(bool)), player, SLOT(setMuted(bool)));
    connect(ui->controls, SIGNAL(changeMode(int)), this, SLOT(setMode(int)));

    connect(ui->controls, SIGNAL(stop()), ui->videoWidget, SLOT(update()));

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            ui->controls, SLOT(setState(QMediaPlayer::State)));
    connect(player, SIGNAL(volumeChanged(int)), ui->controls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)), ui->controls, SLOT(setMuted(bool)));
    connect(this, SIGNAL(playModeChanged(int)), ui->controls, SLOT(setMode(int)));

    m_pMsgThread = new MsgThread(this, id1, id2);
    if (id1 != -1 && id2 != -1)
    {
        connect(m_pMsgThread, SIGNAL(appResume()), this, SLOT(playerShow()));
        connect(this, SIGNAL(appPaused()), m_pMsgThread, SLOT(onAppPaused()));
        m_pMsgThread->start();
    }

    if (!player->isAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        ui->controls->setEnabled(false);
#ifdef ENABLE_PLAYLISTVIEW
        playlistView->setEnabled(false);
#endif
    }

    metaDataChanged();

    QStringList arguments = qApp->arguments();
    arguments.removeAt(0);
    addToPlaylist(arguments);
}

csrplayer::~csrplayer()
{
#ifdef DEBUG_OPEN
	qDebug() << "Player disconstructed";
#endif
	delete ui;
	delete player;
	delete playlist;
	delete coverLabel;
    delete playlistModel;
#ifdef ENABLE_PLAYLISTVIEW
    delete playlistView;
#endif
    delete m_pMsgThread;
}

void csrplayer::open()
{
#ifdef DEBUG_OPEN
	qDebug() << "Open button pressed";
	qDebug() << ui->videoWidget->geometry();
#endif

#if 1
    //QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"));
    //addToPlaylist(fileNames);

	QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setOption(QFileDialog::DontUseNativeDialog,true);

    QLabel *fileTypeLabel = fileDialog->findChild<QLabel*>("fileTypeLabel");
    fileTypeLabel->hide();

    QLabel *fileNameLabel = fileDialog->findChild<QLabel*>("fileNameLabel");
    fileNameLabel->hide();

    QLabel *lookInLabel = fileDialog->findChild<QLabel*>("lookInLabel");
    lookInLabel->hide();

    QToolButton *backButton = fileDialog->findChild<QToolButton*>("backButton");
    backButton->hide();

    QToolButton *detailModeButton = fileDialog->findChild<QToolButton*>("detailModeButton");
    detailModeButton->hide();

    QToolButton *forwardButton = fileDialog->findChild<QToolButton*>("forwardButton");
    forwardButton->hide();

    QToolButton *listModeButton = fileDialog->findChild<QToolButton*>("listModeButton");
    listModeButton->hide();

    QToolButton *newFolderButton = fileDialog->findChild<QToolButton*>("newFolderButton");
    newFolderButton->hide();

    QToolButton *toParentButton = fileDialog->findChild<QToolButton*>("toParentButton");
    toParentButton->hide();

    QDialogButtonBox *buttonBox = fileDialog->findChild<QDialogButtonBox*>("buttonBox");
    //buttonBox->hide();
    buttonBox->setOrientation(Qt::Horizontal);

    QComboBox *fileTypeCombo = fileDialog->findChild<QComboBox*>("fileTypeCombo");
    fileTypeCombo->hide();

	fileDialog->setWindowTitle(tr("Open File"));
    fileDialog->setDirectory("/media/mmcblk0p6/");
	fileDialog->setStyleSheet ("font: 32pt \"Courier\";");
    fileDialog->setWindowFlags(Qt::Window);
    fileDialog->showFullScreen();
    fileDialog->setViewMode(QFileDialog::List);
	if(fileDialog->exec() == QDialog::Accepted) {
                QStringList fileNames = fileDialog->selectedFiles();
    addToPlaylist(fileNames);
	}
#else
    Dialog *fileDialog = new Dialog(this, "/media/mmcblk1p1/", QFileDialog::List);
	fileDialog->setWindowTitle(tr("Open File"));
    fileDialog->setStyleSheet ("font: 20pt \"Courier\";");
    //fileDialog->resize(1024, 600);
    fileDialog->showMaximized();
    fileDialog->exec();
#ifdef DEBUG_OPEN
    qDebug()<<fileDialog->GetFile();
#endif
    addToPlaylist(QStringList(fileDialog->GetFile()));

#endif

#ifdef USE_V4L2sink
#ifdef ENABLE_PLAYLISTVIEW
    player->setOverlay(ui->videoWidget->geometry().y(), ui->videoWidget->geometry().x(), ui->videoWidget->geometry().width(), ui->videoWidget->geometry().height());
#els

    if (ui->videoWidget->geometry().width() * DEFAULT_H > ui->videoWidget->geometry().height() * DEFAULE_W)
    {
        x = ui->videoWidget->geometry().x() + ui->videoWidget->geometry().width() / 2 - ui->videoWidget->geometry().height() * DEFAULE_W / 2 / DEFAULT_H;
        y = ui->videoWidget->geometry().y();
        w = ui->videoWidget->geometry().height() * DEFAULE_W / DEFAULT_H;
        h = ui->videoWidget->geometry().height();
    }
    else
    {
        x = ui->videoWidget->geometry().x();
        y = ui->videoWidget->geometry().y() + ui->videoWidget->geometry().height() / 2 - ui->videoWidget->geometry().width() * DEFAULT_H / 2 / DEFAULE_W;
        w = ui->videoWidget->geometry().width();
        h = ui->videoWidget->geometry().width() * DEFAULT_H / DEFAULE_W;
    }
#ifdef DEBUG_OPEN
    qDebug() << DEFAULE_W << DEFAULT_H;
    qDebug() << x << y << w << h;
#endif
    player->setOverlay(y, x, w, h);
#endif
#endif
}

void csrplayer::setOpenEnabled(QMediaPlayer::State state)
{
    switch (state) {
		case QMediaPlayer::StoppedState:
		ui->openButton->setEnabled(true);
        break;
        case QMediaPlayer::PlayingState:
        ui->openButton->setEnabled(false);
        break;
        case QMediaPlayer::PausedState:
        ui->openButton->setEnabled(false);
        break;
		}
}

void csrplayer::on_closeButton_clicked()
{
#ifdef DEBUG_OPEN
	qDebug() << "Close button pressed";
#endif
	close();
}

void csrplayer::addToPlaylist(const QStringList& fileNames)
{
#ifdef DEBUG_OPEN
	qDebug() << "Add" << fileNames << " to playlist";
#endif
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

void csrplayer::playerMinimize()
{
    player->setOverlay(0, 0, 1, 1);
    hide();
    emit appPaused();
}

void csrplayer::playerShow()
{
    show();
    player->setOverlay(y, x, w, h);
}

void csrplayer::durationChanged(qint64 duration)
{
    this->duration = int(duration/1000.0 + 0.5);
    ui->slider->setMaximum(int(duration/1000.0 + 0.5));
}

void csrplayer::positionChanged(qint64 progress)
{
    if (!ui->slider->isSliderDown()) {
        ui->slider->setValue(progress/1000);
    }
    updateDurationInfo(progress/1000);
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
#ifdef DEBUG_OPEN
	qDebug() << "Previous button pressed";
#endif
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if(player->position() <= 5000)
        playlist->previous();
    else
        player->setPosition(0);
}

void csrplayer::setMode(int mode)
{
#ifdef DEBUG_OPEN
    qDebug() << "Set mode" << mode;
    qDebug() << "Current play mode is " << playlist->playbackMode();
#endif

    switch (mode) {
    case 0:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        emit playModeChanged(0);
        break;
    case 1:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        emit playModeChanged(1);
        break;
    case 2:
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        emit playModeChanged(2);
        break;
    case 3:
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        emit playModeChanged(3);
        break;
    case 4:
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        emit playModeChanged(4);
        break;
    default:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        emit playModeChanged(0);
        break;
    }
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
    QUrl fileUrl = playlistModel->playlist()->currentMedia().canonicalUrl();
#if 0
#ifdef ENABLE_PLAYLISTVIEW
    playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));
    ui->label->setText(fileUrl.fileName());
#else
    QString dispName = fileUrl.fileName();
    if (dispName.length() <= 32) dispName += QString(2*(36-dispName.length()), ' ');
    else
        dispName += "    ";

    ui->label->setText(dispName);
#endif
#endif

#ifdef DEBUG_OPEN
    qDebug() << fileUrl.fileName();
    qDebug() << fileUrl.fileName().length();
#endif
}

void csrplayer::seek(int seconds)
{
#ifdef DEBUG_OPEN
	qDebug() << "Seek to" << seconds << " seconds";
#endif
    player->setPosition(seconds * 1000);
}

void csrplayer::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);
#ifdef DEBUG_OPEN
	qDebug() << "Status change to" << status;
#endif

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

void csrplayer::videoAvailableChanged(bool )
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
