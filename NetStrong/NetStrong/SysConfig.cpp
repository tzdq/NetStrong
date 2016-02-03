// SysConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "NetStrong.h"
#include "SysConfig.h"


// SysConfig 对话框

IMPLEMENT_DYNAMIC(SysConfig, CDialog)

SysConfig::SysConfig(CWnd* pParent /*=NULL*/)
	: CDialog(SysConfig::IDD, pParent)
{
	m_ini.log_path[0] = '\0';
	m_ini.def_file[0] = '\0';
	m_ini.en_mail = 0;
	m_ini.mail_tick = 3;
}

SysConfig::~SysConfig()
{
}

void SysConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SysConfig, CDialog)
	ON_BN_CLICKED(ID_OK, &SysConfig::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &SysConfig::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_BROWSE2, &SysConfig::OnBnClickedBtnBrowse2)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &SysConfig::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_BROWSE1, &SysConfig::OnBnClickedBtnBrowse1)
END_MESSAGE_MAP()


// SysConfig 消息处理程序

void SysConfig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit *hEdit;
	CButton *hBtn;
	CComboBox *hCom;
	CString strout;
	int tmpint;

	//str_last.Format("%s", val->log_path);
	hEdit = (CEdit *)GetDlgItem(IDC_EDIT1);
	hEdit->GetWindowText(strout);
	strcpy(m_ini.log_path, strout.GetBuffer());

	hEdit = (CEdit *)GetDlgItem(IDC_EDIT2);
	hEdit->GetWindowText(strout);
	strcpy(m_ini.def_file, strout.GetBuffer());

	hBtn = (CButton *)GetDlgItem(IDC_CHECK1);
	m_ini.en_mail = hBtn->GetCheck();

	hCom = (CComboBox *)GetDlgItem(IDC_COMBO1);
	hCom->GetWindowText(strout);
	tmpint = atoi(strout.GetBuffer());
	m_ini.mail_tick = tmpint;

	OnOK();
}

void SysConfig::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
// 载入以前的配置文件

BOOL SysConfig::OnInitDialog()
{
	CDialog::OnInitDialog();
	//MessageBox(",,,,,,,,");
	CString strout;
	CListCtrl *hList;
	CRect rect;

	int index;
	CComboBox *hCombo;
	CEdit *hEdit;

	hCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	hCombo->InsertString(-1,"1 小时");
	hCombo->InsertString(-1,"2 小时");
	hCombo->InsertString(-1,"3 小时");
	hCombo->InsertString(-1,"4 小时");
	hCombo->InsertString(-1,"5 小时");
	hCombo->InsertString(-1,"6 小时");
	hCombo->InsertString(-1,"7 小时");
	hCombo->InsertString(-1,"8 小时");
	hCombo->InsertString(-1,"9 小时");
	hCombo->InsertString(-1,"10 小时");
	hCombo->InsertString(-1,"11 小时");
	hCombo->InsertString(-1,"12 小时");
	hCombo->InsertString(-1,"13 小时");
	hCombo->InsertString(-1,"14 小时");
	hCombo->InsertString(-1,"15 小时");
	hCombo->InsertString(-1,"16 小时");
	hCombo->InsertString(-1,"17 小时");
	hCombo->InsertString(-1,"18 小时");
	hCombo->InsertString(-1,"19 小时");
	hCombo->InsertString(-1,"20 小时");
	hCombo->InsertString(-1,"21 小时");
	hCombo->InsertString(-1,"22 小时");
	hCombo->InsertString(-1,"23 小时");
	hCombo->InsertString(-1,"24 小时");
	

	// 选择是否
	struct netst_config_def ini;
	LoadConfig(&ini);
	strout.Format("%d 小时", ini.mail_tick);
	index = hCombo->FindStringExact(0, strout);
	// 没有找到，默认选择3 小时
	if (index == -1) {
		hCombo->SetCurSel(2);
	}
	else {
		hCombo->SetCurSel(index);
	}
	
// 
// 	hCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TICK);
// 	hCombo->InsertString(-1,"10");
// 	hCombo->InsertString(-1,"100");
// 	hCombo->InsertString(-1,"500");
// 	hCombo->InsertString(-1,"1000");
// 	hCombo->SetCurSel(3);


	// 父窗口在调用DoModal前，最好先调用InitData给m_ini赋值
	hEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	hEdit->SetWindowText(m_ini.log_path);

	hEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	hEdit->SetWindowText(m_ini.def_file);

 	CButton *hbtn;
 	hbtn= (CButton *)GetDlgItem(IDC_CHECK1);
	hbtn->SetCheck(m_ini.en_mail);


	
	return TRUE;
}


