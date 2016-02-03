
// NetStrongDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetStrong.h"
#include "NetStrongDlg.h"
#include "zld.h"


#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

int g_con_success = 1;

DWORD SendMail(//不使用hlp.dll里面的SendEMail
    CString SmtpAddr,	// smtp服务器地址
    CString Account,		// 帐号
    CString Password,	// 密码
    CString Sendfrom,	// 发送源地址
    CString Sendto,		// 目地地址
    CString SenderName,	// 发送者姓名
    CString ReceiverName,// 接收者姓名
    CString Subject,		// 邮件主题
    CString Content,		// 邮件正文
    deque<string> m_dqFiles
);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	// 实现
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CStatic *hStatic;

	hStatic = (CStatic*)GetDlgItem(IDC_STATIC_VERSION);
	// 如果字体太短，控件背景（不是字体背景）是灰色，暂时修改不了背景色
	hStatic->SetWindowText("NetStrong 网络强壮性测试客户端");



	return TRUE;
}
HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		//pDC->SetTextColor(RGB(255,0,0));  //字体颜色
		pDC->SetBkColor(RGB(255, 255, 255));   //字体背景色  
		pDC->SetDCBrushColor(RGB(0,255,0));
	//	pDC->
		//pDC->SetBkMode(TRANSPARENT);
	}
	// TODO: Return
	return hbr;
}
void CAboutDlg::OnPaint()
{
	CDC	 *pDc;
	pDc = GetDC();
	SetBkColor(pDc->m_hDC, RGB(255,0,0));
	CRect   rect;  

	CPaintDC   dc(this);  

	GetClientRect(rect);  

	dc.FillSolidRect(rect,RGB(255,255,255));   //设置为绿色背景
	OnBnClickedButton1();

	CDialog::OnPaint();
}

void CAboutDlg::OnBnClickedButton1()
{
	int cx, cy;  
	CImage  image;  
	CWnd *pWnd = NULL;
	CDC *pDc = NULL;
	CRect   rect;

	//image.LoadFromResource(0, IDB_BITMAP1);
	//image.Load("res\\dragonball.bmp");
	image.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP1);
	//获取图片的宽 高度  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();  

	//获取Picture Control控件的大小  
	GetDlgItem(IDC_BMP_LOGO)->GetWindowRect(&rect);  
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);  
	//窗口移动到控件表示的区域  
	GetDlgItem(IDC_BMP_LOGO)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);  
	pWnd    = GetDlgItem(IDC_BMP_LOGO);//获取控件句柄  
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小  
	pDc = pWnd->GetDC();//获取picture的DC  
	image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内
	ReleaseDC(pDc);  

}
// CNetStrongDlg 对话框




CNetStrongDlg::CNetStrongDlg(CWnd *pParent /*=NULL*/)
	: CDialog(CNetStrongDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (int i = 0; i < sizeof(ct) / sizeof(struct con_thread ); i++) {
		ct[i].s_byte = 0;
	}
	m_select_thread = 0;

	m_strip.Format("");
	//m_strfile.Format("");
	m_thread = 1;
	m_tick = 1000;
	for (int i = 0 ; i < sizeof(ct) / sizeof(struct con_thread ); i++ ) {
		ct[i].prbuf = NULL;
		memset(&ct[i], 0 , sizeof(struct con_thread));
		ct[i].sleep_tick = 500;
		ct[i].rbuf_len = CACHE_LEN;
		ct[i].prbuf = (char *)malloc(ct[i].rbuf_len);
		memset(ct[i].prbuf, 0, ct[i].rbuf_len);
		if (ct[i].prbuf == NULL) {
			printf("no memory\n");
			system("pause");
			exit(0);
		}
		ct[i].prbuf[0] = '\0';

	}

	m_tmcon  = CTime::GetCurrentTime();
	m_tmsend = CTime::GetCurrentTime();
	m_tmsave = CTime::GetCurrentTime();
	strcpy(m_savefile, ".\netpacket\2k.bin");
	strcpy(m_autofile, "auto");
	m_autotick = 0;
	m_pmsg_view = NULL;
	m_pnet_cfg = NULL;

}

void CNetStrongDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetStrongDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CNetStrongDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNetStrongDlg::OnBnClickedCancel)

	ON_MESSAGE(UM_DO_RUN_THREAD, &CNetStrongDlg::RunThread)
	ON_MESSAGE(UM_DO_STOP_THREAD, &CNetStrongDlg::StopThread)

	ON_MESSAGE(UM_DO_GO_TRANSFER, &CNetStrongDlg::GoTransfer)
	ON_MESSAGE(UM_DO_PAUSE_TRANSFER, &CNetStrongDlg::PauseTransfer)

	ON_MESSAGE(UM_DO_SELECT_THREAD, &CNetStrongDlg::SelectThreadItem)
	ON_MESSAGE(UM_DO_SAVE, &CNetStrongDlg::OnSaveLog)
	ON_MESSAGE(UM_DO_CHANGE_CFG, &CNetStrongDlg::ChangeConfig)

	ON_COMMAND(ID_MENU_CFG_DEF, &CNetStrongDlg::OnConfigDefault)
	ON_COMMAND(ID_MENU_SAVEAS, &CNetStrongDlg::OnSaveAs)
	ON_COMMAND(ID_MENU_SAVE, &CNetStrongDlg::OnSave)
	ON_COMMAND(ID_MENU_DATACLEAR, &CNetStrongDlg::OnDataClear)
	ON_COMMAND(ID_MENU_TRAY, &CNetStrongDlg::OnTray)
	ON_COMMAND(ID_MENU_AUTOMAIL, &CNetStrongDlg::OnAutoMail)
	ON_COMMAND(ID_MENU_ABOUT, &CNetStrongDlg::OnAbout)
	ON_COMMAND(ID_MENU_VERSION_CONFIG, &CNetStrongDlg::OnVerConfig)


	ON_MESSAGE(UM_DO_TRAY_CLEAR, &CNetStrongDlg::OnTrayClear)
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, &CNetStrongDlg::OnTrayNotification)

	ON_WM_SIZE()

END_MESSAGE_MAP()


// CNetStrongDlg 消息处理程序

BOOL CNetStrongDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	VirsionRequest();
	CString strContext;
	strContext.Format("%s %s", PROGRAM_TITLE, PROGRAM_VERSION);
	SetWindowText(strContext.GetBuffer());
	DragonBallLogo logodlg;
	logodlg.DoModal();

	// 载入配置文件
	SysConfig dlg;
	dlg.LoadConfig(&m_ini);
	if (m_ini.first_mail) {
		SendNullEmail();
		dlg.SaveConfig(&m_ini);
	}

	CMenu menu;
	menu.LoadMenu(IDR_MAIN);
	SetMenu(&menu);
	//main();
	InitChildWin();
	//SetTimer(0,1000,NULL);
	//ReadySendThread(0,0,0);

	// 	CString strout;
	// 	strout.Format("%8.8x", this->m_hWnd);
	// 	MessageBox(strout);
	CString strout;
	char *pbuf;
	int len = 1024 * 1024 * 4;
	// 	pbuf = (char*)malloc(len);
	// 	for (int i = 0; i < len; i++) {
	// 		*(pbuf+i) = 'k';
	// 	}
	// 	pbuf[len - 1] = '\0';
	//strout.Format("%s",pbuf);
	//m_pmsg_view->UpdateString(strout.GetBuffer(), 0);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CNetStrongDlg::OnTimer( UINT_PTR nIDEvent  )
{
	if (nIDEvent == 1) {
		//SetTimer(0, 1000, NULL);
		KillTimer(1);
		TTConnectValid();
	}
	CStatic *hWnd;

	hWnd = (CStatic *)GetDlgItem(IDC_STATIC);
	/*MessageBox("ddf");*/
	CString strout;;

	unsigned long total_send = 0;

	//strout.Format("%d", ct[0].s_byte);
	//ct[0].s_byte = 0;

	hWnd->SetWindowText(strout);

	if (m_pmsg_view->IsEnableInput() == true) {
		GetThreadLog();
	}


	m_pmsg_view->UpdateSpeed(ct, sizeof(ct) / sizeof(struct con_thread ));
	for (int i = 0; i < sizeof(ct) / sizeof(struct con_thread ); i++) {
		total_send += ct[i].s_byte;
		ct[i].s_byte = 0;
	}
	TTConnectValid();
	TTAutoSaveTick();
}

