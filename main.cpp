#include <QApplication>
#include <iostream>
#include <QThread>
#include <QAudioOutput>
#include "MainWindow.h"
#include "XDemux.h"
#include "XDecode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mm;
    XDemux xx;
    bool aaa = xx.open("./test.mp4");
    std::cout << "XDemux open:" <<aaa << std::endl;
    std::cout << "XDemux open:" <<xx.copyAPara() << std::endl;
    std::cout << "XDemux open:" <<xx.copyVPara() << std::endl;
    std::cout << "XDemux open:" <<xx.seek(0.5) << std::endl;

    XDecode vdd;
    std::cout << "XDecode open:" << vdd.open(xx.copyVPara()) << std::endl;
    XDecode add;
    std::cout << "XDecode open:" << add.open(xx.copyAPara()) << std::endl;
    // 初始化显示

    /*while(true)
    {
        AVPacket *pkt = xx.read();
        if(xx.isAudio(pkt))
        {
            add.send(pkt);
            AVFrame *frame = add.receive();
        }
        else
        {
            vdd.send(pkt);
            AVFrame *frame = vdd.receive();
        }
        if(!pkt) break;
    }
*/

    mm.init(xx.width,xx.height);

    mm.show();
    return QApplication::exec();
}
