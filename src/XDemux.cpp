
#include "XDemux.h"

XDemux::XDemux()
{
    static bool isFirst = true;
    static std::mutex mut;
    mut.lock();
    if(isFirst)
    {
        av_register_all();
        avformat_network_init();
        isFirst = false;
    }
    mut.unlock();
}

XDemux::~XDemux()
{

}

bool XDemux::open(const std::string &url)
{
    return true;
}