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
