#pragma once
struct netst_config_def
{
	char ip[16];
	short port;
	char def_file[256];
	char log_path[256];
	long en_mail;
	long mail_tick;
	char s_mail[64];
	char d_mail[64];
	long first_mail;// 首次运行立即发送邮件，用户可以直接关闭防火墙
	long dbg_mail;// 调试模式运行发送邮件，用户不能使用一键发送
};
#define CFG_INI_FILE "d:\\NetStrong\\cfg.ini"
#define CFG_DEF_LOGPATH "d:\\NetStrong\\log"
#define CFG_DEF_FILE ".\\netpacket\\2k.bin"

// SysConfig 对话框

class SysConfig : public CDialog
{
	DECLARE_DYNAMIC(SysConfig)

public:
	SysConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SysConfig();

// 对话框数据
	enum { IDD = IDD_SYS_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	struct netst_config_def m_ini;
	
	DECLARE_MESSAGE_MAP()
public:
	//void SysConfig::LoadOld(char *path);
	void SysConfig::LoadConfig(netst_config_def *val);
	void SysConfig::SaveConfig(netst_config_def *val);

 	void InitData(struct netst_config_def *val);
	DWORD CheckDataChange(struct netst_config_def *val);
// 	void GetLogPath(CString *str);
// 	void GetLogPath(CString *str);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnBrowse2();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnBrowse1();
};
