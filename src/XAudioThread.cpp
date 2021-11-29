
#include <iostream>
#include "XAudioThread.h"

XAudioThread::XAudioThread()
{
    m_resample = new XResample();
}

XAudioThread::~XAudioThread()
{

}

bool XAudioThread::open(AVCodecParameters *para)
{
    if(!para) return false;
    clear();
    aMux.lock();
    pts = 0;
    bool re = true;
    if(!m_resample->open(para,false))
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
    aMux.unlock();
    return re;
}

void XAudioThread::run()
{
    auto * pcm = new unsigned char[1024*1024];
    while(!isExit)
    {
        aMux.lock();
        if(m_isPause)
        {
            aMux.unlock();
            msleep(5);
            continue;
        }
        AVPacket *pkt = pop();
        int re = m_decode->send(pkt);
        if(!re)
        {
            aMux.unlock();
            msleep(1);
            continue;
        }
        while(!isExit)
        {
            AVFrame *frame = m_decode->receive();
            if(!frame) break;
            pts = m_decode->pts - XAudioPlay::instance().getNoPlayMs();
            int size = m_resample->resample(frame,pcm);
            while(!isExit)
            {
                if(size <= 0)
                {
                    break;
                }
                if(XAudioPlay::instance().getFree() < size || m_isPause)
                {
                    msleep(1);
                    continue;
                }
                XAudioPlay::instance().write(pcm,size);
                break;
            }
        }

        aMux.unlock();
    }
    delete pcm;
}

void XAudioThread::close()
{
    XDecodeThread::close();
    if(m_resample)
    {
        m_resample->close();
        aMux.lock();
        delete m_resample;
        m_resample = nullptr;
        aMux.unlock();
    }
}

void XAudioThread::setPause(bool pause)
{
    m_isPause = pause;
    XAudioPlay::instance().setPause(pause);
}
