
// NetStrongDlg.h : 头文件
//

#pragma once
#include "netstrongtype.h"
#include "ViwCfg.h"
#include "NetCfg.h"
#include "MsgView.h"
#include "SysConfig.h"
#include "DragonBallLogo.h"
#include "VerConfig.h"

#define WM_MY_TRAY_NOTIFICATION WM_USER+1005






#define CACHE_LEN (1024*1024*7)
//#define CACHE_LEN (1024*60)


// CNetStrongDlg 对话框
class CNetStrongDlg : public CDialog
{
public:
	struct con_thread ct[1];
	//CViwCfg *pm_net_cfg;
	NetCfg *m_pnet_cfg;
	MsgView *m_pmsg_view;
	DWORD m_select_thread;

	CString m_strip;
	DWORD m_thread, m_tick;

	NOTIFYICONDATA m_nWnd;

	struct netst_config_def m_ini;
	CTime m_tmcon;// 启动连接时间
	CTime m_tmsend;// 启动发送时间
	CTime m_tmsave;// 启动保存时间
	char m_savefile[256];// 自动保存、
	char m_autofile[256];// 邮件发送文件名
	DWORD m_autotick;
	// 构造
public:
	CNetStrongDlg(CWnd *pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_NETSTRONG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;
	void InitChildWin();
	int ReadySendThread(int count, char *file, int tick);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void TTConnectValid();
	void TTAutoSaveTick();
	void OnTimer( UINT_PTR nIDEvent  );
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL PreTranslateMessage(MSG *pMsg);
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg LRESULT RunThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT StopThread(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT GoTransfer(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT PauseTransfer(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT SelectThreadItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveLog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ChangeConfig(WPARAM wParam, LPARAM lParam);
	afx_msg void OnConfigDefault();
	afx_msg void OnSaveAs();
	afx_msg void OnSave();
	afx_msg void OnDataClear();
	afx_msg void OnTray();
	afx_msg void OnAutoMail();
	afx_msg void OnAutoSave();
	afx_msg void OnAbout();
	afx_msg void OnVerConfig();
	afx_msg LRESULT OnTrayNotification(WPARAM   wp,   LPARAM   lp);
	afx_msg LRESULT OnTrayClear(WPARAM   wp,   LPARAM   lp);

	afx_msg void OnSize(
		UINT nType,
		int cx,
		int cy 
		);


private:
	void SendToEmail();
	void SendNullEmail();
	void SaveLog(char *path, char *file);
	void FormatTm(CTime *tm);
	void MailTitle(CString *str, bool emailformat);
	void LogoTitle(CString *str, bool emailformat);
	void InitLogPath(char *path);
	void ViableSelectThread(DWORD val);
	void GetThreadLog();
	int VirsionRequest();
};
