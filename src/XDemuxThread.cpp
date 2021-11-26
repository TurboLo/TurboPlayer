
#include <iostream>
#include "XDemuxThread.h"

XDemuxThread::XDemuxThread()
{

}

XDemuxThread::~XDemuxThread()
{
    isExit = true;
    wait();
}

bool XDemuxThread::open(const char *url, IVideoCall *call)
{
    if(url == 0 || url[0] == '\0')
    {
        return false;
    }
    mux.lock();
    if(!m_demux) m_demux = new XDemux();
    if(!m_vt) m_vt = new XVideoThread();
    if(!m_at) m_at = new XAudioThread();

    bool re = m_demux->open(url);
    if(!re)
    {
        std::cout << "demux->open failed" <<std::endl;
        return false;
    }
    if(!m_vt->open(m_demux->copyVPara(),call,m_demux->width,m_demux->height))
    {
        std::cout << "m_vt->open failed" <<std::endl;
        re = false;
    }
    if(!m_at->open(m_demux->copyAPara()))
    {
        std::cout << "m_at->open failed" <<std::endl;
        re = false;
    }
    mux.unlock();
    std::cout << "open :" << re <<std::endl;
    return re;
}

void XDemuxThread::start()
{
    mux.lock();
    QThread::start();
    if(m_vt) m_vt->start();
    if(m_at) m_at->start();
    mux.unlock();
}

void XDemuxThread::run()
{
    while(!isExit)
    {
        mux.lock();
        if(!m_demux)
        {
            mux.unlock();
            msleep(5);
            continue;
        }
        AVPacket *pkt = m_demux->read();
        if(!pkt)
        {
            mux.unlock();
            msleep(5);
            continue;
        }

        if(m_demux->isAudio(pkt))
        {
            if(m_at)
            {
                m_at->push(pkt);
            }
        }
        else
        {
            if(m_vt)
            {
                m_vt->push(pkt);
            }
        }
        mux.unlock();
    }
}
