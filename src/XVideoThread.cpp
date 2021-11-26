
#include <iostream>
#include "XVideoThread.h"

XVideoThread::XVideoThread()
{

}

XVideoThread::~XVideoThread()
{
    isExit = true;
    wait();
}

bool XVideoThread::open(AVCodecParameters *para,IVideoCall *call,int width ,int height)
{
    if(!para) return false;
    m_call = call;
    m_call->init(width,height);
    mux.lock();
    if(!m_decode)
    {
        m_decode = new XDecode();
    }
    bool re = true;
    if(!m_decode->open(para))
    {
        re = false;
    }
    std::cout << "xvideo open success" << std::endl;
    mux.unlock();
    return re;
}

void XVideoThread::push(AVPacket *pkt)
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

void XVideoThread::run()
{
    while(!isExit)
    {
        mux.lock();
        if(packet.empty() || !m_decode )
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
            if(m_call)
            {
                m_call->repaint(frame);
            }
        }
        mux.unlock();
    }
}
