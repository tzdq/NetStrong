// ViwCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetStrong.h"
#include "ViwCfg.h"


// CViwCfg

IMPLEMENT_DYNCREATE(CViwCfg, CFormView)

CViwCfg::CViwCfg()
	: CFormView(CViwCfg::IDD)
{

}

CViwCfg::~CViwCfg()
{
}

void CViwCfg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViwCfg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &CViwCfg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CViwCfg 诊断

#ifdef _DEBUG
void CViwCfg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViwCfg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViwCfg 消息处理程序

void CViwCfg::OnBnClickedButton2()
{
}
