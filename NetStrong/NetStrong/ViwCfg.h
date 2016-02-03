#pragma once



// CViwCfg 窗体视图

class CViwCfg : public CFormView
{
	DECLARE_DYNCREATE(CViwCfg)

protected:
	CViwCfg();           // 动态创建所使用的受保护的构造函数
	virtual ~CViwCfg();

public:
	enum { IDD = IDD_DIALOG1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
};


