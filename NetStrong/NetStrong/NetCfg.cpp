// NetCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetStrong.h"
#include "NetCfg.h"


// NetCfg 对话框

IMPLEMENT_DYNAMIC(NetCfg, CDialog)

NetCfg::NetCfg(CWnd* pParent /*=NULL*/)
	: CDialog(NetCfg::IDD, pParent)
{
	strcpy(m_ini.ip, "192.168.1.2");
	m_ini.port =6000;
	strcpy(m_ini.def_file, CFG_DEF_FILE);
}

NetCfg::~NetCfg()
{
}

void NetCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NetCfg, CDialog)
	ON_BN_CLICKED(IDC_BTN_START, &NetCfg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDOK, &NetCfg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &NetCfg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_TRANSFER, &NetCfg::OnBnClickedBtnTransfer)
	ON_EN_CHANGE(IDC_EDIT_TICK, &NetCfg::OnEnChangeEditTick)
	ON_WM_CHAR()
	ON_EN_KILLFOCUS(IDC_EDIT_TICK, &NetCfg::OnEnKillfocusEditTick)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &NetCfg::OnBnClickedBtnBrowse)

// 	ON_MESSAGE(UM_NOTICE_RUN_THREAD,&CNetStrongDlg::RunThread)
 	ON_MESSAGE(UM_NOTICE_STOP_THREAD,&NetCfg::StopButton)
// 
// 	ON_MESSAGE(UM_NOTICE_GO_TRANSFER,&CNetStrongDlg::GoTransfer)
// 	ON_MESSAGE(UM_NOTICE_PAUSE_TRANSFER,&CNetStrongDlg::PauseTransfer)

ON_BN_CLICKED(IDC_BTN_SAVE, &NetCfg::OnBnClickedBtnSave)
ON_BN_CLICKED(IDC_BTN_HIDE, &NetCfg::OnBnClickedBtnHide)

END_MESSAGE_MAP()


// NetCfg 消息处理程序
void NetCfg::InitData(struct netst_config_def *val)
{
	if (val == NULL) {
		return ;
	}
	memcpy(&m_ini, val, sizeof(struct netst_config_def));

}
DWORD NetCfg::CheckDataChange(struct netst_config_def *val)
{
	return 0;
}
BOOL NetCfg::OnInitDialog()
{
	CComboBox *hCombo;
	CEdit *hEdit;

	hCombo = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
	hCombo->InsertString(-1,"1");
	// 	hCombo->InsertString(-1,"2");
	// 	hCombo->InsertString(-1,"3");
	// 	hCombo->InsertString(-1,"4");
	hCombo->SetCurSel(0);

	hCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TICK);
	hCombo->InsertString(-1,"10");
	hCombo->InsertString(-1,"100");
	hCombo->InsertString(-1,"500");
	hCombo->InsertString(-1,"1000");
	hCombo->SetCurSel(3);


	hEdit = (CEdit*)GetDlgItem(IDC_EDIT_TICK);
	hEdit->SetWindowText("10");


	CIPAddressCtrl *hip;
	hip = (CIPAddressCtrl *)GetDlgItem(IDC_IP_TARGET);
	hip->SetWindowText(m_ini.ip);

	
	hEdit = (CEdit*)GetDlgItem(IDC_EDIT_BROWSE);
	hEdit->SetWindowText(m_ini.def_file);

	GetDlgItem(IDC_BTN_TRANSFER)->EnableWindow(FALSE);
	return TRUE;
}
void NetCfg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void NetCfg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
}

