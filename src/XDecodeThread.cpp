
#include "XDecodeThread.h"

XDecodeThread::XDecodeThread()
{
    m_decode = new XDecode();
}

XDecodeThread::~XDecodeThread()
{
    isExit = true;
    wait();
}

void XDecodeThread::push(AVPacket *pkt)
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

AVPacket *XDecodeThread::pop()
{
    mux.lock();
    if(packet.empty())
    {
        mux.unlock();
        return nullptr;
    }
    AVPacket *pkt = packet.front();
    packet.pop_front();
    mux.unlock();
    return pkt;
}

void XDecodeThread::clear()
{
    mux.lock();
    m_decode->clear();
    while(!packet.empty())
    {
        AVPacket *pkt = packet.front();
        av_packet_free(&pkt);
        packet.pop_front();
    }
    mux.unlock();
}

void XDecodeThread::close()
{
    clear();
    isExit = true;
    wait();
    m_decode->close();
    mux.lock();
    delete m_decode;
    m_decode = nullptr;
    mux.unlock();
}
