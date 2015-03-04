/**********************
 ** myslider.h
**********************/

#ifndef MYSLIDER_H
#define MYSLIDER_H
#include <QtWidgets/QSlider>

class QSlider;
class MySlider : public QSlider
{
	Q_OBJECT
	
	public:
	MySlider(QWidget * parent = 0 );
	~MySlider(){;}

	protected:
	virtual void mousePressEvent ( QMouseEvent * event ) ;
};
#endif 
