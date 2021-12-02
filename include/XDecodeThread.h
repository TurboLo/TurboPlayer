/************************************************************
 *
 *          Project Name : XDecodeThread
 *
 *              FileName : FFMPEGTEST_XDECODETHREAD_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/27 09:45
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_XDECODETHREAD_H
#define FFMPEGTEST_XDECODETHREAD_H
#include "XDecode.h"
struct AVCodecParameters;
struct AVPacket;
#include <QThread>
#include <list>
#include <mutex>
#include "FFMPegLib.h"
class XDecodeThread :public QThread
{
public:
    virtual void push(AVPacket *pkt);
    virtual AVPacket *pop();
    virtual void clear();
    // 清理资源，关闭线程
    virtual void close();
    XDecodeThread();
    virtual ~XDecodeThread();
protected:
    std::list<AVPacket *> packet;
    std::mutex mux;
    bool isExit = false;
    int maxList = 100;
    XDecode *m_decode{nullptr};
private:
};


#endif //FFMPEGTEST_XDECODETHREAD_H
