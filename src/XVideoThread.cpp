
#include <iostream>
#include "XVideoThread.h"

XVideoThread::XVideoThread()
{

}

XVideoThread::~XVideoThread()
{

}

bool XVideoThread::open(AVCodecParameters *para,IVideoCall *call,int width ,int height)
{
    if(!para) return false;
    clear();
    vMux.lock();
    m_call = call;
    if(m_call)
    {
        m_call->init(width,height);
    }
    synPts = 0;
    vMux.unlock();
    bool re = true;
    if(!m_decode->open(para))
    {
        re = false;
    }
    return re;
}

void XVideoThread::run()
{
    while(!isExit)
    {
        vMux.lock();
        if(m_isPause)
        {
            vMux.unlock();
            msleep(5);
            continue;
        }
        if(synPts > 0 && synPts < m_decode->pts)
        {
            vMux.unlock();
            msleep(1);
            continue;
        }
        AVPacket *pkt = pop();
        int re = m_decode->send(pkt);
        if(!re)
        {
            vMux.unlock();
            msleep(1);
            continue;
        }
        while(!isExit)
        {
            AVFrame *frame = m_decode->receive();
            if(!frame) break;
            if(m_call)
            {
                m_call->repaint(frame);
            }
        }
        vMux.unlock();
    }
}

void XVideoThread::setPause(bool pause)
{
    vMux.lock();
    m_isPause = pause;
    vMux.unlock();
}

bool XVideoThread::repaintPts(AVPacket *pkt, long long int seekPts)
{
    vMux.lock();
    bool re = m_decode->send(pkt);
    if(!re)
    {
        vMux.unlock();
        return true;
    }
    AVFrame *frame = m_decode->receive();
    if(!frame)
    {
        vMux.unlock();
        return false;
    }
    // 到达位置
    if(m_decode->pts >= seekPts)
    {
        if(m_call)
        {
            m_call->repaint(frame);
        }
        vMux.unlock();
        return true;
    }
    av_frame_free(&frame);
    vMux.unlock();
    return false;
}
