/************************************************************
 *
 *          Project Name : XDemuxThread
 *
 *              FileName : FFMPEGTEST_XDEMUXTHREAD_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/26 16:23
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_XDEMUXTHREAD_H
#define FFMPEGTEST_XDEMUXTHREAD_H

#include <QThread>
#include "IVideoCall.h"
#include "XDemux.h"
#include "XVideoThread.h"
#include "XAudioThread.h"

class XDemuxThread : public QThread
{
public:
    virtual bool open(const char *url,IVideoCall *call);
    virtual void start();
    void run() override;
    XDemuxThread();
    virtual ~XDemuxThread() override;
    bool isExit = false;
protected:

private:
    std::mutex mux;
    XDemux *m_demux{nullptr};
    XVideoThread *m_vt{nullptr};
    XAudioThread *m_at{nullptr};
};


#endif //FFMPEGTEST_XDEMUXTHREAD_H
