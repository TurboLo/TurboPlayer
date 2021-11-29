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
#include "XDecodeThread.h"

class XAudioThread :public XDecodeThread
{
public:
    long long pts = 0;
    virtual bool open(AVCodecParameters *para);
    virtual void close();
    void run() override;
    XAudioThread();
    virtual ~XAudioThread();
    void setPause(bool pause);

protected:

private:
    std::mutex aMux;
    XResample *m_resample{nullptr};
    bool m_isPause;

};


#endif //FFMPEGTEST_XAUDIOTHREAD_H