BOOL NetCfg::PreTranslateMessage(MSG* pMsg)
{
// 	if(pMsg->message == WM_KEYDOWN || pMsg->wParam == VK_ESCAPE) {
// 		return 0;
// 	}
	if (pMsg->message == WM_CHAR ) {
		if (pMsg->hwnd == GetDlgItem(IDC_EDIT_TICK)->m_hWnd) {
			OnEditTickFilter(pMsg);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}





// 按下“启动”/“停止”按钮，发消息到父窗体
void NetCfg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton *hButton;
	CString strout;
	CString str_start = "启动";
	HWND hparent;

	hparent = this->GetParent()->GetSafeHwnd();
	hButton = (CButton *)GetDlgItem(IDC_BTN_START);
	hButton->GetWindowText(strout);
	if (strout == str_start) {
		
		hButton->SetWindowText("停止");
		GetDlgItem(IDC_IP_TARGET)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_THREAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRANSFER)->EnableWindow(TRUE);
		::SendNotifyMessage(hparent,UM_DO_RUN_THREAD,NULL,NULL);  
		
	}
	else {
		TurnRun();
		hButton->SetWindowText("启动");
		GetDlgItem(IDC_IP_TARGET)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_THREAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRANSFER)->EnableWindow(FALSE);

// 		GetDlgItem(IDC_EDIT_BROWSE)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_TICK)->EnableWindow(TRUE);
		::SendNotifyMessage(hparent,UM_DO_STOP_THREAD,NULL,NULL);  
	}
	
}

void NetCfg::TurnPause()
{
	CButton *hButton;
	CString strout;
	CString str_start = "传输";
	HWND hparent;

	hparent = this->GetParent()->GetSafeHwnd();
	hButton = (CButton *)GetDlgItem(IDC_BTN_TRANSFER);
	hButton->GetWindowText(strout);

	// todo 检查文件是否存在
	// 文件不存在，退出
	if (0) {
		return  ;
	}

	
	hButton->SetWindowText("暂停");
	GetDlgItem(IDC_EDIT_BROWSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TICK)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_TICK)->EnableWindow(FALSE);
	::SendNotifyMessage(hparent,UM_DO_GO_TRANSFER,NULL,NULL);  
		

}
void NetCfg::TurnRun()
{
	CButton *hButton;
	CString strout;
	CString str_start = "传输";
	HWND hparent;

	hparent = this->GetParent()->GetSafeHwnd();
	hButton = (CButton *)GetDlgItem(IDC_BTN_TRANSFER);
	hButton->GetWindowText(strout);

	// todo 检查文件是否存在
	// 文件不存在，退出
	if (0) {
		return  ;
	}
	
	hButton->SetWindowText("传输");
	GetDlgItem(IDC_EDIT_BROWSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TICK)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_TICK)->EnableWindow(TRUE);
	::SendNotifyMessage(hparent,UM_DO_PAUSE_TRANSFER,NULL,NULL);  

}


LRESULT NetCfg::StopButton(WPARAM wParam, LPARAM lParam)
{
	CButton *hButton;
	TurnRun();
	hButton = (CButton *)GetDlgItem(IDC_BTN_START);
	hButton->SetWindowText("启动");
	GetDlgItem(IDC_IP_TARGET)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_THREAD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_TRANSFER)->EnableWindow(FALSE);

	return 0;
}
// 按下“传输”/“暂停”，通知父窗体
void NetCfg::OnBnClickedBtnTransfer()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CButton *hButton;
	CString strout;
	CString str_start = "传输";
	HWND hparent;

	hparent = this->GetParent()->GetSafeHwnd();
	hButton = (CButton *)GetDlgItem(IDC_BTN_TRANSFER);
	hButton->GetWindowText(strout);

	// todo 检查文件是否存在
	// 文件不存在，退出
	if (0) {
		return  ;
	}

	if (strout == str_start) {
		TurnPause();
// 		hButton->SetWindowText("暂停");
// 		GetDlgItem(IDC_EDIT_BROWSE)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_TICK)->EnableWindow(FALSE);
// 		::SendNotifyMessage(hparent,UM_DO_GO_TRANSFER,NULL,NULL);  

	}
	else {
		TurnRun();
// 		hButton->SetWindowText("传输");
// 		GetDlgItem(IDC_EDIT_BROWSE)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_TICK)->EnableWindow(TRUE);
// 		::SendNotifyMessage(hparent,UM_DO_PAUSE_TRANSFER,NULL,NULL);  
	}
}

