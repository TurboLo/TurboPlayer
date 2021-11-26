
#include <iostream>
#include "XAudioThread.h"

XAudioThread::XAudioThread()
{

}

XAudioThread::~XAudioThread()
{
    isExit = true;
    wait();
}

bool XAudioThread::open(AVCodecParameters *para)
{
    if(!para) return false;
    mux.lock();
    if(!m_decode)
    {
        m_decode = new XDecode();
    }
    if(!m_resample)
    {
        m_resample = new XResample();
    }
    bool re = true;
    if(m_resample->open(para,false))
    {
        re = false;
    }
    XAudioPlay::instance().m_sampleRate = para->sample_rate;
    XAudioPlay::instance().m_sampleSize = para->format * para->channels;
    XAudioPlay::instance().m_channels = para->channels;
    if(!XAudioPlay::instance().open())
    {
        re = false;
        std::cout << "xaudio open failed" << std::endl;
    }
    if(!m_decode->open(para))
    {

    }
    std::cout << "xaudio open success" << std::endl;
    mux.unlock();
    return re;
}

void XAudioThread::run()
{
    unsigned char * pcm = new unsigned char[1024*1024];
    while(!isExit)
    {
        mux.lock();
        if(packet.empty() || !m_decode || !m_resample)
        {
            mux.unlock();
            msleep(1);
            continue;
        }
        AVPacket *pkt = packet.front();
        packet.pop_front();

        int re = m_decode->send(pkt);
        if(!re)
        {
            mux.unlock();
            msleep(1);
            continue;
        }
        while(!isExit)
        {
            AVFrame *frame = m_decode->receive();
            if(!frame) break;
            int size = m_resample->resample(frame,pcm);
            while(!isExit)
            {
                if(size <= 0)
                {
                    break;
                }
                if(XAudioPlay::instance().getFree() < size)
                {
                    msleep(1);
                    continue;
                }
                XAudioPlay::instance().write(pcm,size);
                break;
            }
        }

        mux.unlock();
    }
    delete pcm;
}

void XAudioThread::push(AVPacket *pkt)
{
    if(!pkt) return;

    while(!isExit)
    {
        mux.lock();
        if(packet.size() < maxList)
        {

            packet.push_back(pkt);
            mux.unlock();
            break;
        }
        mux.unlock();
        msleep(1);
    }
}
