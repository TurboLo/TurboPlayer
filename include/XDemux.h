/************************************************************
 *
 *          Project Name : XDemux
 *
 *              FileName : FFMPEGTEST_XDEMUX_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/18 17:37
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_XDEMUX_H
#define FFMPEGTEST_XDEMUX_H

#include <string>
#include <mutex>
#include "FFMPegLib.h"
struct AVFormatContext;
struct AVPacket;
class XDemux
{
public:
    XDemux();
    virtual ~XDemux();

    virtual bool open(const char *url);
    // 空间需要调用者释放，释放对象为AVpacket对象空间和数据空间 av_packet_free
    virtual AVPacket *read();

    int totalMs = 0;
private:
    std::mutex mux;

    AVFormatContext *m_ic{nullptr};

    int videoStream = 0;
    int audioStream = 1;
    char *m_bTcp{};
};


#endif //FFMPEGTEST_XDEMUX_H
