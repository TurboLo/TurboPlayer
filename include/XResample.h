/************************************************************
 *
 *          Project Name : XResample
 *
 *              FileName : FFMPEGTEST_XRESAMPLE_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/26 14:06
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_XRESAMPLE_H
#define FFMPEGTEST_XRESAMPLE_H

#include <mutex>
#include "FFMPegLib.h"
struct AVCodecParameters;
struct AVFrame;
struct SwrContext;
class XResample

{
public:
    virtual bool open(AVCodecParameters *para,bool isClear = false);
    virtual void close();

    virtual int resample(AVFrame *inDate,unsigned char *data);
    XResample();
    ~XResample();
    int outFormat = 1;
protected:

private:
    std::mutex mux;
    SwrContext  *m_actx{};

};


#endif //FFMPEGTEST_XRESAMPLE_H
