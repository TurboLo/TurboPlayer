/************************************************************
 *
 *          Project Name : XDecode
 *
 *              FileName : FFMPEGTEST_XDECODE_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/25 14:52
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_XDECODE_H
#define FFMPEGTEST_XDECODE_H

#include <mutex>

extern "C"
{
#include <libavcodec/avcodec.h>
};

struct AVCodecParameters;
struct AVCodecContext;

class XDecode
{
public:
    long long pts = 0;
    bool isAudio = false;
    // 打开解码器,并且释放解码器
    virtual bool open(AVCodecParameters *para);

    // 发送到解码线程，且释放pkt空间
    virtual bool send(AVPacket *pkt);
    // 获取解码数据，一次发送多次接受
    // 由调用者释放
    virtual AVFrame *receive();

    virtual void close();
    virtual void clear();

    XDecode();
    virtual ~XDecode();

private:
    AVCodecContext *m_codec{nullptr};
    std::mutex mux;
};


#endif //FFMPEGTEST_XDECODE_H
