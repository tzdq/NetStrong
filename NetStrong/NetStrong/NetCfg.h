#pragma once

#include "SysConfig.h"

#define UM_DO_RUN_THREAD (WM_USER + 1)
#define UM_DO_STOP_THREAD (WM_USER + 2)
#define UM_DO_GO_TRANSFER (WM_USER + 3)
#define UM_DO_PAUSE_TRANSFER (WM_USER + 4)

#define UM_NOTICE_RUN_THREAD (WM_USER + 5)
#define UM_NOTICE_STOP_THREAD (WM_USER + 6)
#define UM_NOTICE_GO_TRANSFER (WM_USER + 7)
#define UM_NOTICE_PAUSE_TRANSFER (WM_USER + 8)

#define UM_DO_SAVE (WM_USER + 9)
#define UM_DO_CHANGE_CFG (WM_USER + 10)
#define UM_DO_TRAY_CLEAR (WM_USER + 11)

// NetCfg 对话框

class NetCfg : public CDialog
{
	DECLARE_DYNAMIC(NetCfg)

public:

	NetCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NetCfg();

// 对话框数据
	enum { IDD = IDD_DLG_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	
	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
private:
	struct netst_config_def m_ini;
	void TurnPause();
	void TurnRun();
	LRESULT StopButton(WPARAM wParam, LPARAM lParam);
public:
	void InitData(struct netst_config_def *val);
	DWORD CheckDataChange(struct netst_config_def *val);
	BOOL OnEditTickFilter(MSG* pMsg);
	void GetIP(CString *str);
	DWORD GetThreadCount();
	DWORD GetTick();
	DWORD GetFile(CString *str);

	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnTransfer();
	afx_msg void OnEnChangeEditTick();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnKillfocusEditTick();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnHide();
	
};
