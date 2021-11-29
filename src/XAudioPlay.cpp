
#include "XAudioPlay.h"
#include <QAudioFormat>
#include <QAudioOutput>
#include <mutex>
class CXAudiePlay : public XAudioPlay
{
public:
    QAudioOutput *m_output;
    QIODevice *m_io;
    std::mutex mux;
    virtual bool open()
    {
        close();
        QAudioFormat fmt;
        fmt.setSampleRate(m_sampleRate);
        fmt.setSampleSize(m_sampleSize);
        fmt.setChannelCount(m_channels);
        fmt.setCodec("audio/pcm");
        fmt.setByteOrder(QAudioFormat::LittleEndian);
        fmt.setSampleType(QAudioFormat::UnSignedInt);
        mux.lock();
        m_output = new QAudioOutput(fmt);
        m_io = m_output->start();
        mux.unlock();
        if(!m_io)
        {
            return false;
        }
        return true;
    }
    virtual void close()
    {
        mux.lock();
        if(m_io)
        {
            m_io->close();
            m_io = nullptr;
        }
        if(m_output)
        {
            m_output->stop();
            delete m_output;
            m_output = nullptr;
        }
        mux.unlock();
    }
    virtual int getFree()
    {
        mux.lock();
        if(!m_output)
        {
            mux.unlock();
            return 0;
        }
        int free = m_output->bytesFree();
        mux.unlock();
        return free;
    }
    virtual bool write(const unsigned char *data , int dataSize)
    {
        if(!data || dataSize<=0) return false;
        mux.lock();
        if(!m_output || !m_io)
        {
            mux.unlock();
            return false;
        }
        m_io->write((char *)data,dataSize);
        mux.unlock();
        return true;
    }
    virtual long long getNoPlayMs()
    {
        mux.lock();
        if(!m_output)
        {
            mux.unlock();
            return 0;
        }
        long long pts = 0;
        double size = m_output->bufferSize() - m_output->bytesFree();
        double secSize = m_sampleRate*(m_sampleSize/8)*m_channels;
        if(secSize <= 0)
        {
            pts = 0;
        }
        else
        {
            pts = (size/secSize) *1000;
        }
        mux.unlock();
        return pts;
    }
    virtual void setPause(bool pause)
    {
        mux.lock();
        if(!m_output)
        {
            mux.unlock();
            return;
        }
        if(pause)
        {
            m_output->suspend();
        }
        else
        {
            m_output->resume();
        }
        mux.unlock();
    }
};



XAudioPlay &XAudioPlay::instance()
{
    static CXAudiePlay instance;
    return instance;
}

XAudioPlay::~XAudioPlay()
{

}

XAudioPlay::XAudioPlay()
{

}
