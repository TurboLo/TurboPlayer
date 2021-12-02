
#include <iostream>
#include "XDemuxThread.h"

XDemuxThread::XDemuxThread()
{

}

XDemuxThread::~XDemuxThread()
{

}

bool XDemuxThread::open(const char *url, IVideoCall *call)
{
    if(url == 0 || url[0] == '\0')
    {
        return false;
    }
    mux.lock();
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
    totalMs = m_demux->totalMs;
    mux.unlock();
    std::cout << "open :" << re <<std::endl;
    return re;
}

void XDemuxThread::start()
{
    mux.lock();
    if(!m_demux) m_demux = new XDemux();
    if(!m_vt) m_vt = new XVideoThread();
    if(!m_at) m_at = new XAudioThread();
    QThread::start();
    if(m_vt) m_vt->start();
    if(m_at) m_at->start();
    mux.unlock();
}

void XDemuxThread::run()
{
    while(!isExit)
    {
        if(m_isPause)
        {
            msleep(5);
            continue;
        }
        mux.lock();
        if(!m_demux)
        {
            mux.unlock();
            msleep(5);
            continue;
        }

        if(m_vt && m_at)
        {
            m_pts = m_at->pts;
            m_vt->synPts = m_at->pts;
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
        msleep(1);
    }
}

void XDemuxThread::close()
{
    isExit = true;
    wait();
    if(m_vt) m_vt->close();
    if(m_at) m_at->close();
    mux.lock();
    delete m_vt;
    delete m_at;
    m_vt = nullptr;
    m_at = nullptr;
    mux.unlock();
}

void XDemuxThread::setPause(bool pause)
{
    mux.lock();
    m_isPause = pause;
    if(m_at) m_at->setPause(pause);
    if(m_vt) m_vt->setPause(pause);
    mux.unlock();
}

void XDemuxThread::seek(double pos)
{
    clear();
    // 暂停
    mux.lock();
    bool status = m_isPause;
    mux.unlock();
    setPause(true);
    mux.lock();
    if(m_demux)
    {
        m_demux->seek(pos);
    }
    long long seekPos = pos*m_demux->totalMs;
    while(!isExit)
    {
        AVPacket *pkt = m_demux->readVideo();
        if(!pkt) break;
        if(m_vt->repaintPts(pkt,seekPos))
        {
            m_pts = seekPos;
            break;
        }
    }
    mux.unlock();
    setPause(status);
}

void XDemuxThread::clear()
{
    mux.lock();
    if(m_demux)
    {
        m_demux->clear();
    }
    if(m_vt) m_vt->clear();
    if(m_at) m_at->clear();
    mux.unlock();
}
