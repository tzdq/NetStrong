
/*-======================- Copyright (c) 2006 -======================-

��������:
    ��³��(zhangluduo) : Ϊ���а��ҵ��˺��Ұ�����Ŭ��!

��ϵ��ʽ:
    zhangluduo@msn.com
	[oopfans]Ⱥ:34064264

�޸�ʱ��:
    2007-02-03

��������:
    �ַ�������

��Ȩ����:
    ����κε�λ,��������ʹ��,����,�޸�,ɢ����������ݴ���,���Ǳ��뱣
    ���˰�Ȩ��Ϣ,��ο���������ڵ��Ͷ�,�������˴������Դ,�����˷ݴ���
    ���κ�BUG,��֪ͨ����,�Ա��ֲ���������ˮƽ���޶����µ�һЩ����Ͳ�
    ��,лл!

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

	//���ݲ�ֱ�Ƕ�Դ�����в��
	deque<string> Splitter(string strSource,const string strFlag);

	//ȥ���ַ��������еĿո�
	string EraseSpace(const string str);

	//ȥ��ǰ��ͺ���Ŀո�
	string TrimAll(const string str);

	//ȥ��ǰ��ո�
	string TrimLeft(const string str);

	//ȥ������ո�
	string TrimRight(const string str);
};

#endif
