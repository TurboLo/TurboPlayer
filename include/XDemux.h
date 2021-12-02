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
struct AVCodecParameters;
class XDemux
{
public:
    XDemux();
    virtual ~XDemux();

    virtual bool open(const char *url);
    // 空间需要调用者释放，释放对象为AVpacket对象空间和数据空间 av_packet_free
    virtual AVPacket *read();

    virtual bool isAudio(AVPacket *pkt);
    // 获取视频参数,返回空间需要清理
    virtual AVCodecParameters  *copyVPara();
    // 获取音频参数
    virtual AVCodecParameters  *copyAPara();

    virtual bool seek(double pos);

    virtual void clear();

    virtual void close();


    int m_sampleRate = 0;
    int m_sampleSize = 0;
    int m_channels = 0;
    long long totalMs = 0;
    int width;
    int height;
    int videoStream = 0;
    int audioStream = 1;
private:
    std::mutex mux;

    AVFormatContext *m_ic{nullptr};


    char *m_bTcp{};
};


#endif //FFMPEGTEST_XDEMUX_H
