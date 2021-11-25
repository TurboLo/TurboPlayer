
#include "XDecode.h"
extern "C"
{
#include <libavcodec/avcodec.h>
}
#include <iostream>

XDecode::XDecode()
{

}

XDecode::~XDecode()
{

}

bool XDecode::open(AVCodecParameters *para)
{
    if(!para) return false;
    close();
    mux.lock();
    AVCodec *vCodec = avcodec_find_decoder(para->codec_id);
    if(!vCodec)
    {
        mux.unlock();
        avcodec_parameters_free(&para);
        std::cout << "cant find codec id" << para->codec_id << std::endl;
        return false;
    }
    std::cout << "find avcodec " << para->codec_id << std::endl;
    AVCodecContext *vc = avcodec_alloc_context3(vCodec);
    avcodec_parameters_to_context(vc,para);
    m_codec = avcodec_alloc_context3(vCodec);
    avcodec_parameters_free(&para);
    vc->thread_count=8;
    int re = avcodec_open2(vc,0,0);
    if(re!=0)
    {
        avcodec_free_context(&m_codec);
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf)-1);
        std::cout << "open error " << buf << std::endl;
        return false;
    }
    mux.unlock();
    std::cout << "open finished " << std::endl;
    return true;
}

void XDecode::close()
{
    mux.lock();
    if(m_codec)
    {
        avcodec_close(m_codec);
        avcodec_free_context(&m_codec);
    }
    mux.unlock();
}

void XDecode::clear()
{
    mux.lock();
    // 清理解码缓冲
    if(m_codec)
    {
        avcodec_flush_buffers(m_codec);
    }
    mux.unlock();
}

bool XDecode::send(AVPacket *pkt)
{
    if(!pkt || pkt->size<=0 ||!pkt->data) return false;
    mux.lock();
    if(!m_codec)
    {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(m_codec,pkt);
    mux.unlock();
    if(re!=0)
    {
        return false;
    }
    return true;
}

AVFrame *XDecode::receive()
{
    mux.lock();
    if(!m_codec)
    {
        mux.unlock();
        return nullptr;
    }

    AVFrame  *frame = av_frame_alloc();
    int re = avcodec_receive_frame(m_codec,frame);
    mux.unlock();
    if(re!=0)
    {
        av_frame_free(&frame);
        return nullptr;
    }
    std::cout << frame->linesize[0] << " " << std::flush;
    return frame;
}
