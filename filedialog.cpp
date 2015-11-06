/****************************************************************************
 **
 ** Copyright (c) [2015] Qualcomm Technologies International, Ltd.
 ** All Rights Reserved.
 ** Qualcomm Technologies Inc. Confidential and Proprietary.
 ** Not a Contribution. Notifications and licenses are retained for attribution
 ** purposes only . Your use of this code is governed by the terms of your
 ** license agreement with Qualcomm Technologies International, Ltd.(“QTIL”).
 ** Except as may be granted by separate express written agreement, this file
 ** provides no rights under or license to any QTIL or its affiliates patents,
 ** trademarks, copyrights,or other intellectual property.
 **
 ****************************************************************************/

#include "filedialog.h"

Dialog::Dialog(QWidget *parent, const QString &filename, int mode)
{
	QFileDialog *m_filedlg = new QFileDialog(parent,Qt::Dialog);
	m_filedlg->setOption(QFileDialog::DontUseNativeDialog,true);
	m_filedlg->setDirectory(filename);
	m_filedlg->setViewMode((QFileDialog::ViewMode)mode);
	m_filedlg->resize(800, 480);

	QStringList filters;
	filters << "Supported files (*.3gp *.ape *.asf *.avi *.flac *.flv *.h264 *.m4a *.m4v *.mkv *.mov *.mpeg *.mpeg2 *.mpeg4 *.mp2 *.mp4 *.mpg *.ts *.vob *.wav *.wmv *.mp3 *.ogg)"
		//<< "Video files (*.3gp *.ape *.avi *.flac *.flv *.h264 *.m4v *.mkv *.mov *.mpeg *.mpeg2 *.mpeg4 *.mp2 *.mp4 *.mpg *.ts *.wmv)"
		//<< "Audio files (*.mp3 *.ogg *.wav)"
		<< "Any files (*)";
	m_filedlg->setNameFilters(filters);

	QList<QUrl> urls;
	urls << QUrl::fromLocalFile("/media/mmcblk0p6")
		<< QUrl::fromLocalFile("/media/mmcblk1p1");
	m_filedlg->setSidebarUrls(urls);

	m_mainLaout = new QGridLayout(this);
	m_mediainfo = new QTextEdit(parent);

	m_mediainfo->setReadOnly(true);

	m_mainLaout->addWidget(m_mediainfo,  0 , 0 , 2 , 1 );
	m_mainLaout->addWidget(m_filedlg,    0 , 1 , 2 , 1 );

	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	m_fileinfo = new QFileInfo();

	QObject::connect(m_filedlg, SIGNAL(finished(int)), this, SLOT(dialogQuit()));
	QObject::connect(m_filedlg, SIGNAL(currentChanged(QString)), this, SLOT(currentFile(QString)));
	QObject::connect(m_filedlg, SIGNAL(fileSelected(QString)), this, SLOT(selectFile(QString)));

	handle = dlopen (MEDIA_LIB, RTLD_LAZY);
	if(!handle)
	{
		qDebug() << MEDIA_LIB << " open failed!";
	}

	*(void **) (&mediainfo) = dlsym(handle, "media_info");

	if(!mediainfo)
	{
		qDebug() << "Get mediainfo" << " open failed!";
	}
}

void Dialog::currentFile(QString file)
{
	m_fileinfo->setFile(file);
	if (m_fileinfo->isFile())
	{
		m_mediainfo->setFontPointSize(12);
		QByteArray filename = m_fileinfo->absoluteFilePath().toLatin1();
		if(mediainfo)(* mediainfo)(filename.data(), msg);
		m_mediainfo->setText(QString::fromStdString(msg));
	}
}

void Dialog::selectFile(QString file)
{
	m_file = file;
}

void Dialog::dialogQuit()
{
	delete m_mediainfo;
	hide();
}
