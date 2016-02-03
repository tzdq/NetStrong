#pragma once
#define UM_UPDATA_STRING (WM_USER + 20)
#define UM_ADD_STRING		(WM_USER + 22)
#define UM_AUTOSCROLL		(WM_USER + 23)
#define UM_DO_SELECT_THREAD    (WM_USER + 24)
// MsgView �Ի���
#include "netstrongtype.h"

class MsgView : public CDialog
{
	DECLARE_DYNAMIC(MsgView)
	BOOL m_en_input;

public:
	CString m_strout;
	MsgView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MsgView();

// �Ի�������
	enum { IDD = IDD_DLG_MSG_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	DWORD InputString(char *pbuf, unsigned long len);
	DWORD UpdateString(char *pbuf,unsigned long len);
	void  ClearString();
	void EnableAutoScroll();
	void InputValid(BOOL en);
	bool IsEnableInput();

	void InsertThreadInfo(struct con_thread *ct, int len);
	void UpdateSpeed(struct con_thread *ct, int len);
	void DeleteAllThreadInfo();


	


	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedShow();
	afx_msg void OnSize(UINT nType,int cx, int cy );
};