BOOL NetCfg::OnEditTickFilter(MSG* pMsg)
{
	DWORD codekey;
	
	codekey = pMsg->wParam;
	if (codekey >= '0' && codekey <= '9' ||
		codekey == VK_BACK) {
		return CDialog::PreTranslateMessage(pMsg);
	}
// 	CString	 strout;
// 	strout.Format("pm	%x", pMsg->wParam);
// 	MessageBox(strout);

	return 0;
}
void NetCfg::OnEnChangeEditTick()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	
	CString strout;
	CEdit *hEdit;
	int len;
	char *pbuf;
	int tick;

	hEdit = (CEdit*)GetDlgItem(IDC_EDIT_TICK);
	hEdit->GetWindowText(strout);
	len = strout.GetLength();
	pbuf = strout.GetBuffer();

	// 限定最大设置值为 60000 ms


	tick = atoi(pbuf);
	if (tick < 10) {
		hEdit->SetWindowText("10");
		hEdit->SetSel(2,2,1);
	}
	else if (tick > 60000) {
		hEdit->SetWindowText("60000");
		hEdit->SetSel(5,5,1);
	}
}

void NetCfg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	MessageBox("kk");
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void NetCfg::OnEnKillfocusEditTick()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

void NetCfg::OnBnClickedBtnBrowse()
{
	static TCHAR BASED_CODE szFilter[] =
		"Bin File(*.bin)|*.bin|"
		"Text File(*.txt)|*.txt|"
		"All File(*.*)|*.*||";

	CFileDialog dlg(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter); 
	CEdit *hEdit;
	CString strout;
	
	if( IDOK == dlg.DoModal() ) {
		hEdit = (CEdit*)GetDlgItem(IDC_EDIT_BROWSE);
		strout = dlg.GetPathName();
		
		hEdit->SetWindowText(strout);
	}
}

void NetCfg::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hparent;

	hparent = this->GetParent()->GetSafeHwnd();
	::SendNotifyMessage(hparent,UM_DO_SAVE,NULL,NULL);

	CString strout;
	strout.Format("文件已经保存在%s", m_ini.log_path);
	MessageBox(strout);		
}
void NetCfg::GetIP(CString *str)
{
	CIPAddressCtrl *hip;
	hip = (CIPAddressCtrl *)GetDlgItem(IDC_IP_TARGET);
	hip->GetWindowText(*str);
}
DWORD NetCfg::GetThreadCount()
{
	CComboBox *hCombo;
	CString strout;
	DWORD val;
	char *pbuf;

	hCombo = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
	hCombo->GetWindowText(strout);
	pbuf = strout.GetBuffer();
	val = atoi(pbuf);
	return val;

}
DWORD NetCfg::GetTick()
{
	CComboBox *hCombo;
	CString strout;
	DWORD val;
	char *pbuf;

	hCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TICK);
	hCombo->GetWindowText(strout);
	pbuf = strout.GetBuffer();
	val = atoi(pbuf);
	return val;
}
DWORD NetCfg::GetFile(CString *str)
{
	CEdit *hEdit;
	CString strout;

	hEdit = (CEdit*)GetDlgItem(IDC_EDIT_BROWSE);
	hEdit->GetWindowText(strout);

	FILE *fp;
	fp = fopen(strout.GetBuffer(), "r");
	if (fp == NULL) {
		MessageBox("路径无效");
		return 1;
	}
	else {
		hEdit->GetWindowText(*str);
		fclose(fp);
	}
	return 0;
	
}
void NetCfg::OnBnClickedBtnHide()
{
// 	static bool show = true;
// 	m_nWnd.cbSize  =  sizeof(NOTIFYICONDATA);  
// 	m_nWnd.hWnd  =  m_hWnd;
// 	m_nWnd.hIcon  =  m_hIcon;
// 	m_nWnd.uID  =  IDR_MAINFRAME;  
// 	m_nWnd.uFlags =  NIF_MESSAGE|NIF_ICON|NIF_TIP;  
// 	m_nWnd.uCallbackMessage  =  WM_MY_TRAY_NOTIFICATION;  
// 	strcpy(m_nWnd.szTip,"传感器终端");
// 	ShowWindow(SW_HIDE); 
	//Shell_NotifyIcon(NIM_ADD,&m_nWnd);

	CButton *hButton;
	HWND hparent;

	hparent = this->GetParent()->GetSafeHwnd();
	::SendNotifyMessage(hparent,UM_DO_TRAY_CLEAR,NULL,NULL);  
	
}
