// VerConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "NetStrong.h"
#include "VerConfig.h"


// VerConfig 对话框

IMPLEMENT_DYNAMIC(VerConfig, CDialog)

VerConfig::VerConfig(CWnd* pParent /*=NULL*/)
	: CDialog(VerConfig::IDD, pParent)
{

}

VerConfig::~VerConfig()
{
}

void VerConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(VerConfig, CDialog)
END_MESSAGE_MAP()

extern char font_list[4][30];
extern int font_index;
// VerConfig 消息处理程序
BOOL VerConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	CStatic *hStatic;
	CString strout;
	static CFont font;

	font.CreatePointFont(100, font_list[font_index]);

	hStatic = (CStatic*)GetDlgItem(IDC_STATIC_VERCFG);
	strout.Format("");
	strout.Format(strout + "SAVE_TICK              %d\n", SAVE_TICK);
 	strout.Format(strout + "CONFIG_MAX_AUTO_MAIL   %d\n", CONFIG_MAX_AUTO_MAIL);
 	strout.Format(strout + "PROGRAM_VERSION        %s\n", PROGRAM_VERSION);
 	strout.Format(strout + "EMAIL_TITLE            %s\n", EMAIL_TITLE);
 	strout.Format(strout + "PROGRAM_TITLE          %s\n", PROGRAM_TITLE);
	hStatic->SetFont(&font);
	hStatic->SetWindowText(strout);
	
	
	

	SetWindowText("软件配置信息");
	return true;
}