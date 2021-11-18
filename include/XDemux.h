/************************************************************
 *
 *          Project Name : XDemux
 *
 *              FileName : FFMPEGTEST_XDEMUX_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/18 17:37
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_XDEMUX_H
#define FFMPEGTEST_XDEMUX_H

#include <string>
#include <mutex>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg3.4.2/lib/win64/avformat.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg3.4.2/lib/win64/avutil.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg3.4.2/lib/win64/avcodec.lib")

class XDemux
{
public:
    XDemux();
    virtual ~XDemux();

    virtual bool open(const std::string &url);
};


#endif //FFMPEGTEST_XDEMUX_H
