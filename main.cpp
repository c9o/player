/****************************************************************************
 **
 ** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include "csrplayer.h"

#include <QApplication>
#include "getopt.h"

#define APP_MGR_PROCESS_NAME "AppMgr"
#define NATIVE_APP_MSG_QID_ARG "nMsgId"
#define APP_MANAGER_MSG_QID_ARG "rMsgId"

int main(int argc, char *argv[])
{

    /* Response Msg Queue id from application to AppMgr */
    /* AppMgr (waiting on respAppMqId)<---Sends--- Application */
    int messageQueueID1 = -1;

    /* Send Msg Queue id to launched application */
    /* AppMgr ----Sends---> Application (waiting on sendAppMqId) */
    int messageQueueID2 = -1;

    /* Parse args and get required msg queue id's :*/
    struct option long_options[] =
    {
        /* Ignore */
    {"platform", required_argument, 0, 'p'},
    {"plugin", required_argument, 0, 'P'},
    /* Args to parse */
    /* AppMgr <--- Application */
    {NATIVE_APP_MSG_QID_ARG, required_argument, 0, 'n'},
    /* AppMgr ---> Application */
    {APP_MANAGER_MSG_QID_ARG, required_argument, 0, 'r'},
    };

    int c;
    int opt_index = 0;
    while ((c = getopt_long_only(argc, argv, "n:r:p:P", long_options, &opt_index)) != -1)
    {
        switch (c)
        {
        case 'n':
            printf("App Response MsgId: %s\n", optarg);
            messageQueueID1 = atoi(optarg);
            break;
        case 'r':
            printf("App Receive MsgId: %s\n", optarg);
            messageQueueID2 = atoi(optarg);
            break;
        default:
            break;
        }
    }
	QApplication app(argc, argv);

	csrplayer player;
	player.resize(800,480);
	player.showFullScreen();
	//player.show();
	return app.exec();
}
