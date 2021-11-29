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

#include "XDecode.h"
#include "IVideoCall.h"
#include "XDecodeThread.h"

struct AVCodecParameters;
struct AVPacket;
class XVideoThread :public XDecodeThread
{
public:
    virtual bool open(AVCodecParameters *para,IVideoCall *call,int width ,int height);
    void run() override;
    XVideoThread();
    virtual ~XVideoThread();
    long long synPts = 0;
    void setPause(bool pause);
protected:

private:
    std::mutex vMux;
    IVideoCall *m_call{nullptr};
    bool m_isPause = false;
};


#endif //FFMPEGTEST_XVIDEOTHREAD_H
