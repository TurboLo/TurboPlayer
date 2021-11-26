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
    xx.open("./test.mp4",mm.ui.openGLWidget);
    xx.start();
    return QApplication::exec();
}
