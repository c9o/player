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
