/************************************************************
 *
 *          Project Name : XVideoWidget
 *
 *              FileName : FFMPEGTEST_XVIDEOWIDGET_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/18 13:24
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_XVIDEOWIDGET_H
#define FFMPEGTEST_XVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>

class XVideoWidget :public QOpenGLWidget ,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit XVideoWidget(QWidget *parent = nullptr);
    ~XVideoWidget() override;
    void init(int width,int height);

protected:
    // 刷新显示
    void paintGL() override;

    // 初始化GL
    void initializeGL() override;

    // 窗口尺寸变化
    void resizeGL(int w, int h) override;

private:
    QGLShaderProgram program;
    GLuint unis[3] = {0};
    GLuint texs[3] = {0};

    unsigned char *data[3] = {0};

    int width = 240;
    int height = 128;

    std::mutex mux;
};


#endif //FFMPEGTEST_XVIDEOWIDGET_H
