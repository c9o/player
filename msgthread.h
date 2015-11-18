#ifndef MSGTHREAD_H
#define MSGTHREAD_H

#include <QThread>
#include <pthread.h>

typedef struct  {
       /*!
        * \brief mType - App Mgr message with the event
        */
       int mtype;

       /*!
        * \brief event - App manager events (Launch, request for display or exit)
        */
       int event;

       /*!
        * \brief applicationID - Calling application ID.
        */
       int appId;

}msg_buf;

/* Commands from Launcher to app */
#define APP_RESUME                         101
#define APP_STOP                           102
#define APP_QUITTING                       103

/* Commands from app to Launcher */
#define APP_PAUSED                      102
#define APP_STARTED                     103
#define APP_RESUMED                     104

class MsgThread : public QThread
{
    Q_OBJECT
public:
    explicit MsgThread(QObject *parent = 0
            , int msgqid1 = 0, int msgqid2 = 0);
    ~MsgThread();

    void setMessageQueueId(int,int);
    void sendMessage(int , int);
    int receiveMessage(int);

protected:
    void run();

signals:
    void appResume();
    void appStop();

public slots:
    void onAppPaused();

private:
    int msgQId1; //messeage ID to send to AppMgr
    int msgQId2; //messeage ID to receive message
};

#endif // MSGTHREAD_H
