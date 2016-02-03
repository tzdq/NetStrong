
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

#include "stdafx.h"
#include "zld.h"
#include "ZString.h"


ZString::ZString()
{

}

ZString::~ZString()
{

}

deque<string> ZString::Splitter(string strSource,const string strFlag)
{
	deque<string> dqResult;
	
	//参数检查
	if(strSource==strFlag)
	{
		dqResult.push_back(strSource);
		return dqResult;
	}
	else if(strSource.length()<=strFlag.length())
	{
		return dqResult;
	}

	//首期处理
	if(strSource.substr(strSource.length()-strFlag.length())!=strFlag)
	{
		strSource+=strFlag;
	}

	string strTmp;
	int Pos=strSource.find(strFlag);

	while(Pos!=-1)
	{
		dqResult.push_back(strSource.substr(0,Pos));
		strSource.erase(0,Pos+strFlag.length());
		Pos=strSource.find(strFlag);
	}

	return dqResult;
}

string ZString::EraseSpace(const string str)
{
	string strTmp=str;
	strTmp.erase(remove_if(strTmp.begin(),strTmp.end(),ptr_fun(::isspace)),strTmp.end());
	return strTmp;
}

string ZString::TrimLeft(const string str)
{
	string strTmp=str;
	string::iterator pos=find_if(strTmp.begin(),strTmp.end(),not1(ptr_fun(::isspace)));
	strTmp.erase(strTmp.begin(), pos);
	return strTmp;
}

string ZString::TrimRight(const string str)
{
	string strTmp=str;
	string::reverse_iterator rpos = find_if(strTmp.rbegin(),strTmp.rend(),not1(ptr_fun(::isspace)));   
	strTmp.erase(rpos.base(),strTmp.end());
	return strTmp;
}

string ZString::TrimAll(const string str)
{
	return TrimLeft(TrimRight(str));
}
