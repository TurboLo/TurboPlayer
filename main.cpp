#include <QApplication>
#include "MainWindow.h"
#include "XDemuxThread.h"

int main(int argc, char *argv[])
{
    // 初始化显示
    QApplication a(argc, argv);
    MainWindow mm;
    mm.show();
    XDemuxThread xx;
    // http://39.134.65.162/PLTV/88888888/224/3221225611/index.m3u8
    xx.open("",mm.ui.openGLWidget);
    xx.start();
    return QApplication::exec();
}
