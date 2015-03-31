#ifndef __FILE_DIALOG_H__
#define __FILE_DIALOG_H__

#include <QLineEdit>
#include <QWidget>
#include <QGridLayout>
#include <QFileDialog>
#include <assert.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QDebug>

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
            qDebug() << "select file " << file;
            textFileName->setText(file);
            m_file = file;
        }
        void currentDirectory(QString dir)
        {
            qDebug() << "enter dir " << dir;
            textDirName->setText(dir);
        }

private:
        QFileDialog *m_filedlg;
        QString m_file;
        QLabel *textDir;
        QLabel *textDirName;
        QLabel *textFile;
        QLabel *textFileName;

        QGridLayout *mainLayout;
};
#endif
