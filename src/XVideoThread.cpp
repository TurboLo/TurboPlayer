
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
