#include "msgthread.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define APP_ID 6

MsgThread::MsgThread(QObject *parent, int msgqid1, int msgqid2) :
    QThread(parent),
    msgQId1(msgqid1),
    msgQId2(msgqid2)
{
}

MsgThread::~MsgThread()
{
}

void MsgThread::setMessageQueueId(int id1, int id2)
{
    //LOGF("setMessageQueueId(%d, %d)", id1, id2);

    msgQId1 = id1;
    msgQId2 = id2;
}

void MsgThread::sendMessage(int messageQueueID,int sMsg)
{

    msg_buf sbuf;

    sbuf.mtype = 1;
    sbuf.event = sMsg;
    sbuf.appId = APP_ID;
    size_t buf_length = sizeof(sbuf);

    //LOGD("starts to send message");

    int returnValue = msgsnd(messageQueueID, &sbuf, buf_length, IPC_NOWAIT) ;

    if( returnValue < 0)
    {
        //LOGE("msgsnd failed - %d", returnValue);
        perror("msgsend");
    }

    //LOGD("the message has been sent");
}

int MsgThread::receiveMessage(int messageQueueID)
{
    /* DO NOT use printf in this function.
       Or it will cause crash, cause unknown */
    msg_buf rbuf;

    //qDebug("<Radio - messageQueue>waiting for the message to receive \n");

    int status = msgrcv(messageQueueID,&rbuf, sizeof(rbuf), 0, 0);

    if(status < 0)
    {
      return -1;
    }

    //qDebug("<Radio - messageQueue>the message received is %d\n"
    //       , rbuf.event);

    return rbuf.event;
}

void MsgThread::run()
{
    int msg;
    while (true)
    {
        msg = receiveMessage(msgQId2);
        switch (msg)
        {
        case APP_RESUME:
            emit appResume();
            break;
        case APP_STOP:
            emit appStop();
            break;
        default:
            break;
        }
    }
}

void MsgThread::onAppPaused()
{
    //LOGF(__FUNCTION__);
    sendMessage(msgQId1, APP_PAUSED);
}