void SysConfig::LoadConfig(struct netst_config_def *val)
{
	struct netst_config_def cfg;
	char str[32];

	if (val == NULL) {
		return ;
	}
	GetPrivateProfileString("NetStrong", "ip", "192.168.1.251",
		val->ip,16,
		CFG_INI_FILE);
	GetPrivateProfileString("NetStrong", "port", "6000",
		str,16,
		CFG_INI_FILE);
	val->port = atoi(str);

	GetPrivateProfileString("NetStrong", "logpath", CFG_DEF_LOGPATH,
		val->log_path,256,
		CFG_INI_FILE);
	GetPrivateProfileString("NetStrong", "deffile", ".\\netpacket\\2k.bin",
		val->def_file,256,
		CFG_INI_FILE);

	GetPrivateProfileString("NetStrong", "enmail", "1",
		str,32,
		CFG_INI_FILE);
	val->en_mail = atoi(str);
	val->en_mail = !!val->en_mail;

	GetPrivateProfileString("NetStrong", "mailtick", "3",
		str,32,
		CFG_INI_FILE);
	val->mail_tick = atoi(str);
	if (val->mail_tick >= 24) {
		val->mail_tick = 24;
	}

	GetPrivateProfileString("NetStrong", "smail", "dev2016mail@163.com",
		val->s_mail,64,
		CFG_INI_FILE);
	GetPrivateProfileString("NetStrong", "dmail", "netstrongwml@163.com",
		val->d_mail,64,
		CFG_INI_FILE);
	GetPrivateProfileString("NetStrong", "firstmail", "1",
		str,64,
		CFG_INI_FILE);
	val->first_mail = atoi(str);
	val->first_mail = !!val->first_mail;

	GetPrivateProfileString("NetStrong", "dbg_mail", "0",
		str,64,
		CFG_INI_FILE);
	val->dbg_mail = atoi(str);
	val->dbg_mail = !!val->dbg_mail;
	
}
void SysConfig::InitData(struct netst_config_def *val)
{


	if (val == NULL) {
		return ;
	}
	strcpy( m_ini.log_path, val->log_path);
	strcpy( m_ini.def_file, val->def_file);
	m_ini.en_mail = val->en_mail;
	m_ini.mail_tick = val->mail_tick;
}
DWORD SysConfig::CheckDataChange(struct netst_config_def *val)
{
	if (strcmp(m_ini.log_path, val->log_path) != 0) {
		goto _Copy;
	}
	else if (strcmp(m_ini.def_file, val->def_file) != 0) {
		goto _Copy;
	}
	else if (m_ini.en_mail != val->en_mail) {
		goto _Copy;

	}
	else if (m_ini.mail_tick != val->mail_tick) {
		goto _Copy;

	}
	return 0;
_Copy:;
	strcpy(val->log_path, m_ini.log_path);
	strcpy(val->def_file, m_ini.def_file);
	val->en_mail   = m_ini.en_mail;
	val->mail_tick = m_ini.mail_tick;

	return 1;
}
void SysConfig::SaveConfig(netst_config_def *val)
{
	struct netst_config_def cfg;
	char str[32];

	if (val == NULL) {
		return ;
	}
	CreateDirectory("d:\\NetStrong",NULL);
	WritePrivateProfileString("NetStrong", "ip", 
		val->ip, CFG_INI_FILE);
	itoa(val->port, str,10);
	WritePrivateProfileString("NetStrong", "port", 
		str, CFG_INI_FILE);
	WritePrivateProfileString("NetStrong", "logpath", 
		val->log_path, CFG_INI_FILE);
	WritePrivateProfileString("NetStrong", "deffile", 
		val->def_file, CFG_INI_FILE);
	itoa(val->en_mail, str,10);
	WritePrivateProfileString("NetStrong", "enmail", 
		str,CFG_INI_FILE);
	itoa(val->mail_tick, str,10);
	WritePrivateProfileString("NetStrong", "mailtick", 
		str,CFG_INI_FILE);
	val->first_mail = 0;
	itoa(val->first_mail, str,10);
	WritePrivateProfileString("NetStrong", "firstmail",
		str,CFG_INI_FILE);

	val->dbg_mail = 0;
	itoa(val->dbg_mail, str,10);
	WritePrivateProfileString("NetStrong", "dbg_mail",
		str,CFG_INI_FILE);
}
CString BrowseFolder(HWND hWnd, LPCTSTR lpTitle)
{
	char szPath[MAX_PATH]={0};
	BROWSEINFO m_bi;

	m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.hwndOwner = hWnd;
	m_bi.pidlRoot = NULL;
	m_bi.lpszTitle = lpTitle;
	m_bi.lpfn = NULL;
	m_bi.lParam = NULL;
	m_bi.pszDisplayName = szPath;

	LPITEMIDLIST pidl = ::SHBrowseForFolder( &m_bi );
	if ( pidl )
	{
		if( !::SHGetPathFromIDList ( pidl, szPath ) ) szPath[0]=0;

		IMalloc * pMalloc = NULL;
		if ( SUCCEEDED ( ::SHGetMalloc( &pMalloc ) ) ) // 取得IMalloc分配器接口
		{
			pMalloc->Free( pidl ); // 释放内存
			pMalloc->Release(); // 释放接口
		}
	}
	return szPath;
}





void SysConfig::OnBnClickedBtnBrowse()
{

}

void SysConfig::OnBnClickedBtnBrowse1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString SelectPath=BrowseFolder(this->m_hWnd,"选择日志目录"); 
	//AfxMessageBox(SelectPath);

	CEdit *hEdit;
	hEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	hEdit->SetWindowText(SelectPath);
}
void SysConfig::OnBnClickedBtnBrowse2()
{
	// TODO: 在此添加控件通知处理程序代码
	static TCHAR BASED_CODE szFilter[] =
		"Bin File(*.bin)|*.bin|"
		"Text File(*.txt)|*.txt|"
		"All File(*.*)|*.*||";

	CFileDialog dlg(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter); 
	CEdit *hEdit;
	CString strout;

	if( IDOK == dlg.DoModal() ) {
		hEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
		strout = dlg.GetPathName();

		hEdit->SetWindowText(strout);
	}
}