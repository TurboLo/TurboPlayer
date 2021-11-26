/************************************************************
 *
 *          Project Name : XVideoThread
 *
 *              FileName : FFMPEGTEST_XVIDEOTHREAD_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/26 15:58
 *
 *           Last Update : 
 *
 ***********************************************************/


#ifndef FFMPEGTEST_XVIDEOTHREAD_H
#define FFMPEGTEST_XVIDEOTHREAD_H
#include <QThread>
#include "XDecode.h"
#include "IVideoCall.h"

struct AVCodecParameters;
struct AVPacket;
class XVideoThread :public QThread
{
public:
    virtual bool open(AVCodecParameters *para,IVideoCall *call,int width ,int height);
    virtual void push(AVPacket *pkt);
    void run() override;

    XVideoThread();
    virtual ~XVideoThread();
    bool isExit = false;
    int maxList = 100;
protected:

private:
    std::list<AVPacket *> packet;
    std::mutex mux;
    XDecode *m_decode{nullptr};
    IVideoCall *m_call{nullptr};
};


#endif //FFMPEGTEST_XVIDEOTHREAD_H
