
/************************ Copyright (c) 2006 ************************

��������:

    ��³��(zhangluduo) : Ϊ���а��ҵ��˺��Ұ�����Ŭ��!

��ϵ��ʽ:

    zhangluduo@msn.com
	[oopfans]Ⱥ:34064264

�޸�ʱ��:

    2007-03-04

��������:

    Base64����ͽ���

��Ȩ����:

    ����κε�λ,��������ʹ��,����,�޸�,ɢ����������ݴ���,���Ǳ��뱣
    ���˰�Ȩ��Ϣ,��ο���������ڵ��Ͷ�,�������˴������Դ,�����˷ݴ���
    ���κ�BUG,��֪ͨ����,�Ա��ֲ���������ˮƽ���޶����µ�һЩ����Ͳ�
    ��,лл!

	ע:
	��ǰд�Ĵ�����Bug,���ܶ��ļ�����base64����,��������������޸Ķ���!

************************ Copyright (c) 2006 ************************/

#ifndef _ZBASE64
#define _ZBASE64

#pragma warning(disable:4786) 
#include <string>
using namespace std;

class ZLD_API ZBase64
{

public:

	/*����
	DataByte
		[in]��������ݳ���,���ֽ�Ϊ��λ
	*/
	string  Encode(const unsigned char* Data,int DataByte);

	/*����
	DataByte
		[in]��������ݳ���,���ֽ�Ϊ��λ
	OutByte
		[out]��������ݳ���,���ֽ�Ϊ��λ,�벻Ҫͨ������ֵ����
		������ݵĳ���
	*/
	string  Decode(const char* Data,int DataByte,int& OutByte);
};
#endif