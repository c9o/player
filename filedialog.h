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

class Dialog : public QDialog
{
    Q_OBJECT

public:
		Dialog(QWidget *parent, const QString &filename, int mode);

        QFileDialog* GetFileDlg()
		{
			return m_filedlg;
        }

        QString GetFile()
        {
            return m_file;
        }

private slots:
        void currentFile(QString file)
        {
            m_fileinfo->setFile(file);
            //m_mediainfo->setItem(0, 1, new QTableWidgetItem(m_fileinfo->absolutePath()));
            //m_mediainfo->setItem(1, 1, new QTableWidgetItem(m_fileinfo->fileName()));
        }

        void selectFile(QString file)
        {
            m_file = file;
        }

        void dialogQuit();

private:
        QFileDialog         *m_filedlg;
        QFileInfo           *m_fileinfo;
        QTextEdit           *m_mediainfo;
        QTableWidgetItem    *m_item;
        QGridLayout         *m_mainLaout;
        QString              m_file;
};
#endif
