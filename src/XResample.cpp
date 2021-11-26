
#include <iostream>
#include "XResample.h"

XResample::~XResample()
{

}

XResample::XResample()
{

}

bool XResample::open(AVCodecParameters *para,bool isClear)
{
    mux.lock();
    m_actx = swr_alloc();
    m_actx = swr_alloc_set_opts(m_actx
            , av_get_default_channel_layout(2)
            ,(AVSampleFormat)outFormat
            ,para->sample_rate
            , av_get_default_channel_layout(para->channels)
            ,(AVSampleFormat)para->format
            ,para->sample_rate
            ,0,0);
    if(isClear)
    {
        avcodec_parameters_free(&para);
    }
    int re = swr_init(m_actx);
    mux.unlock();
    if(re!=0)
    {
        char buf[1024] ={0};
        av_strerror(re,buf,sizeof(buf)-1);
        std::cout << "swr_init failed" << buf << std::endl;
        return false;
    }
    return true;
}

void XResample::close()
{
    mux.lock();
    if (m_actx)
        swr_free(&m_actx);
    mux.unlock();
}

int XResample::resample(AVFrame *inDate, unsigned char *d)
{
    if(!inDate) return 0;
    if(!d)
    {
        av_frame_free(&inDate);
        return 0;
    }
    uint8_t *data[2] = {0};
    data[0] = d;
    int re = swr_convert(m_actx
            ,data
            ,inDate->nb_samples
            ,(const uint8_t **)inDate->data
            ,inDate->nb_samples);

    std::cout << "swr_convert = " <<re << std::endl;
    if(re <=0 ) return re;
    int outSide =  re * inDate->channels * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    return outSide;
}
