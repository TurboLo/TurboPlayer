#include <QApplication>
#include "MainWindow.h"
#include "XDemuxThread.h"

int main(int argc, char *argv[])
{
    // 初始化显示
    QApplication a(argc, argv);
    MainWindow mm;
    mm.show();
    return QApplication::exec();
}
