
#include <iostream>
#include <QTimer>
#include "XVideoWidget.h"
#define  GET_STR(x) #x
#define  A_VER 3
#define  T_VER 4
FILE *fp = 0;
//顶点shader
const char *vString = GET_STR(
        attribute vec4 vertexIn;
        attribute vec2 textureIn;
        varying vec2 textureOut;
        void main(void)
        {
            gl_Position = vertexIn;
            textureOut = textureIn;
        }
);

//片源shader
const char *tString = GET_STR(
        varying vec2 textureOut;
        uniform sampler2D tex_y;
        uniform sampler2D tex_u;
        uniform sampler2D tex_v;
        void main(void)
        {
            vec3 yuv;
            vec3 rgb;
            yuv.x = texture2D(tex_y , textureOut).r;
            yuv.y = texture2D(tex_u , textureOut).r - 0.5;
            yuv.z = texture2D(tex_v , textureOut).r - 0.5;
            rgb = mat3(1.0,1.0,1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0)*yuv;
            gl_FragColor = vec4(rgb , 1.0);
        }
);

// 准备yuv数据
// ffmpeg -i file  -t 10 -s 240*128 -pix_fmt yuv420p out240X128.yuv
void XVideoWidget::paintGL()
{
    /*if(feof(fp))
    {
        fseek(fp,0,SEEK_SET);
    }
    fread(data[0],1,width*height,fp);
    fread(data[1],1,width*height/4,fp);
    fread(data[2],1,width*height/4,fp);*/
    mux.lock();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texs[0]); // 绑定到材质
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_RED,GL_UNSIGNED_BYTE,data[0]);
    //与shader变量关联
    glUniform1i(unis[0],0);

    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D,texs[1]); // 绑定到材质
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width/2,height/2,GL_RED,GL_UNSIGNED_BYTE,data[1]);
    //与shader变量关联
    glUniform1i(unis[1],1);

    glActiveTexture(GL_TEXTURE0+2);
    glBindTexture(GL_TEXTURE_2D,texs[2]); // 绑定到材质
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width/2,height/2,GL_RED,GL_UNSIGNED_BYTE,data[2]);
    //与shader变量关联
    glUniform1i(unis[2],2);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    mux.unlock();
}

void XVideoWidget::initializeGL()
{
    mux.lock();
    // 初始化opengl函数
    initializeOpenGLFunctions();

    // program版本
    program.addShaderFromSourceCode(QGLShader::Fragment,tString);
    program.addShaderFromSourceCode(QGLShader::Vertex,vString);

    // 设置顶点坐标变量
    program.bindAttributeLocation("vertexIn",A_VER);

    // 设置材质坐标
    program.bindAttributeLocation("textureIn",T_VER);

    program.link();
    program.bind();

    static const GLfloat ver[] =
            {
                    -1.0f, -1.0f,
                    1.0f, -1.0f,
                    -1.0f, 1.0f,
                    1.0f, 1.0f
            };
    static const GLfloat tex[] =
            {
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    0.0f, 0.0f,
                    1.0f, 0.0f
            };
    // 顶点
    glVertexAttribPointer(A_VER,2,GL_FLOAT,0,0,ver);
    glEnableVertexAttribArray(A_VER);
    // 材质
    glVertexAttribPointer(T_VER,2,GL_FLOAT,0,0,tex);
    glEnableVertexAttribArray(T_VER);

    // 从shader中获取材质
    unis[0] = program.uniformLocation("tex_y");
    unis[1] = program.uniformLocation("tex_u");
    unis[2] = program.uniformLocation("tex_v");

    mux.unlock();
    /*fp = fopen("./out240X128.yuv","rb");
    if(!fp)
    {
        std::cout << "open file failed!" << std::endl;
    }
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(update()));
    timer->start(40);*/
}

void XVideoWidget::resizeGL(int w, int h)
{
    mux.lock();

    mux.unlock();
}

XVideoWidget::XVideoWidget(QWidget *parent)
: QOpenGLWidget(parent)
{

}

XVideoWidget::~XVideoWidget()
{

}

void XVideoWidget::init(int width, int height)
{
    mux.lock();
    this->width = width;
    this->height = height;
    delete data[0];
    delete data[1];
    delete data[2];
    // 分配材质内存空间
    data[0] = new unsigned char[width * height];
    data[1] = new unsigned char[width * height/4];
    data[2] = new unsigned char[width * height/4];

    if(texs[0])
    {
        glDeleteTextures(3,texs);
    }
    // 创建材质
    glGenTextures(3,texs);
    // Y
    glBindTexture(GL_TEXTURE_2D ,texs[0]);
    // 放大过滤，线性插值
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //创建材质显卡空间
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width,height,0,GL_RED,GL_UNSIGNED_BYTE,0);
    // u
    glBindTexture(GL_TEXTURE_2D ,texs[1]);
    // 放大过滤，线性插值
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //创建材质显卡空间
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width/2,height/2,0,GL_RED,GL_UNSIGNED_BYTE,0);
    // v
    glBindTexture(GL_TEXTURE_2D ,texs[2]);
    // 放大过滤，线性插值
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //创建材质显卡空间
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width/2,height/2,0,GL_RED,GL_UNSIGNED_BYTE,0);

    mux.unlock();
}

void XVideoWidget::repaint(AVFrame *frame)
{
    if(!frame) return;
    // 行对齐问题
    mux.lock();
    // 容错，保证尺寸正确
    if(width * height ==0 || frame->width != width || frame->height != height)
    {
        av_frame_free(&frame);
        mux.unlock();
        return;
    }
    if(width = frame->linesize[0])
    {
        memcpy(data[0],frame->data[0],width*height);
        memcpy(data[1],frame->data[1],width*height/4);
        memcpy(data[2],frame->data[2],width*height/4);
    }
    else
    {
        for(int i = 0;i<height;i++)
        {
            memcpy(data[0]+width*i,frame->data[0]+frame->linesize[0]*i,width);
        }for(int i = 0;i<height/2;i++)
        {
            memcpy(data[1]+width/2*i,frame->data[1]+frame->linesize[0]*i,width);
        }for(int i = 0;i<height/2;i++)
        {
            memcpy(data[2]+width/2*i,frame->data[2]+frame->linesize[0]*i,width);
        }
    }
    mux.unlock();
    av_frame_free(&frame);
    update();
}
