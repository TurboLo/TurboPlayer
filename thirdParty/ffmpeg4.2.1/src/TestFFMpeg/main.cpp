/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               创建：夏曹俊，此代码可用作为学习参考                       **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : FFmpeg
** Description   : FFMPEG项目创建示例
** Contact       : xiacaojun@qq.com
**        博客   : http://blog.csdn.net/jiedichina
**		视频课程 
** 网易云课堂	http://study.163.com/u/xiacaojun
** 腾讯课堂		https://jiedi.ke.qq.com/
** csdn学院		http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961
** 51cto学院	https://edu.51cto.com/sd/07beb
** 下载最新的ffmpeg版本 http://www.ffmpeg.club
**
**   老夏课堂交流群 ：296249312 
**   老夏课堂网址：www.laoxiaketang.com
**   微信公众号  : jiedi2007
**		头条号	 : 夏曹俊
**
*******************************************************************************/
//！！！！！！！！！ 老夏课堂交流群 ：296249312 


#include <iostream>
extern "C" 
{
#include <libavcodec/avcodec.h>
}
#pragma comment(lib,"avcodec.lib")
using namespace std;
int main(int argc, char *argv[])
{
	cout << "Test FFmpeg.club " << endl;
#ifdef WIN32
	cout << "32 位程序" << endl;
#else
	cout << "64 位程序" << endl;
#endif
	cout << avcodec_configuration() << endl;
	getchar();
	return 0;
}