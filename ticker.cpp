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
#include "ticker.h"

Ticker::Ticker(QWidget *parent)
: QLabel(parent)
{
	offset = 0;
	myTimerId = 0;
}

void Ticker::setText(const QString &newText)
{
	myText = newText;
	update();
	updateGeometry();
}

QSize Ticker::sizeHint() const
{
	return fontMetrics().size(0, text());
}

void Ticker::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);

	int textWidth = fontMetrics().width(text());
	if (textWidth < 1)
		return;
	int x = -offset;
	while (x < width()) {
		painter.drawText(x, 0, textWidth, height(),
				Qt::AlignLeft | Qt::AlignVCenter, text());
		x += textWidth;
	}
}
void Ticker::showEvent(QShowEvent * /* event */)
{
	myTimerId = startTimer(30);
}

void Ticker::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == myTimerId) {
		++offset;
		if (offset >= fontMetrics().width(text()))
			offset = 0;
		scroll(-1, 0);
	} else {
		QWidget::timerEvent(event);
	}
}
void Ticker::hideEvent(QHideEvent * /* event */)
{
	killTimer(myTimerId);
	myTimerId = 0;
}
