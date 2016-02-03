#pragma once


// VerConfig 对话框

class VerConfig : public CDialog
{
	DECLARE_DYNAMIC(VerConfig)

public:
	VerConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~VerConfig();

// 对话框数据
	enum { IDD = IDD_DLG_VER_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
