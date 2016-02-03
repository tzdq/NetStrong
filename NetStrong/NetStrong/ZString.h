
/*-======================- Copyright (c) 2006 -======================-

程序作者:
    张鲁夺(zhangluduo) : 为所有爱我的人和我爱的人努力!

联系方式:
    zhangluduo@msn.com
	[oopfans]群:34064264

修改时间:
    2007-02-03

功能描述:
    字符串操作

版权声明:
    许可任何单位,个人随意使用,拷贝,修改,散布及出售这份代码,但是必须保
    留此版权信息,以慰藉作者辛勤的劳动,及表明此代码的来源,如若此份代码
    有任何BUG,请通知作者,以便弥补作者由于水平所限而导致的一些错误和不
    足,谢谢!

-======================- Copyright (c) 2006 -======================-*/

#ifndef _ZSTRING
#define _ZSTRING

#pragma warning(disable:4786) 
#include <string>
#include <deque>
#include <functional>
#include <algorithm>
using namespace std;

class ZLD_API  ZString  
{
public:
	ZString();
	~ZString();

	//根据拆分标记对源串进行拆分
	deque<string> Splitter(string strSource,const string strFlag);

	//去除字符串中所有的空格
	string EraseSpace(const string str);

	//去除前面和后面的空格
	string TrimAll(const string str);

	//去掉前面空格
	string TrimLeft(const string str);

	//去掉后面空格
	string TrimRight(const string str);
};

#endif
