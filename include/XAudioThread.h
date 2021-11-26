/************************************************************
 *
 *          Project Name : XAudioThread
 *
 *              FileName : FFMPEGTEST_XAUDIOTHREAD_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/26 15:21
 *
 *           Last Update : 
 *
 ***********************************************************/
#ifndef FFMPEGTEST_XAUDIOTHREAD_H
#define FFMPEGTEST_XAUDIOTHREAD_H

#include <QThread>
#include "FFMPegLib.h"
#include "XDecode.h"
#include "XAudioPlay.h"
#include "XResample.h"

struct AVCodecParameters;
struct AVPacket;
class XAudioThread :public QThread
{
public:
    virtual bool open(AVCodecParameters *para);
    virtual void push(AVPacket *pkt);
    void run();
    XAudioThread();
    virtual ~XAudioThread();

    bool isExit = false;
    int maxList = 100;
protected:

private:
    std::list<AVPacket *> packet;
    std::mutex mux;
    XDecode *m_decode{nullptr};
    XResample *m_resample{nullptr};
};


#endif //FFMPEGTEST_XAUDIOTHREAD_H
