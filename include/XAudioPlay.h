/************************************************************
 *
 *          Project Name : XAudioPlay
 *
 *              FileName : FFMPEGTEST_XAUDIOPLAY_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/26 14:40
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_XAUDIOPLAY_H
#define FFMPEGTEST_XAUDIOPLAY_H

class XAudioPlay
{
public:
    int m_sampleRate = 44100;
    int m_sampleSize = 8;
    int m_channels = 2;
    static XAudioPlay &instance();
    // 打开音频播放
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool write(const unsigned char *data , int dataSize) = 0;
    virtual int getFree() = 0;
    virtual long long getNoPlayMs() = 0;
    virtual ~XAudioPlay();
protected:
    XAudioPlay();
private:

};


#endif //FFMPEGTEST_XAUDIOPLAY_H
