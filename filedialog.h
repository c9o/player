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
        int receive(QString file);

	private:
        QFileDialog *m_filedlg;
        QLabel *textLabel;
        QLineEdit *textEdit;

        QGridLayout *mainLayout;
};
#endif
