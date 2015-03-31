#include "filedialog.h"

Dialog::Dialog(QWidget *parent, const QString &filename, int mode)
{
    QFileDialog *m_filedlg = new QFileDialog(parent,Qt::Dialog);
    m_filedlg->setOption(QFileDialog::DontUseNativeDialog,true);
    m_filedlg->setDirectory(filename);
    m_filedlg->setViewMode((QFileDialog::ViewMode)mode);
    m_filedlg->resize(400, 240);

    textDir = new QLabel(tr("Directory:"));
    textDirName = new QLabel(tr("          "));
    textFile = new QLabel(tr("Filename:"));
    textFileName = new QLabel(tr("          "));

    mainLayout = new QGridLayout(this);

    mainLayout->addWidget(m_filedlg,    0 , 0 , 4 , 1 ) ;
    mainLayout->addWidget(textDir,      1 , 1 , 1 , 1 ) ;
    mainLayout->addWidget(textDirName,  1 , 2 , 1 , 1 ) ;
    mainLayout->addWidget(textFile,     2 , 1 , 1 , 1 ) ;
    mainLayout->addWidget(textFileName, 2 , 2 , 1 , 1 ) ;

    //setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed)) ;

    QObject::connect(m_filedlg, SIGNAL(finished(int)), this, SLOT(done(int)));
    QObject::connect(m_filedlg, SIGNAL(directoryEntered(QString)), this, SLOT(currentDirectory(QString)));
    QObject::connect(m_filedlg, SIGNAL(currentChanged(QString)), this, SLOT(currentFile(QString)));
}
