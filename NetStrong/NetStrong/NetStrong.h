
// NetStrong.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNetStrongApp:
// �йش����ʵ�֣������ NetStrong.cpp
//

class CNetStrongApp : public CWinAppEx
{
public:
	CNetStrongApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNetStrongApp theApp;