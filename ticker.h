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

#ifndef TICKER_H
#define TICKER_H
#include <QLabel>
class Ticker : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
public:
    Ticker(QWidget *parent = 0);
    void setText(const QString &newText);
    QString text() const { return myText; }
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
private:
    QString myText;
    int offset;
    int myTimerId;
};
#endif
