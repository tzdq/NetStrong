#pragma once


// DragonBallLogo �Ի���

class DragonBallLogo : public CDialog
{
	DECLARE_DYNAMIC(DragonBallLogo)

public:
	DragonBallLogo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DragonBallLogo();

// �Ի�������
	enum { IDD = IDD_LOGO };

protected:
	BOOL PreTranslateMessage(MSG *pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void OnTimer( UINT_PTR nIDEvent  );
	void DrawLogoAscii();
	void Meng(char *meng, char *pt);
	DECLARE_MESSAGE_MAP()
};
