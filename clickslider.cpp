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

#include <QtGui>
#include <QStyleOptionSlider>
#include <QDebug>
#include "clickslider.h"

ClickSlider::ClickSlider( QWidget * parent) :QSlider(parent)
{

}

void ClickSlider::mousePressEvent ( QMouseEvent * event )
{
	QStyleOptionSlider opt;
	initStyleOption(&opt);
    int pos = 0;
	QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    pos = value();

	if (event->button() == Qt::LeftButton &&
			sr.contains(event->pos()) == false)
	{
		int newVal;
		if (orientation() == Qt::Vertical)
			newVal = minimum() + ((maximum()-minimum()) * (height()-event->y())) / height();
		else
			newVal = minimum() + ((maximum()-minimum()) * event->x()) / width();

		if (invertedAppearance() == true)
		{
            setValue(maximum() - newVal);
			pos = maximum() - newVal;
		}
		else
		{
			setValue(newVal);
			pos = newVal;
		}

		event->accept();
	}
	QSlider::mousePressEvent(event);
	emit(sliderMoved(pos));
}
