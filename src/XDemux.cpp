
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
    close();
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
        //std::cout << "open " << url << " failed! :" << buf << std::endl;
        return false;
    }
    //std::cout << "open " << url << " success! " << std::endl;

    //获取流信息
    re = avformat_find_stream_info(m_ic, 0);

    //总时长 毫秒
    totalMs = m_ic->duration / (AV_TIME_BASE / 1000);
    //std::cout << "totalMs = " << totalMs << std::endl;

    //打印视频流详细信息
    av_dump_format(m_ic, 0, url, 0);

    //获取视频流
    videoStream = av_find_best_stream(m_ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    AVStream *as = m_ic->streams[videoStream];
    width = as->codecpar->width;
    height = as->codecpar->height;

    /*std::cout << "=======================================================" << std::endl;
    std::cout << videoStream << "视频信息" << std::endl;
    std::cout << "codec_id = " << as->codecpar->codec_id << std::endl;
    std::cout << "format = " << as->codecpar->format << std::endl;
    std::cout << "width=" << as->codecpar->width << std::endl;
    std::cout << "height=" << as->codecpar->height << std::endl;*/
    //帧率 fps 分数转换
    /*std::cout << "video fps = " << r2d(as->avg_frame_rate) << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << audioStream << "音频信息" << std::endl;*/
    //获取音频流
    audioStream = av_find_best_stream(m_ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    as = m_ic->streams[audioStream];
    m_channels = as->codecpar->channels;
    m_sampleRate = as->codecpar->sample_rate;
    m_sampleSize = as->codecpar->format * as->codecpar->channels;
    /*std::cout << "=======================================================" << std::endl;
    std::cout << audioStream << "音频信息" << std::endl;
    std::cout << "codec_id = " << as->codecpar->codec_id << std::endl;
    std::cout << "format = " << as->codecpar->format << std::endl;
    std::cout << "m_sampleRate=" << as->codecpar->sample_rate << std::endl;
    std::cout << "channels=" << as->codecpar->channels << std::endl;*/
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

AVCodecParameters *XDemux::copyVPara()
{
    mux.lock();
    if(!m_ic)
    {
        mux.unlock();
        return nullptr;
    }
    AVCodecParameters *pa = avcodec_parameters_alloc();
    avcodec_parameters_copy(pa,m_ic->streams[videoStream]->codecpar);
    mux.unlock();
    return pa;
}

AVCodecParameters *XDemux::copyAPara()
{
    mux.lock();
    if(!m_ic)
    {
        mux.unlock();
        return nullptr;
    }
    AVCodecParameters *pa = avcodec_parameters_alloc();
    avcodec_parameters_copy(pa,m_ic->streams[audioStream]->codecpar);
    mux.unlock();
    return pa;
}

bool XDemux::seek(double pos)
{
    mux.lock();
    if(!m_ic)
    {
        mux.unlock();
        return false;
    }
    avformat_flush(m_ic);
    long long seekPos = 0;
    seekPos = m_ic->streams[videoStream]->duration*pos;

    int re = av_seek_frame(m_ic,videoStream,seekPos,AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME);
    mux.unlock();
    if(re < 0)
    {
        return false;
    }
    return true;
}

void XDemux::close()
{
    mux.lock();
    if(!m_ic)
    {
        mux.unlock();
        return;
    }
    avformat_close_input(&m_ic);
    totalMs = 0;
    mux.unlock();
}

void XDemux::clear()
{
    mux.lock();
    if(!m_ic)
    {
        mux.unlock();
        return;
    }
    avformat_flush(m_ic);
    mux.unlock();
}

bool XDemux::isAudio(AVPacket *pkt)
{
    if(!pkt || pkt->size<=0 ||!pkt->data) return false;
    if(pkt->stream_index == videoStream) return false;
    return true;
}

AVPacket *XDemux::readVideo()
{
    AVPacket *pkt = av_packet_alloc();
    for(int i = 0 ; i < 20 ;i++)
    {
        pkt = read();
        if(pkt->stream_index == videoStream)
        {
            break;
        }
        av_packet_free(&pkt);
        continue;
    }
    return pkt;
}
