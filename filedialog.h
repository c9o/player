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

#ifndef __FILE_DIALOG_H__
#define __FILE_DIALOG_H__

#include <assert.h>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <dlfcn.h>

#define MEDIA_LIB "libmediainfo.so.0"
typedef void (*M_FUNC)(char *, char *);

class Dialog : public QDialog
{
    Q_OBJECT

public:
		Dialog(QWidget *parent, const QString &filename, int mode);

        QFileDialog* GetFileDlg()
		{
            if (handle) dlclose(handle);
			return m_filedlg;
        }

        QString GetFile()
        {
            return m_file;
        }

private slots:
        void currentFile(QString file);
        void selectFile(QString file);
        void dialogQuit();

private:
        QFileDialog         *m_filedlg;
        QFileInfo           *m_fileinfo;
        QTextEdit           *m_mediainfo;
        QTableWidgetItem    *m_item;
        QGridLayout         *m_mainLaout;
        QString              m_file;

        void *handle;
        M_FUNC mediainfo;
        char msg[4096];
};
#endif
