#include <QApplication>
#include <iostream>
#include "MainWindow.h"
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg3.4.2/lib/win64/avformat.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg3.4.2/lib/win64/avutil.lib")
#pragma comment(lib,"E:/FFMPEGTest/TurboPlayer/thirdParty/ffmpeg3.4.2/lib/win64/avcodec.lib")
int main(int argc, char *argv[])
{
    const char * path = "D:/BaiduNetdiskDownload/SQL Server 2014全套/1.SQL Server 2014简介.mp4";
    // 初始化封装库
    av_register_all();
    // 初始化网络库
    avformat_network_init();

    // 注册解码器
    avcodec_register_all();

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

    for(int i =0;i<ic->nb_streams;i++)
    {
        AVStream  *as = ic->streams[i];
        if(as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            std::cout<< i << "声音" <<std::endl;
            std::cout<< "sample_rate=" << as->codecpar->sample_rate <<std::endl;
            std::cout<< "format=" << as->codecpar->format <<std::endl;
            std::cout<< "channels=" << as->codecpar->channels <<std::endl;
            std::cout<< "codec_id=" << as->codecpar->codec_id <<std::endl;
            std::cout<< "frame_size=" << as->codecpar->frame_size <<std::endl;
        }
        else if(as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            std::cout<< i << "视频" <<std::endl;
            std::cout<< "width=" << as->codecpar->width <<std::endl;
            std::cout<< "height=" << as->codecpar->height <<std::endl;
            std::cout<< "num = " << as->avg_frame_rate.num << "  dem=" << as->avg_frame_rate.den <<std::endl;
            std::cout<< "height" << as->codecpar->height <<std::endl;
            std::cout<< "height" << as->codecpar->height <<std::endl;
        }
    }

    //获取视频流
    av_find_best_stream(ic,AVMEDIA_TYPE_VIDEO,-1,-1, nullptr,0);

    //找到解码器
    AVCodec *vCodec = avcodec_find_decoder(ic->streams[0]->codecpar->codec_id);

    // 创建爱解码器上下文
    AVCodecContext *vc = avcodec_alloc_context3(vCodec);

    // 配置解码器上下文
    avcodec_parameters_to_context(vc,ic->streams[0]->codecpar);

    //八线程解码器
    vc->thread_count = 8;

    ret = avcodec_open2(vc,0,0);

    if(ret!=0)
    {
        char buf[1024]={0};
        av_strerror(ret,buf,sizeof(buf)-1);
        std::cout<< buf <<std::endl;
        getchar();
        return -1;
    }

    // 音频解码器
    //找到解码器
    AVCodec *aCodec = avcodec_find_decoder(ic->streams[1]->codecpar->codec_id);

    // 创建爱解码器上下文
    AVCodecContext *ac = avcodec_alloc_context3(aCodec);

    // 配置解码器上下文
    avcodec_parameters_to_context(ac,ic->streams[1]->codecpar);

    //八线程解码器
    ac->thread_count = 8;

    ret = avcodec_open2(ac,0,0);

    if(ret!=0)
    {
        char buf[1024]={0};
        av_strerror(ret,buf,sizeof(buf)-1);
        std::cout<< buf <<std::endl;
        getchar();
        return -1;
    }


    AVPacket *pkt = av_packet_alloc();
    while(true)
    {
        int re = av_read_frame(ic,pkt);
        if(re!=0)
        {
            break;
        }
        std::cout<< "size=" << pkt->size <<std::endl;
        // 显示时间
        std::cout<< "pts=" << pkt->pts <<std::endl;
        // 解码时间
        std::cout<< "dts=" << pkt->dts <<std::endl;

        // 释放，引用计数为-1时候释放控件
        av_packet_unref(pkt);
        break;
    }
    av_packet_free(&pkt);
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
