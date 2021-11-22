
#include <iostream>
#include "XDemux.h"

static double r2d(AVRational r)
{
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}


XDemux::XDemux()
{
    static bool isFirst = true;
    mux.lock();
    if(isFirst)
    {
        av_register_all();
        avformat_network_init();
        isFirst = false;
    }
    mux.unlock();
}

XDemux::~XDemux()
{

}

bool XDemux::open(const char *url)
{
    //参数设置
    AVDictionary *opts = NULL;
    //设置rtsp流已tcp协议打开
    av_dict_set(&opts, "rtsp_transport",  "tcp", 0);

    //网络延时时间
    av_dict_set(&opts, "max_delay", "3000", 0);

    mux.lock();
    int re = avformat_open_input(
            &m_ic,
            url,
            0,  // 0表示自动选择解封器
            &opts //参数设置，比如rtsp的延时时间
    );
    if (re != 0)
    {
        mux.unlock();
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        std::cout << "open " << url << " failed! :" << buf << std::endl;
        return false;
    }
    std::cout << "open " << url << " success! " << std::endl;

    //获取流信息
    re = avformat_find_stream_info(m_ic, 0);

    //总时长 毫秒
    int totalMs = m_ic->duration / (AV_TIME_BASE / 1000);
    std::cout << "totalMs = " << totalMs << std::endl;

    //打印视频流详细信息
    av_dump_format(m_ic, 0, url, 0);

    //获取视频流
    videoStream = av_find_best_stream(m_ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    AVStream *as = m_ic->streams[videoStream];

    std::cout << "=======================================================" << std::endl;
    std::cout << videoStream << "视频信息" << std::endl;
    std::cout << "codec_id = " << as->codecpar->codec_id << std::endl;
    std::cout << "format = " << as->codecpar->format << std::endl;
    std::cout << "width=" << as->codecpar->width << std::endl;
    std::cout << "height=" << as->codecpar->height << std::endl;
    //帧率 fps 分数转换
    std::cout << "video fps = " << r2d(as->avg_frame_rate) << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << audioStream << "音频信息" << std::endl;
    //获取音频流
    audioStream = av_find_best_stream(m_ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    as = m_ic->streams[audioStream];
    std::cout << "codec_id = " << as->codecpar->codec_id << std::endl;
    std::cout << "format = " << as->codecpar->format << std::endl;
    std::cout << "sample_rate = " << as->codecpar->sample_rate << std::endl;
    //AVSampleFormat;
    std::cout << "channels = " << as->codecpar->channels << std::endl;
    //一帧数据？？ 单通道样本数
    std::cout << "frame_size = " << as->codecpar->frame_size << std::endl;
    //1024 * 2 * 2 = 4096  fps = sample_rate/frame_size
    mux.unlock();


    return true;
}

AVPacket *XDemux::read()
{
    mux.lock();
    if(!m_ic)
    {
        mux.unlock();
        return nullptr;
    }
    AVPacket *pkt = av_packet_alloc();
    int ret = av_read_frame(m_ic,pkt);
    if(ret != 0)
    {
        mux.unlock();
        av_packet_free(&pkt);
        return nullptr;
    }

    pkt->pts = pkt->pts*(1000*(r2d(m_ic->streams[pkt->stream_index]->time_base)));
    pkt->dts = pkt->dts*(1000*(r2d(m_ic->streams[pkt->stream_index]->time_base)));

    mux.unlock();
    return pkt;
}