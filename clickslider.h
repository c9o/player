/**********************
 ** clickslider.h
**********************/

#ifndef CLICKSLIDER_H
#define CLICKSLIDER_H
#include <QtWidgets/QSlider>

class QSlider;
class ClickSlider : public QSlider
{
	Q_OBJECT
	
	public:
	ClickSlider(QWidget * parent = 0 );
	~ClickSlider(){;}

	protected:
	virtual void mousePressEvent ( QMouseEvent * event ) ;
};
#endif 
