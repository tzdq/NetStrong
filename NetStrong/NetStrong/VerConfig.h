#pragma once


// VerConfig �Ի���

class VerConfig : public CDialog
{
	DECLARE_DYNAMIC(VerConfig)

public:
	VerConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~VerConfig();

// �Ի�������
	enum { IDD = IDD_DLG_VER_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
