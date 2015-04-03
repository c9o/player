#include "filedialog.h"

Dialog::Dialog(QWidget *parent, const QString &filename, int mode)
{
    QFileDialog *m_filedlg = new QFileDialog(parent,Qt::Dialog);
    m_filedlg->setOption(QFileDialog::DontUseNativeDialog,true);
    m_filedlg->setDirectory(filename);
    m_filedlg->setViewMode((QFileDialog::ViewMode)mode);
    m_filedlg->resize(800, 480);

    QStringList filters;
    filters << "Video files (*.avi *.mp4 *.mkv *.mov *.3gp *.mpeg *.mpg *.rmvb *.rm *.ts)"
            << "Music files (*.mp3 *.ogg)"
            << "Any files (*)";
    m_filedlg->setNameFilters(filters);

    m_mediainfo = new QTableWidget(parent);
    m_mainLaout = new QGridLayout(this);

    m_mediainfo->setColumnCount(2);
    m_mediainfo->setRowCount(2);
    m_mediainfo->horizontalHeader()->setVisible(false);
    m_mediainfo->verticalHeader()->setVisible(false);
    m_mediainfo->setFrameShape(QFrame::NoFrame);
    m_mediainfo->setShowGrid(false);
    m_mediainfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_mediainfo->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_mediainfo->horizontalHeader()->setStretchLastSection(true);
    //m_mediainfo->horizontalHeader()->resizeSection(0,100);
    m_mediainfo->setItem(0, 0, new QTableWidgetItem("Path"));
    m_mediainfo->setItem(1, 0, new QTableWidgetItem("Name"));

    m_mainLaout->addWidget(m_mediainfo,    0 , 0 , 2 , 1 );
    m_mainLaout->addWidget(m_filedlg,    1 , 1 , 1 , 1 );

    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_filedlg->setGeometry(QRect(0,0,200,100));

    m_fileinfo = new QFileInfo();

    QObject::connect(m_filedlg, SIGNAL(finished(int)), this, SLOT(dialogQuit()));
    QObject::connect(m_filedlg, SIGNAL(currentChanged(QString)), this, SLOT(currentFile(QString)));
    QObject::connect(m_filedlg, SIGNAL(fileSelected(QString)), this, SLOT(selectFile(QString)));
}

void Dialog::dialogQuit()
{
    delete m_mediainfo;
    hide();
}
