
#ifndef FFMPEGTEST_MAINWINDOW_H
#define FFMPEGTEST_MAINWINDOW_H

#include <QMainWindow>
#include <QTimerEvent>
#include "ui_MainWindow.h"
#include "XDemuxThread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;
    void init(int w,int h);
    void repaint(AVFrame *frame);

protected:
    void timerEvent(QTimerEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void sliderPressed();
    void sliderReleased();

private slots:
    void openFile();
    void playOrPause();

private:
    Ui::MainWindow ui;
    XDemuxThread *m_xdt{nullptr};
    bool m_isSliderPress{false};

};


#endif //FFMPEGTEST_MAINWINDOW_H
