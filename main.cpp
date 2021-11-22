#include <QApplication>
#include <iostream>
#include <QThread>
#include <QAudioOutput>
#include "MainWindow.h"
#include "XDemux.h"


int main(int argc, char *argv[])
{
    XDemux xx;
    bool aaa = xx.open("./test.mp4");
    std::cout << "XDemux open:" <<aaa << std::endl;

    while(true)
    {
        AVPacket *packet = xx.read();
        if(!packet) break;
    }

    QApplication a(argc, argv);
//    MainWindow mm;
//    mm.show();
    return QApplication::exec();
}
