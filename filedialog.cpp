#include "filedialog.h"

Dialog::Dialog(QWidget *parent, const QString &filename, int mode)
{
    QFileDialog *m_filedlg = new QFileDialog(this,Qt::SubWindow);
    m_filedlg->setOption(QFileDialog::DontUseNativeDialog,true);
    m_filedlg->setDirectory(filename);
    m_filedlg->setViewMode((QFileDialog::ViewMode)mode);
    m_filedlg->resize(800, 480);

    textLabel = new QLabel( tr("StepX: ") );
    textEdit = new QLineEdit();
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget( m_filedlg , 0 , 0 , 6 , 1 ) ;
    mainLayout->addWidget( textLabel , 1 , 1 , 1 , 1 ) ;
    mainLayout->addWidget(textEdit , 1 , 2 , 1 , 1 ) ;

    setMinimumHeight( 400 ) ;
    setMinimumWidth( 600 ) ;
    setSizePolicy( QSizePolicy( QSizePolicy::Expanding , QSizePolicy::Expanding ) ) ;

    QObject::connect(m_filedlg, SIGNAL(finished(int)), this, SLOT(done(int)));
}

int Dialog::receive(QString file)
{
    qDebug() << "select file " << file;
    return 0;
}
