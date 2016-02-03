// MsgView.cpp : 实现文件
//

#include "stdafx.h"
#include "NetStrong.h"
#include "MsgView.h"


// MsgView 对话框

IMPLEMENT_DYNAMIC(MsgView, CDialog)

MsgView::MsgView(CWnd *pParent /*=NULL*/)
	: CDialog(MsgView::IDD, pParent)
{
	m_en_input = true;
}

MsgView::~MsgView()
{
}

void MsgView::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MsgView, CDialog)
	ON_BN_CLICKED(IDOK, &MsgView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MsgView::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &MsgView::OnNMDblclkList1)
	ON_WM_SIZE()
//	ON_BN_CLICKED(IDC_CHECK1, &MsgView::OnBnClickedCheck1)
ON_BN_CLICKED(IDC_SHOW, &MsgView::OnBnClickedShow)
	
END_MESSAGE_MAP()


// MsgView 消息处理程序
BOOL MsgView::OnInitDialog()
{
	CDialog::OnInitDialog();
	//MessageBox(",,,,,,,,");
	CListCtrl *hList;
	CRect rect;

	hList = (CListCtrl *)GetDlgItem(IDC_LIST1);
	hList->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	hList ->GetWindowRect(&rect);


	hList->InsertColumn(0, _T("线程"), LVCFMT_LEFT, (int)(rect.Width() * 0.2)); //设置列
	hList->InsertColumn(1, _T("速度"), LVCFMT_LEFT, (int)(rect.Width() * 0.2));
	hList->InsertColumn(2, _T("sip"), LVCFMT_LEFT, (int)(rect.Width() * 0.3));
	hList->InsertColumn(3, _T("dip"), LVCFMT_LEFT, (int)(rect.Width() * 0.3));
	hList->InsertColumn(4, _T("tid"), LVCFMT_LEFT, (int)(rect.Width() * 0.2));

	// 	hList->InsertItem(0,0);
	// 	hList->SetItemText(0,0,"dff");
	// 	hList->SetItemText(0,1,"dff");
	//
	// 	hList->InsertItem(1,0);
	// 	hList->SetItemText(1,0,"dff1");
	// 	hList->SetItemText(1,1,"dff11");
	//
	// 	hList->InsertItem(2,0);
	// 	hList->SetItemText(2,0,"dff2");
	// 	hList->SetItemText(2,1,"dff22");
	//hList->DeleteAllItems();

	CButton *hbtn;
	hbtn = (CButton*)GetDlgItem(IDC_SHOW);
	hbtn->SetWindowText("显示");
	hbtn->SetCheck(1);
	return TRUE;
}

void MsgView::OnSize(
						   UINT nType,
						   int cx,
						   int cy 
						   )
{
	RECT rect, trect;
	GetClientRect(&rect);
	CEdit *hEdit;
	CListCtrl *hList;

	hEdit = (CEdit *)GetDlgItem(IDC_EDIT1);	
	hList = (CListCtrl *)GetDlgItem(IDC_LIST1);
	// 窗口刚建立时候子窗口没有建立
	if (hList && hEdit) {
		rect.left += 10;
		rect.right -= 10;
		rect.bottom -= 10;
		trect = rect;
		trect.top    = trect.top + 30;
		trect.bottom = trect.bottom - 120;
		hEdit->MoveWindow(&trect);

		trect = rect;
		trect.top = trect.bottom - 110;
		hList->MoveWindow(&trect);

	}
}

void MsgView::OnBnClickedOk()
{
	//OnOK();
}

void MsgView::OnBnClickedCancel()
{
	//	OnCancel();
}

DWORD MsgView::InputString(char *pbuf, unsigned long len)
{

	// 	if ( *(pbuf + len) != '\0') {
	// 		return 0;
	// 	}



	return 1;
}
DWORD MsgView::UpdateString(char *pbuf, unsigned long len)
{
	// 	if ( *(pbuf + len) != '\0') {
	// 		return 0;
	// 	}
	if (m_en_input == FALSE) {
		return 0;
	}

	m_strout.Format("%s", pbuf);


	CEdit *hEdit;
	hEdit = (CEdit *)GetDlgItem(IDC_EDIT1);
	hEdit->SetWindowText(m_strout);
	hEdit->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	return 1;
}
void  MsgView::ClearString()
{

}
void MsgView::EnableAutoScroll()
{

}
void MsgView::InputValid(BOOL en)
{
	m_en_input = en;
}

bool MsgView::IsEnableInput()
{
	return m_en_input;
}

void MsgView::InsertThreadInfo(struct con_thread *ct, int len)
{
	CListCtrl *hList;
	CString strout;

	hList = (CListCtrl *)GetDlgItem(IDC_LIST1);
	hList->DeleteAllItems();
	for (int i = 0; i < len; i++) {
		hList->InsertItem(i, 0);


		strout.Format("%d", ct[i].thread_fake_id);
		hList->SetItemText(i, 0, strout);

		hList->SetItemText(i, 1, "0B/s");


		//hList->SetItemText(i, 2, ct[i].d_ip);

		hList->SetItemText(i, 3, ct[i].d_ip);

		strout.Format("%d", ct[i].thread_fake_id);
		//hList->SetItemText(i, 4, strout);

	}

}
void MsgView::UpdateSpeed(struct con_thread *ct, int len)
{
	DWORD total_send;
	CString strout;
	CListCtrl *hList;

	hList = (CListCtrl *)GetDlgItem(IDC_LIST1);
	for (int i = 0; i < len; i++) {
		total_send = ct[i].s_byte;
		if (total_send > 1024 * 1024) {
			strout.Format( "%0.2fMB/s", (float)total_send / (1024 * 1024));
		}
		else if (total_send > 1024) {
			strout.Format( "%0.2fKB/s", (float)total_send / (1024));
		}
		else  {
			strout.Format(  "%dB/s", total_send);
		}

		//strout.Format("%d-",ct[i].thread_fake_id);
		hList->SetItemText(i, 1, strout);
	}
}
void MsgView::DeleteAllThreadInfo()
{
	CListCtrl *hList;

	hList = (CListCtrl *)GetDlgItem(IDC_LIST1);
	hList->DeleteAllItems();
}
void MsgView::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	*pResult = 0;

	CListCtrl *hList;
	POSITION selectItem;

	hList = (CListCtrl *)GetDlgItem(IDC_LIST1);
	selectItem = hList->GetFirstSelectedItemPosition();
// 	TRACE("POST %d", pos);
// 	CString strout;
// 	strout.Format("--%d", pos);
// 	MessageBox(strout);

	HWND hparent;

	hparent = this->GetParent()->GetSafeHwnd();
	::SendNotifyMessage(hparent,UM_DO_SELECT_THREAD, DWORD(selectItem) - 1,NULL);  
}

//void MsgView::OnBnClickedCheck1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}



void MsgView::OnBnClickedShow()
{
	// TODO: 在此添加控件通知处理程序代码
// 
	CButton *hbtn;
	hbtn = (CButton*)GetDlgItem(IDC_SHOW);
	hbtn->SetWindowText("显示");
	
	if (hbtn->GetCheck()) {
		m_en_input = true;
	}
	else {
		m_en_input = false;
	}

	
}
