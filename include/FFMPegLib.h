
#ifndef FFMPEGTEST_FFMPEGLIB_H
#define FFMPEGTEST_FFMPEGLIB_H
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg4.2.1/lib/win64/avformat.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg4.2.1/lib/win64/avutil.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg4.2.1/lib/win64/avcodec.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg4.2.1/lib/win64/swscale.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg4.2.1/lib/win64/swresample.lib")

#endif //FFMPEGTEST_FFMPEGLIB_H
