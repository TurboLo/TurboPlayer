/************************************************************
 *
 *          Project Name : IVideoCall
 *
 *              FileName : FFMPEGTEST_IVIDEOCALL_H
 *
 *            Programmer : 
 *
 *            Start Date : 2021/11/26 16:06
 *
 *           Last Update : 
 *
 ***********************************************************/

#ifndef FFMPEGTEST_IVIDEOCALL_H
#define FFMPEGTEST_IVIDEOCALL_H
struct AVFrame;
class IVideoCall
{
public:
    virtual void init(int width,int height) = 0;
    virtual void repaint(AVFrame * frame) = 0;
protected:

private:

};


#endif //FFMPEGTEST_IVIDEOCALL_H
