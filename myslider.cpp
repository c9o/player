/**********************
 ** myslider.cpp
**********************/
#include <QtGui>
#include <QStyleOptionSlider>
#include <QDebug>
#include "myslider.h"

MySlider::MySlider( QWidget * parent) :QSlider(parent)
{
 
}

void MySlider::mousePressEvent ( QMouseEvent * mouseEvent )
{
 if(mouseEvent->button()==Qt::LeftButton && isEnabled())
 {
  int duration = maximum() - minimum();
  int pos = minimum() + duration * ((double)mouseEvent->x() / width());
  qDebug() << Q_FUNC_INFO << " pos" << pos;
  if(pos != sliderPosition())
  { 
   setValue(pos);     
  }
  // else ok.
 }
}

/*
void MySlider::mousePressEvent ( QMouseEvent * event )
{
  QStyleOptionSlider opt;
  initStyleOption(&opt);
  QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

  if (event->button() == Qt::LeftButton &&
      sr.contains(event->pos()) == false)
  {
    int newVal;
    if (orientation() == Qt::Vertical)
       newVal = minimum() + ((maximum()-minimum()) * (height()-event->y())) / height();
    else
       newVal = minimum() + ((maximum()-minimum()) * event->x()) / width();

    if (invertedAppearance() == true)
        setValue( maximum() - newVal );
    else
        setValue(newVal);

    event->accept();
  }
  QSlider::mousePressEvent(event);
}
*/
