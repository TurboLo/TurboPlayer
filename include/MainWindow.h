
#ifndef FFMPEGTEST_MAINWINDOW_H
#define FFMPEGTEST_MAINWINDOW_H

#include <QMainWindow>
#include "ui_MainWindow.h"

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
    Ui::MainWindow ui;
private:

};


#endif //FFMPEGTEST_MAINWINDOW_H
