#include <QApplication>
#include <iostream>
#include "MainWindow.h"
extern "C"
{
#include "libavformat/avformat.h"
}
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg3.4.2/lib/win64/avformat.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg3.4.2/lib/win64/avutil.lib")
int main(int argc, char *argv[])
{
    const char * path = "D:/BaiduNetdiskDownload/SQL Server 2014全套/1.SQL Server 2014简介.mp4";
    // 初始化封装库
    av_register_all();
    // 初始化网络库
    avformat_network_init();
    AVDictionary *opts = nullptr;
    // 设置rtsp参数
    av_dict_set(&opts,"rtsp_transport","tcp",0);
    // 网络延时时间
    av_dict_set(&opts,"max_delay","500",0);
    AVFormatContext *ic = nullptr;
    int ret = avformat_open_input(&ic,path,0,&opts);
    if(ret!=0)
    {
        char buf[1024]={0};
        av_strerror(ret,buf,sizeof(buf)-1);
        std::cout<< buf <<std::endl;
        getchar();
        return -1;
    }
    std::cout<< "ok" <<std::endl;
    ret = avformat_find_stream_info(ic,0);
    // 获取视频流总时长
    int total = ic->duration /(AV_TIME_BASE /1000);

    std::cout<< total << "ms" <<std::endl;
    // 打印视频流信息
    av_dump_format(ic,0,"",0);

    if(ic)
    {
        avformat_close_input(&ic);
        ic= nullptr;
    }
    getchar();
    return 0;


    /*QApplication a(argc, argv);
    MainWindow mm;
    mm.show();
    return QApplication::exec();*/
}
