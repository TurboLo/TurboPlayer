#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
    ui.setupUi(this);
}

MainWindow::~MainWindow()
{

}

void MainWindow::init(int w, int h)
{
    ui.openGLWidget->init(w,h);
}

void MainWindow::repaint(AVFrame *frame)
{
    ui.openGLWidget->repaint(frame);
}