// 定时任务，检查连接是否有效
void CNetStrongDlg::TTConnectValid()
{
	DWORD ExitCode;
	bool ret;
	if (g_con_success == 0) {
		goto _CloseAll;
	}

	return ;
_CloseAll:
	;
	for (int i = 0; i < sizeof(ct) / sizeof(struct con_thread ); i++) {
		if (ct[i].hThread) {
			CloseHandle(ct[i].hThread);
			ct[i].hThread = 0;
		}
	}
	KillTimer(0);
	StopThread(NULL, NULL);
	return ;

}

// 定时检查是否需要自动保存
void CNetStrongDlg::TTAutoSaveTick()
{

	// 达到定时时间就保存
	// 或者接受数据还差90K达到缓存上限也保存
	if (m_ini.en_mail && 
		(m_autotick++ >= (SAVE_TICK * m_ini.mail_tick) ||
	     ct[0].rbuf_use > (CACHE_LEN - (1024*90)) )
		) {
		m_autotick = 0;
		OnAutoSave();
	}
}
void CNetStrongDlg::OnSize(
					UINT nType,
					int cx,
					int cy 
					)
{
	RECT rect;
	GetClientRect(&rect);
//	rect.left = 200;
	//m_pmsg_view = new MsgView;
// 	m_pmsg_view->Create(IDD_DLG_MSG_VIEW, this);
// 	m_pmsg_view->ShowWindow(SW_SHOW);
	if (m_pmsg_view) {

		rect.left = 200;
		m_pmsg_view->MoveWindow(&rect, TRUE);
	}
	
}
void CNetStrongDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}

	
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNetStrongDlg::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNetStrongDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CNetStrongDlg::ReadySendThread(int count, char *file, int tick)
{
	unsigned long tID = 1;

	memset(&ct, 0, sizeof(struct con_thread));

	for (int i = 0 ; i < sizeof(ct) / sizeof(struct con_thread ); i++ ) {
		//for (int i = 0 ;i < 1; i++ ) {
		strcpy(ct[i].d_ip, m_ini.ip);
		ct[i].d_port = m_ini.port;
		switch(i) {
		case 0:
			strcpy(ct[i].fname, "netpacket/10k.bin");
			ct[i].sleep_tick = 1000;
			break;
		case 1:
			strcpy(ct[i].fname, "netpacket/2k.bin");
			ct[i].sleep_tick = 14000;
			break;
		case 2:
			strcpy(ct[i].fname, "netpacket/4k.bin");
			ct[i].sleep_tick = 13000;
			break;
		case 3:
			strcpy(ct[i].fname, "netpacket/6k.bin");
			ct[i].sleep_tick = 12000;
			break;
		default:
			strcpy(ct[i].fname, "netpacket/6k.bin");
			ct[i].sleep_tick = 12000;
			break;
		}

		ct[i].en_run  = 1;
		ct[i].en_send = 0;
		ct[i].rbuf_len = CACHE_LEN;
		ct[i].thread_fake_id = i;
		ct[i].sleep_tick = 500;
		if (NULL == ct[i].prbuf) {
			ct[i].prbuf = (char *)malloc(ct[i].rbuf_len);
			memset(ct[i].prbuf, 0, ct[i].rbuf_len);
		}

		if (ct[i].prbuf == NULL) {
			printf("no memory\n");
			system("pause");
			exit(0);
		}
		ct[i].prbuf[0] = '\0';

		ct[i].hThread = CreateThread(0, 0x1000, OneConnect, (void *)&ct[i], 0, &tID);
		printf("tid %d\n", tID);
	}
	m_pmsg_view->InsertThreadInfo(ct, sizeof(ct) / sizeof(struct con_thread ));


	return 0;
}
void CNetStrongDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void CNetStrongDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
BOOL CNetStrongDlg::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN || pMsg->wParam == VK_ESCAPE) {
		return 0;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CNetStrongDlg::InitChildWin()
{
	//pm_net_cfg = new CViwCfg("config");
	RECT rect;
	GetClientRect(&rect);

	m_pnet_cfg = new NetCfg;
	m_pnet_cfg->InitData(&m_ini);
	m_pnet_cfg ->Create(IDD_DLG_CFG, this);
	m_pnet_cfg ->ShowWindow(SW_SHOW);

	m_pmsg_view = new MsgView;
	m_pmsg_view->Create(IDD_DLG_MSG_VIEW, this);
	m_pmsg_view->ShowWindow(SW_SHOW);
	rect.left += 200;
	m_pmsg_view->MoveWindow(&rect, TRUE);

}

LRESULT CNetStrongDlg::RunThread(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("run");
	char str_last[256];
	CString strout;

	// 保存之前的字符串
	strcpy(str_last, m_ini.ip);



	// 读取子窗口的参数值
	m_pnet_cfg->GetIP(&strout);
	strcpy(&m_ini.ip[0], strout.GetBuffer());
	m_pnet_cfg->GetFile(&strout);
	strcpy(&m_ini.def_file[0], strout.GetBuffer());

	m_thread = m_pnet_cfg->GetThreadCount();
	m_tick   = m_pnet_cfg->GetTick();

	for (int i = 0 ; i < sizeof(ct) / sizeof(struct con_thread ); i++ ) {
		ct[i].en_run = 1;
	}
	m_tmcon  = CTime::GetCurrentTime();
	g_con_success = 0;
	ReadySendThread(0, 0, 0);

	// 连接后3s检查连接是否成功，接着启动1s定时器，关闭3s定时器
	SetTimer(1, 3000, NULL);
	// 如果参数有变化，保存到ini文件
	if (strcmp(str_last, m_ini.def_file) != 0) {
		SysConfig dlg;
		dlg.SaveConfig(&m_ini);
	}
	return 0;
}
LRESULT CNetStrongDlg::StopThread(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("stop");

	for (int i = 0 ; i < sizeof(ct) / sizeof(struct con_thread ); i++ ) {
		ct[i].en_run = 0;
	}
	PauseTransfer(NULL, NULL);
	KillTimer(0);
	::SendNotifyMessage(m_pnet_cfg->m_hWnd, UM_NOTICE_STOP_THREAD, NULL, NULL);
	// 安全退出线程
	// 	DWORD ExitCode;
	// 	for (int i = 0 ;i < sizeof(ct)/sizeof(struct con_thread ); i++ ) {
	// 		ExitCode = 88;
	// 		while(1) {
	// 			GetExitCodeThread(ct[i].hThread,&ExitCode);
	// 			if (ExitCode == 0) {
	// 				break;
	// 			}
	// 			//Sleep(10);
	// 		}
	// 		//ExitThread(ct[i].thread_fake_id);
	// 		CloseHandle(ct[i].hThread);
	// 	}
	return 0;
}

LRESULT CNetStrongDlg::GoTransfer(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("go");
	int ret;
	CString strout;
	char str_last[256];

	strcpy(str_last, m_ini.def_file);
	ret =  m_pnet_cfg->GetFile(&strout);
	strcpy(m_ini.def_file, strout.GetBuffer());
	m_tick = m_pnet_cfg->GetTick();

	// 如果文件无效则不载入文件，
	if (ret == 1 ) {
		m_pnet_cfg->OnBnClickedBtnTransfer();
		return 0;
	}
	strcpy(ct[0].fname, m_ini.def_file);

	// 刷新启动发送时间
	if (wParam == 0) {
		m_tmsend  = CTime::GetCurrentTime();
		m_autotick = 0;
	}


	for (int i = 0 ; i < sizeof(ct) / sizeof(struct con_thread ); i++ ) {
		ct[i].en_send = 2;
		ct[i].sleep_tick = m_tick;
	}
	//m_pmsg_view->InputValid(TRUE);

	// 如果参数有变化，保存到ini文件
	if (strcmp(str_last, m_ini.def_file) != 0) {
		SysConfig dlg;
		dlg.SaveConfig(&m_ini);
	}

	SetTimer(0, 1000 , NULL);
	return 0;
}
LRESULT CNetStrongDlg::PauseTransfer(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("pause");
	for (int i = 0 ; i < sizeof(ct) / sizeof(struct con_thread ); i++ ) {
		ct[i].en_send = 0;
	}
	//m_pmsg_view->InputValid(FALSE);
	KillTimer(0);
	return 0;
}

LRESULT CNetStrongDlg::SelectThreadItem(WPARAM wParam, LPARAM lParam)
{
	CString strout;
	// 	strout.Format("%d", wParam);
	// 	MessageBox(strout);
	ViableSelectThread(wParam);
	return 0;
}

void CNetStrongDlg::InitLogPath(char *path)
{
	char tmppath[256];
	char *p;
	char *pfind;

	strcpy(tmppath, path);
	p = tmppath;
	while(p < tmppath + 200) {
		p = strstr(p, "\\");
		if (p == NULL) {
			break;
		}
		pfind = p;
		p++;
		*pfind = '\0';
		CreateDirectory(tmppath, NULL);
		*pfind = '\\';
	}
	CreateDirectory(tmppath, NULL);
}

void CNetStrongDlg::MailTitle(CString *str, bool emailformat)
{
	char host_name[256] = "127.0.0.1";
	struct hostent *netcard;
	int ret;
	char *pendline;
	if (emailformat == 1) {
		pendline = "<br>";
	}
	else {
		pendline = "";
	}

	// 当没有任何网络连接的情况下 gethostname 返回 -1
	ret = gethostname(host_name, sizeof(host_name) - 1);
	if (ret != 0) {
		return ;
	}
	str->Format("%s", host_name);
}


void CNetStrongDlg::LogoTitle(CString *str, bool emailformat)
{
	char host_name[256] = "127.0.0.1";
	struct hostent *netcard;
	int ret;
	char *pendline;
	if (emailformat == 1) {
		pendline = "<br>";
	}
	else {
		pendline = "";
	}

	// 当没有任何网络连接的情况下 gethostname 返回 -1
	str->Format("File Title%s\n", pendline);
	ret = gethostname(host_name, sizeof(host_name) - 1);
	if (ret != 0) {
		return ;
	}
	netcard = gethostbyname(host_name);

	str->Format(*str + "host name: %s%s\n", netcard->h_name, pendline);
	for (int i = 0; i < netcard->h_length; i++) {
		if(!netcard->h_addr_list[i]) {//rendb add
			break;
		}
		str->Format(*str + "ip %d :%s%s\n", i, inet_ntoa(*((struct in_addr *)netcard->h_addr_list[i])), pendline);
	}

	str->Format(*str + "coonnect time %2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d%s\n", m_tmcon.GetYear(), m_tmcon.GetMonth(), m_tmcon.GetDay(),
	            m_tmcon.GetHour(), m_tmcon.GetMinute(), m_tmcon.GetSecond(), pendline);
	str->Format(*str + "send time     %2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d%s\n", m_tmsend.GetYear(), m_tmsend.GetMonth(), m_tmsend.GetDay(),
	            m_tmsend.GetHour(), m_tmsend.GetMinute(), m_tmsend.GetSecond(), pendline);
	str->Format(*str + "save time     %2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d%s\n", m_tmsave.GetYear(), m_tmsave.GetMonth(), m_tmsave.GetDay(),
	            m_tmsave.GetHour(), m_tmsave.GetMinute(), m_tmsave.GetSecond(), pendline);
	str->Format(*str + "------------------------------------------------------%s\n", pendline);
}

void CNetStrongDlg::SaveLog(char *path, char *file)
{

	CString fileTitle;
	LogoTitle(&fileTitle, 0);
	FILE *fp = 0;
	CString strout;
	char *fname;
	DWORD len, ret;




	InitLogPath(path);

	for (int i = 0 ; i < sizeof(ct) / sizeof(struct con_thread ); i++ ) {
		strout.Format("%s\\%s-th%d.txt", path, file, i);
		fname = strout.GetBuffer();
		fp = fopen(fname, "wb");
		if (fp == NULL) {
			break;
		}
		strout.Format("%s", ct[i].prbuf);
		len = strout.GetLength();

		ret = fwrite(fileTitle.GetBuffer(), 1, fileTitle.GetLength(), fp);
		ret = fwrite(ct[i].prbuf, 1, len, fp);
		fclose(fp);
	}


}
LRESULT CNetStrongDlg::OnSaveLog(WPARAM wParam, LPARAM lParam)
{
	bool pause = false;

	if (ct[0].en_send == 1) {
		pause = true;
		PauseTransfer(NULL, NULL);
	}


	CString strout;
	char *fname;

	m_tmsave = CTime::GetCurrentTime();
	strout.Format("unname");
	fname = strout.GetBuffer();
	SaveLog(m_ini.log_path, fname);

	if (pause == true) {
		GoTransfer(1, NULL);
	}
	return 0;
}

void CNetStrongDlg::OnAutoMail()
{
#ifdef CONFIG_LIMIT
	// 受限版本之有修改ini文件才能发送一次邮件
	struct netst_config_def ini;
	SysConfig dlg;

	dlg.LoadConfig(&ini);
	if (ini.dbg_mail) {
		dlg.SaveConfig(&ini);
		MessageBox("邮件发送完毕");
		return;
	}
	else {
		MessageBox("只有收集元气弹的人才能使用该功能");
	}
#else
	OnAutoSave();
#endif
	
}
void CNetStrongDlg::OnAutoSave()
{
	bool pause = false;

	if (ct[0].en_send == 1) {
		pause = true;
		PauseTransfer(NULL, NULL);
	}


	CString strout;
	
	m_tmsave = CTime::GetCurrentTime();
#if 1
	// Debug
	static int k = 0;

	// 受限版本
#ifdef CONFIG_LIMIT
	if (k > CONFIG_MAX_AUTO_MAIL) {
		goto _UnMail;
	}
#endif

	strout.Format("auto-%d-%d-%d_%d_%d-%d", m_tmsave.GetYear(), m_tmsave.GetMonth(), m_tmsave.GetDay(),
	              m_tmsave.GetHour(), m_tmsave.GetMinute(), k++);
#else
	strout.Format("auto-%d-%d-%d_%d_%d", m_tmsave.GetYear(), m_tmsave.GetMonth(), m_tmsave.GetDay(),
	              m_tmsave.GetHour(), m_tmsave.GetMinute());
#endif
	strcpy(m_autofile, strout.GetBuffer());
	SaveLog(m_ini.log_path, m_autofile);

	SendToEmail();
	OnDataClear();
_UnMail:;
	if (pause == true) {
		GoTransfer(1, NULL);
	}
}

LRESULT CNetStrongDlg::ChangeConfig(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CNetStrongDlg::OnConfigDefault()
{
	SysConfig dlg;


	dlg.LoadConfig(&m_ini);
	dlg.InitData(&m_ini);
	if (IDOK == dlg.DoModal()) {

		// 数据有改变才保存
		if (dlg.CheckDataChange(&m_ini) ) {
			m_pnet_cfg->InitData(&m_ini);
		}
		dlg.SaveConfig(&m_ini);

	}
	else {

	}
}

void CNetStrongDlg::OnSaveAs()
{
	//MessageBox("OnSaveAs");
	bool pause = false;
	CString strpath, strname;

	if (ct[0].en_send == 1) {
		pause = true;
		PauseTransfer(NULL, NULL);
	}

	static TCHAR BASED_CODE szFilter[] =
		"Text File(*.txt)|*.txt|"
		"All File(*.*)|*.*||";
	CFileDialog SaveDialog(false,NULL,"",OFN_NOCHANGEDIR |OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);

	if(IDOK !=SaveDialog.DoModal()) {
		goto _UNSAVE;
	}
	m_tmsave = CTime::GetCurrentTime();
	strpath.Format("%s", SaveDialog.GetFolderPath());
	strname.Format("%s", SaveDialog.GetFileName());
	SaveLog(strpath.GetBuffer(), strname.GetBuffer());

_UNSAVE:

	if (pause == true) {
		GoTransfer(1, NULL);
	}
}

void CNetStrongDlg::OnAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();

}

void CNetStrongDlg::OnVerConfig()
{
	VerConfig dlg;
	dlg.DoModal();

}

void CNetStrongDlg::OnSave()
{
	//MessageBox("OnSave");
	m_pnet_cfg->OnBnClickedBtnSave();
}
void CNetStrongDlg::OnDataClear()
{
	//MessageBox("OnDataClear");
	bool pause = false;

	// 如果现在正在发送数据，暂停，并稍等片刻等待对方发送数据完毕
	if (ct[0].en_send == 1) {
		pause = true;
		PauseTransfer(NULL, NULL);

		Sleep(1000);
	}

	// 清除缓存

	for (int i = 0; i < sizeof(ct) / sizeof(struct con_thread ); i++) {
		ct[i].rbuf_use = 0;
	}

	m_pmsg_view->UpdateString(ct[m_select_thread].prbuf, 0);
	// 如果之前已经启动，则再次启动
	if (pause) {
		GoTransfer(1, NULL);
	}
}

void CNetStrongDlg::OnTray()
{
	OnTrayClear(NULL, NULL);
}


void CNetStrongDlg::ViableSelectThread(DWORD val)
{
	// 检查用户双击list返回的指定行是否越界
	if (val >= sizeof(ct) / sizeof(struct con_thread )) {
		return ;
	}
	m_select_thread = val;
	//m_pmsg_view->UpdateString(ct[m_select_thread].prbuf, 0);

	GetThreadLog();
}
void CNetStrongDlg::GetThreadLog()
{
	CString strout;

	//	strout.Format("%s", ct[m_select_thread].prbuf);
	//strout.Format("%s", ct[2].prbuf);
	//	m_pmsg_view->UpdateString(strout.GetBuffer(), 0);
	char *pbuf;
	char tmp;
	DWORD index ;

	pbuf = ct[m_select_thread].prbuf;
	index = ct[m_select_thread].rbuf_use;
	if (index > 0) {
		index = index - 1;
	}
	tmp = *(pbuf + index);
	*(pbuf + index) = '\0';
	//strout.Format("%d\n",ct[m_select_thread].rbuf_use);
	//ct[m_select_thread].en_send = 0;
	m_pmsg_view->UpdateString(ct[m_select_thread].prbuf, 0);
	//ct[m_select_thread].en_send = 1;
	*(pbuf + index) = tmp;

}

LRESULT CNetStrongDlg::OnTrayClear(WPARAM   wp,   LPARAM   lp)
{
	static bool show = true;
	m_nWnd.cbSize  =  sizeof(NOTIFYICONDATA);
	m_nWnd.hWnd  =  m_hWnd;
	m_nWnd.hIcon  =  m_hIcon;
	m_nWnd.uID  =  IDR_MAINFRAME;
	m_nWnd.uFlags =  NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_nWnd.uCallbackMessage  =  WM_MY_TRAY_NOTIFICATION;
	strcpy(m_nWnd.szTip, "网络强壮测试");
	ShowWindow(SW_HIDE);
	Shell_NotifyIcon(NIM_ADD, &m_nWnd);
	return 0;
}

LRESULT CNetStrongDlg::OnTrayNotification(WPARAM   wp,   LPARAM   lp)
{
	if(lp == WM_LBUTTONDOWN) {
		ShowWindow(SW_NORMAL);
		Shell_NotifyIcon(NIM_DELETE, &m_nWnd);
	}
	return 0;
}

void CNetStrongDlg::SendNullEmail()
{
	CString path, mailContent, subject, strTitle;
	char host_name[256] = "127.0.0.1";
	CTime tm;
	int times = 0, ret;
	tm = CTime::GetCurrentTime();
	CString str;


	// 邮件附件
	deque<string> file;
	// 建立一个无用的文件，绕过发送邮件的BUG——必须发送附件
	SaveLog("D:\\netstrong", "unuse");
	for (int i = 0; i < sizeof(ct) / sizeof(struct con_thread ); i++) {
		str.Format("D:\\netstrong\\unuse-th0.txt");
		file.push_back(str.GetBuffer());
	}

	// 邮件内容
	CString strContext;
	LogoTitle(&strContext, 1);
	// strContext.Format("-sdf <br> sdfsdf<br>lll");
	// 邮件标题
	subject.Format("首次运行提示防火墙%d.%d.%d %d:%d:%d", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());

	MailTitle(&strTitle, 1);
	strTitle.Format("%s" + strTitle, EMAIL_TITLE);
	// strContext.Format("-sdf <br> sdfsdf<br>lll");
	// 邮件标题
	subject.Format("首次运行提示防火墙%d.%d.%d %d:%d:%d", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	do {
		times++;
		ret = SendMail("smtp.163.com", m_ini.s_mail, "ZGV2MjAxNm1haWwxMjM=", m_ini.s_mail, m_ini.d_mail, strTitle, "rname", subject, strContext, file);
	}
	while(ret != 0 && times < 5); //小于5次发送不成功就继续

}


void CNetStrongDlg::SendToEmail()
{
	CString path, mailContent, subject, strTitle;
	char host_name[256] = "127.0.0.1";
	CTime tm;
	int times = 0, ret;
	tm = CTime::GetCurrentTime();
	CString str;


	// 邮件附件
	deque<string> file;

	// 	file.push_back("C:\\notebook\\NoteBookTrack.txt");
	// 	file.push_back("C:\\notebook\\notebook1.jpg");
	// 	file.push_back("C:\\notebook\\notebook2.jpg");
	// 	file.push_back("C:\\notebook\\notebook3.jpg");

	//CString strSubDir;
	// 	str.Format("%s\\NoteBookTrack.txt",strSubDir);
	// 	file.push_back(str.GetBuffer());
	// 	str.Format("%s\\notebook1.jpg",strSubDir);
	// 	file.push_back(str.GetBuffer());
	// 	str.Format("%s\\notebook2.jpg",strSubDir);
	// 	file.push_back(str.GetBuffer());
	// 	str.Format("%s\\notebook3.jpg",strSubDir);
	// 	file.push_back(str.GetBuffer());
	for (int i = 0; i < sizeof(ct) / sizeof(struct con_thread ); i++) {
		str.Format("%s\\%s-th%d.txt", m_ini.log_path, m_autofile, i);
		file.push_back(str.GetBuffer());
	}

	// 邮件内容
	CString strContext;
	LogoTitle(&strContext, 1);
	MailTitle(&strTitle, 1);
	strTitle.Format("%s" + strTitle, EMAIL_TITLE);
	// strContext.Format("-sdf <br> sdfsdf<br>lll");
	// 邮件标题
	subject.Format("网络测试%d.%d.%d %d:%d:%d %s %s", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond(),
		m_ini.ip, m_autofile);

	do {
		times++;
		ret = SendMail("smtp.163.com", m_ini.s_mail, "ZGV2MjAxNm1haWwxMjM=", m_ini.s_mail, m_ini.d_mail, strTitle.GetBuffer(), "rname", subject, strContext, file);
	}
	while(ret != 0 && times < 5); //小于5次发送不成功就继续

}

DWORD SendMail(//不使用hlp.dll里面的SendEMail
    CString SmtpAddr,	// smtp服务器地址
    CString Account,		// 帐号
    CString Password,	// 密码
    CString Sendfrom,	// 发送源地址
    CString Sendto,		// 目地地址
    CString SenderName,	// 发送者姓名
    CString ReceiverName,// 接收者姓名
    CString Subject,		// 邮件主题
    CString Content,		// 邮件正文
    deque<string> m_dqFiles
)
{

	//deque<string> m_dqFiles;
	bool	m_IsHtml = 1;

	ZSmtp smtp;				// smtp对像
	CString strSmtpAddr;	// smtp服务器地址
	CString strAccount;		// 帐号
	CString strPassword;	// 密码
	CString strSendfrom;	// 发送源地址
	CString strSendto;		// 目地地址
	CString strSenderName;	// 发送者姓名
	CString strReceiverName;// 接收者姓名
	CString strSubject;		// 邮件主题
	CString strContent;		// 邮件正文


	strSmtpAddr = SmtpAddr;//"smtp.gmail.com";
	if(!smtp.Connect((LPSTR)(LPCTSTR)strSmtpAddr, 25)) {
		//AfxMessageBox("连接服务器失败!"); return ;
		return 1;
	}
	strAccount = Account;
	strPassword = Password;
#ifdef CONFIG_BASE64_PWD
	if(!smtp.ValidateBase64((LPSTR)(LPCTSTR)strAccount, (LPSTR)(LPCTSTR)strPassword)) {
		//AfxMessageBox("用户名或密码失败!"); return ;
		return 1;
	}
#else
	if(!smtp.Validate((LPSTR)(LPCTSTR)strAccount, (LPSTR)(LPCTSTR)strPassword)) {
		//AfxMessageBox("用户名或密码失败!"); return ;
		return 1;
	}
#endif
	ZString zstr;
	strSendto = Sendto;
	deque<string> strMails = zstr.Splitter((LPSTR)(LPCTSTR)strSendto, ",");

	strSendfrom = Sendfrom;
	strSenderName = SenderName;
	strReceiverName = ReceiverName;
	strSubject = Subject;//"user other source code";
	strContent = Content;//"cfpwd";

	if(!smtp.SendData((LPSTR)(LPCTSTR)strSendfrom,
	                  strMails,
	                  (LPSTR)(LPCTSTR)strSenderName,
	                  (LPSTR)(LPCTSTR)strReceiverName,
	                  (LPSTR)(LPCTSTR)strSubject,
	                  (LPSTR)(LPCTSTR)strContent,
	                  m_dqFiles,
	                  m_IsHtml)) {
		TRACE("发送失败\n");
		return 1;

	}
	else {
		TRACE("发送成功\n");

		return 0;
	}
}

// 先调用WSAStartup，否则gethostname不成功
int CNetStrongDlg::VirsionRequest()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
		return -1;
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {
		WSACleanup( );
		return -1; 
	}
	return 0;	
}