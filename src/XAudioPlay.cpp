
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
