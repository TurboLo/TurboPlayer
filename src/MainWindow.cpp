#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
// http://39.134.65.162/PLTV/88888888/224/3221225611/index.m3u8
//xx.open("./test.mp4",mm.ui.openGLWidget);
/*xx.open("http://39.134.65.162/PLTV/88888888/224/3221225611/index.m3u8",mm.ui.openGLWidget);
xx.start();*/

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
    ui.setupUi(this);
    m_xdt = new XDemuxThread();
    m_xdt->start();
    connect(ui.actionOpen,&QAction::triggered,this,&MainWindow::openFile);
    connect(ui.actionPause,&QAction::triggered,this,&MainWindow::playOrPause);
    connect(ui.videoProgressSlider,&QSlider::sliderPressed,this,&MainWindow::sliderPressed);
    connect(ui.videoProgressSlider,&QSlider::sliderReleased,this,&MainWindow::sliderReleased);
    startTimer(40);
}

MainWindow::~MainWindow()
{
    m_xdt->close();
    delete m_xdt;
    m_xdt= nullptr;
}

void MainWindow::sliderPressed()
{
    m_isSliderPress = true;
}

void MainWindow::sliderReleased()
{
    m_isSliderPress = false;
    double pos = 0.0;
    pos = (double)ui.videoProgressSlider->value() /(double)ui.videoProgressSlider->maximum();
    m_xdt->seek(pos);
}

void MainWindow::init(int w, int h)
{
    ui.openGLWidget->init(w,h);
}

void MainWindow::repaint(AVFrame *frame)
{
    ui.openGLWidget->repaint(frame);
}

void MainWindow::playOrPause()
{
    if(m_xdt->m_isPause)
    {
        m_xdt->m_isPause = false;
    }
    else
    {
        m_xdt->m_isPause = true;
    }
}

void MainWindow::openFile()
{
    QString name = QFileDialog::getOpenFileName(this,tr("打开文件"),"./");
    if(name.isEmpty())
    {
        return;
    }
    setWindowTitle(name);
    if(!m_xdt->open(name.toLocal8Bit(),ui.openGLWidget))
    {
        QMessageBox::warning(this,"error","open failed");
        return;
    }

}

void MainWindow::timerEvent(QTimerEvent *e)
{
    if(m_isSliderPress) return;
    long long total = m_xdt->totalMs;
    if(total>0)
    {
        double pos = (double)m_xdt->m_pts/(double) total;
        int value = ui.videoProgressSlider->maximum() * pos;
        ui.videoProgressSlider->setValue(value);
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(isFullScreen())
    {
        showNormal();
    }
    else
    {
        showFullScreen();
    }
}

