#include <QApplication>
#include "MainWindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mm;
    mm.show();
    return QApplication::exec();
}
