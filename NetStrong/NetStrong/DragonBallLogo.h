#pragma once


// DragonBallLogo 对话框

class DragonBallLogo : public CDialog
{
	DECLARE_DYNAMIC(DragonBallLogo)

public:
	DragonBallLogo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DragonBallLogo();

// 对话框数据
	enum { IDD = IDD_LOGO };

protected:
	BOOL PreTranslateMessage(MSG *pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void OnTimer( UINT_PTR nIDEvent  );
	void DrawLogoAscii();
	void Meng(char *meng, char *pt);
	DECLARE_MESSAGE_MAP()
};
