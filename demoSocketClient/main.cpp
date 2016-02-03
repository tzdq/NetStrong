//////////////////////////////////////////////////////////////////////////
/*************************************************************************
文件说明：
	简单cs程序：客户端
	客户端与服务器实现双向文件传输
格式为:
	数据包长度是定长的2000字节，有些大了
	0 - 4  5 - 8  9 - 1999
	Cmd    Size    FullInf
使用方法：
	输入可执行文件目录下需要发送的文件名hello zepp.mp3或send.txt，在另一端目录下查看。
作者：DreagonWoo
联系：83746227@qq.com
维护记录：
	在原来几个基本程序的基础上添加异常处理，这也只是某工程文件传输的模块代码
	本模块的不足之处，如只能连接一次客户端,客户端只连接环回地址,发送端不能很快
	的接收到对方断开连接的异常（不懂处理）等问题，在工程里解决。传输大的文件出现大	小不一致，或大或小
*************************************************************************/
#include "stdafx.h"



#include "stdio.h"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib");

int VirsionRequest();
int CreateSocket(SOCKET *skt);
int ConnectSocket(SOCKET *skt,const u_short port,const char *IpDest);
int ListenSocket(SOCKET *skt,const u_short port,u_short limit);
int SendSocket(SOCKET *skt,u_char *buf,u_int len, int *err);
int ReceiveSocket(SOCKET *skt,u_char *buf,u_int len, int *err);
int ErrorSocket();

DWORD WINAPI RecvThread(LPVOID lpParameter);
bool CompareCmd(unsigned char* buf1, unsigned char* buf2, int len);

#define LEN_PACK     1460                         //数据包长度
#define LEN_CMD      5                            //数据包头（命令）长度
#define LEN_SIZE     (sizeof(long))
#define LEN_FULLINF  (LEN_PACK-LEN_CMD - LEN_SIZE)//4个字节表示数据包里信息长度

u_char tbuf[LEN_PACK] = {0x55,0xaa,0x10,0x20,0x33};
u_char rbuf[LEN_PACK] = {0};
u_char fname[LEN_FULLINF] = {0}; 

u_char CmdFileHead[LEN_CMD]     = {0xaa,0x55,0x00,0x00,0x01};//文件开头
u_char CmdFileContent[LEN_CMD]  = {0xaa,0x55,0x00,0x00,0x02};//文件内容
u_char CmdFileEnd[LEN_CMD]      = {0xaa,0x55,0x00,0x00,0x04};//文件结束

#include "netstrongtype.h"

extern int g_con_success;

void ChangeID(u_char *pdata, unsigned long len, unsigned long id,
			  unsigned long thread_id)
{
	struct test_netpacket *pkt;
	pkt = (struct test_netpacket *)(pdata + 36);
	pkt->id = id;
	pkt->thread_id = thread_id;
	pkt->time[19] = '\0';
	pkt->time[0] = '\0';
	
#ifdef _WINDOWS
	CTime tm;
	CString strout;
	tm = CTime::GetCurrentTime();
	strout.Format("%d-%d-%d %d:%d:%d",tm.GetYear(),tm.GetMonth(),tm.GetDay(),
		tm.GetHour(),tm.GetMinute(),tm.GetSecond());
	strcpy(pkt->time, strout.GetBuffer());
#endif
	
}
DWORD WINAPI OneConnect(LPVOID lpParameter)
{
	struct con_thread *ct = (struct con_thread*)lpParameter;

	unsigned long thread_id = GetCurrentThreadId();
	VirsionRequest();
	//建立客户端
	//SOCKET client;
	struct recv_thread rt;
	char remoteIP[17];

	rt.prbuf = ct->prbuf;
	rt.rbuf_len = ct->rbuf_len;
	rt.rbuf_use = &ct->rbuf_use;
_WaitForIP:
	// 	printf("输入远方IP：");
	// 	scanf("%16s",remoteIP);
	//strcpy(remoteIP,"192.168.1.252");
	strcpy(remoteIP,ct->d_ip);
	CreateSocket(&rt.client);
	printf("向服务器：%s：%d 发出连接请求...\n",remoteIP, ct->d_port);
	//if(ConnectSocket(&rt.client,ct->d_port,remoteIP) == 10061) {
	if(ConnectSocket(&rt.client,ct->d_port,remoteIP)) {
		//system("pause");
		//exit(0);
		rt.en_run = 0;
		g_con_success = 0;
		return 1;
	}
	g_con_success = 1;
	ct->sockfd = rt.client;

	printf("连接成功\n\n");
	
	//建立接收线程
	unsigned long tID = 1;
	HANDLE hThread;
	DWORD ExitCode;	
	rt.en_run = 1;
	hThread = CreateThread(0,0x1000,RecvThread,(void*)&rt,0,&tID);


	//处理文件传输过程
	FILE *fd;
	long lenFile,lenInf;//文件长度和数据包纯数据长度
	u_char pdata[120*1024];


	FILE *fp;
	unsigned long flen;
	DWORD pack_id = 0;
_ReloadFile:;

	//fp = fopen("netpacket/10k.bin", "rb");
	fp = fopen(ct->fname, "rb");
	if (fp == NULL) {
		goto _SendFile;
	}
	
	fseek(fp, 0, SEEK_END);
	flen = ftell(fp);	
	fseek(fp, 0, SEEK_SET);

	printf("flen %d\n", flen);
	//pdata = (u_char*)malloc(flen);
	fread(pdata, 1, flen, fp);
	fclose(fp);
	// 	for (int i = 0; i < flen; i++) {
	// 		printf("%2.2x ", *(pdata + i));
	// 	}
	int err;
	int havesend, sendbyte;
	int packet;
	int i;

_SendFile:;
	err = 0;

	while(1) {
		// 		ret = SendSocket(
		// 			&client,
		// 			(u_char*)(pdata+0),
		// 			flen, 
		// 			&err);
		// 		if (ret == SOCKET_ERROR) {
		// 			printf("error\n");
		// 		}
		// 		else {
		// 			havesend = ret;
		// 		}
		// 		//printf("havesend = %d err %d\n", havesend, err);
		// 		Sleep(10);
		// 		continue;

		if (ct->en_run == 0) {
			break;
		}
		if (ct->en_send == 0) {
			Sleep(1000);
			continue;
		}
		// 再次载入文件，文件改变
		else if (ct->en_send == 2) {
			ct->en_send = 1;
			if (fp) {
				fclose(fp);
			}
			goto _ReloadFile;

		}
		havesend = 0;
		//ChangeID(pdata, flen, i, thread_id );
		ChangeID(pdata, flen, pack_id, ct->thread_fake_id);
		
		while(havesend < flen) {
			if (flen - havesend  >= 10000) {
				packet = 10000;
			}
			else {
				packet = flen - havesend;
			}


			err = 0;
			sendbyte = SendSocket(
				&rt.client,
				(u_char*)(pdata+havesend),
				packet, 
				&err);
			havesend += sendbyte;
			ct->s_byte += sendbyte;


			switch(err) {
			case 10038:
			case 10053://异常
				goto _Close;
				break;
			}
			//printf("havesend = %d err %d\n", havesend, err);
			Sleep(10);
		}
		pack_id++;
		Sleep(10);
		Sleep(ct->sleep_tick);
	}
_Close:;
	g_con_success = 0;
	rt.en_run = 0;
	ExitCode = 88;
	closesocket(rt.client);
	while(1) {
		GetExitCodeThread(hThread,&ExitCode);
		if (ExitCode == 0) {
			break;
		}
		Sleep(100);
	}
 	Sleep(1000);
	ExitThread(tID);
	CloseHandle(hThread);
	
	return 0;
}

void main()
{
	unsigned long tID = 1;
	struct con_thread ct[1];

	memset(&ct, 0, sizeof(struct con_thread));

	for (int i = 0 ;i < sizeof(ct)/sizeof(struct con_thread ); i++ ) {
		//strcpy(ct[i].d_ip,"192.168.1.251");
		strcpy(ct[i].d_ip,"192.168.1.251");
		ct[i].d_port = 6000;
		strcpy(ct[i].fname,"netpacket/10b.bin");
		ct[i].en_run = 1;
		ct[i].en_send = 1;
		ct[i].rbuf_len = 4*1024*1024;
		ct[i].thread_fake_id = i;
		ct[i].sleep_tick = 4000;
		ct[i].prbuf = (char*)malloc(ct[i].rbuf_len);
		if (ct[i].prbuf == NULL) {
			printf("no memory\n");
			system("pause");
			exit(0);
		}
		CreateThread(0,0x1000,OneConnect,(void*)&ct[i],0,&tID);
		printf("tid %d\n", tID);
	}
// 	Sleep(3000);
// 	ct[1].en_run = 0;
// 	Sleep(3000);
// 	ct[0].en_run = 0;

 	unsigned long total_send;
	
	while(1) {
		total_send = 0;
		for (int i = 0; i < sizeof(ct)/sizeof(struct con_thread ); i++) {
			total_send += ct[i].s_byte;
			ct[i].s_byte = 0;
		}
		//total_send = 1024*2;
		if (total_send > 1024 * 1024) {
			printf("\n%0.2fMB/s",(float)total_send / (1024*1024));
		}
		else if (total_send > 1024) {
			printf("\n%0.2fKB/s",(float)total_send / (1024));
		}
		else  {
			printf("\n%0.2fB/s",(float)total_send);
		}
		
		Sleep(1000);
	}
}
int ma2in(int argc, char* argv[])
{
	int ret = 0;
	
	//版本申请
	printf("Client\n");
	VirsionRequest();

	//建立客户端
	SOCKET client;
	char remoteIP[17];
	
_WaitForIP:
// 	printf("输入远方IP：");
// 	scanf("%16s",remoteIP);
	strcpy(remoteIP,"192.168.1.251");
	CreateSocket(&client);
	printf("向服务器：%s：1000发出连接请求...\n",remoteIP);
	if(ConnectSocket(&client,6000,remoteIP)) {
		system("pause");
		exit(0);
	}
	printf("连接成功\n\n");

	//建立接收线程
	unsigned long tID = 1;
	HANDLE hThread;
	DWORD ExitCode;	
	hThread = CreateThread(0,0x1000,RecvThread,(void*)&client,0,&tID);

	//处理文件传输过程
	FILE *fd;
	long lenFile,lenInf;//文件长度和数据包纯数据长度
	Sleep(3000);
	u_char pdata[120*1024];

	FILE *fp;
	unsigned long flen;
	
	fp = fopen("netpacket/100k.bin", "rb");

	fseek(fp, 0, SEEK_END);
	flen = ftell(fp);	
	fseek(fp, 0, SEEK_SET);

	printf("flen %d\n", flen);
	//pdata = (u_char*)malloc(flen);
	fread(pdata, 1, flen, fp);
	fclose(fp);
// 	for (int i = 0; i < flen; i++) {
// 		printf("%2.2x ", *(pdata + i));
// 	}
	int err;
	int havesend;
	int packet;
	err = 0;

	for (int i = 0; i < 10; i++) {
// 		ret = SendSocket(
// 			&client,
// 			(u_char*)(pdata+0),
// 			flen, 
// 			&err);
// 		if (ret == SOCKET_ERROR) {
// 			printf("error\n");
// 		}
// 		else {
// 			havesend = ret;
// 		}
// 		//printf("havesend = %d err %d\n", havesend, err);
// 		Sleep(10);
// 		continue;
	
		havesend = 0;
		
		while(havesend < flen) {
			if (flen - havesend  >= 10000) {
				packet = 10000;
			}
			else {
				packet = flen - havesend;
			}
			err = 0;
			havesend += SendSocket(
					&client,
					(u_char*)(pdata+havesend),
					packet, 
					&err);
// 			if (err == 10038) {
// 				break;
// 			}
			//printf("havesend = %d err %d\n", havesend, err);
			Sleep(10);
		}
		
	}

	
	
	Sleep(2000);
	///free(pdata);

	//return 0;
	while(ret != 10054 && ret != 10038) {
		printf("输入要发送的文件名:\n");
		gets((char*)fname);
		GetExitCodeThread(hThread,&ExitCode);
		if(ExitCode == 0) {
			printf("对方断开连接\n");
			closesocket(client);
			goto _WaitForIP;
		}
		if(*fname == '\0')
			continue;
		if( (fd = fopen((char*)fname,"rb")) == NULL) {//二进制方式打开，不然会出现接收端忽略换行符（回车符不记得）的错误
			printf("当前目录没找到文件%s!\n",fname);
			continue;
		}
		
		fseek(fd,0,SEEK_END);
		lenFile = ftell(fd);
		printf("文件长度%d字节\n",lenFile);
		fseek(fd,0,SEEK_SET);

		//构建和发送【文件信息头】数据包——暂时不关心数据包长度，packSize的位置留空
		memcpy(tbuf,CmdFileHead,LEN_CMD);                //构建数据包头
		memcpy(tbuf+LEN_CMD+LEN_SIZE,fname,LEN_FULLINF); //构建文件信息，这里只包含文件的名字，最好加上原文件大小（不是packSize，是fileSize）
		//ret = SendSocket(&client,tbuf,LEN_PACK);         //发送

		//构建和发送【文件内容】数据包
		memset(tbuf,0,LEN_PACK);
		while(lenFile > 0 && (ret != 10054 && ret != 10038) ) {//文件太大不能一次发送完成
			lenInf = LEN_FULLINF;                 //默认长度是满信息量
			if(lenFile < LEN_FULLINF) {           //文件小于一个数据包满信息量长度
				memset(tbuf,0,LEN_PACK);          //首先清空缓存，填不满数据包部分补0，由于其他的文件内容数据包由fread填满，不用memset
				lenInf = lenFile;                 //更具长度文件最后长度而定
			}
			memcpy(tbuf,CmdFileContent,LEN_CMD);  //加上数据包头
			memcpy(tbuf+LEN_CMD,&lenInf,LEN_SIZE);//数据包长度,注意inter存储方式是倒序的
			fread (tbuf+LEN_CMD+LEN_SIZE,sizeof(unsigned char),LEN_FULLINF,fd);//接着数据包内容，即使文件内容不足LEN_FULLINF也不会出错
			lenFile -= LEN_FULLINF;      
		//	ret = SendSocket(&client,tbuf,LEN_PACK);
		}

		//构建和发送【文件结束】数据包
		memcpy(tbuf,CmdFileEnd,LEN_CMD);          //加上数据包头
		//ret = SendSocket(&client,tbuf,LEN_PACK);
		if(fd)
			fclose(fd);
		printf("文件发送完成\n");
	}
	printf("退出\n");
	WSACleanup ();
	return 0;
}

DWORD WINAPI RecvThread(LPVOID lpParameter)
{
	long   ret = 0,lenInf;
	struct recv_thread *rt = (struct recv_thread *)lpParameter;
	SOCKET *skt = &rt->client;
	u_char recvFileName[LEN_PACK];//收到文件的名字
	FILE   *fd = NULL;
	int err = 0;
	int tlast_echo;
	int tmp = 0;//debug//////////
	printf("recv thread %d\n", rt->en_run);
	static int k = 0;

	*(rt->rbuf_use)  = 0;
	while(err != 10054 && err != 10053 && rt->en_run == 1) {
		
		++tmp;
		err = 0;
		ret = ReceiveSocket(skt,rbuf,LEN_PACK, &err);
		
		if (ret == 0) {
			Sleep(0);
		}
		else {
			k++;
			rbuf[ret] = '\0';
			//printf("%s", rbuf);
			tlast_echo = *(rt->rbuf_use);
			*(rt->rbuf_use) += _snprintf(
					rt->prbuf + *(rt->rbuf_use), 
					rt->rbuf_len - *(rt->rbuf_use),
					"%s",rbuf);
			*(rt->prbuf + *(rt->rbuf_use)  ) = '\0';
			printf("%s", rt->prbuf + tlast_echo);
			Sleep(10);
		}
		continue;

		
		lenInf = rbuf[LEN_CMD+3] << 24 | rbuf[LEN_CMD+2] << 16 | rbuf[LEN_CMD+1] << 8 | rbuf[LEN_CMD];//获取信息量长度
		
		if(CompareCmd(rbuf,CmdFileContent,LEN_CMD)) {   //*********文件内容*********
			fwrite(rbuf+LEN_CMD+LEN_SIZE,sizeof(u_char),lenInf,fd);
		}		
		else if(CompareCmd(rbuf,CmdFileHead,LEN_CMD)) { //*********文件信息头*********
			tmp = 0;//debug//////////
			memcpy(recvFileName,rbuf+LEN_CMD+LEN_SIZE,LEN_FULLINF);
			printf("开始接收一个文件:%s\n",recvFileName);
			fd = fopen((char*)recvFileName,"w");        //这种方式打开文件会把里面的内容清空，没文件则建立
			fclose(fd);
			fd = fopen((char*)recvFileName,"ab");       //建立一个文件，二进制方式打开，不然会出现接收端忽略换行符（回车符不记得）的错误
		}
		else if(CompareCmd(rbuf,CmdFileEnd,LEN_CMD)) {  //*********文件结束*********
			printf("文件%s接收完毕\n",recvFileName);
			fclose(fd);
		}
		else {
			printf("无法识别   %d\n",tmp);//debug//////////
		}
	}
	closesocket(*skt);//想在这通知主线程，让他跳出循环，这样就不用等到主线程发送一个数据后才跳出，怎么办？？？
					  //控制台在不建立虚拟窗口的情况下怎么发送消息，难道用SetConsoleCtrlHandler
	if(fd)
		fclose(fd);
	printf("连接中断\n");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//Compare the packet cmd
bool CompareCmd(unsigned char* buf1, unsigned char* buf2, int len)
{
	for(int i=0; i<len; i++)
		if(*(buf1+i) != *(buf2+i))
			return false;
		return true;
}

int VirsionRequest()
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
int CreateSocket(SOCKET *skt)
{
	*skt = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(*skt == INVALID_SOCKET) {
		printf("创建套接字错误\n");
		return ErrorSocket();
	}
	return 0;
}

int ConnectSocket(SOCKET *skt,const u_short port,const char *IpDest)
{
	SOCKADDR_IN addr;
	int ret = 0;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons (port); //Port to connect on
    addr.sin_addr.s_addr = inet_addr(IpDest); //Target IP
	
	unsigned long ul = 1;  
	timeval tm; 
	fd_set set;  
	
	tm.tv_sec  = 8;
	tm.tv_usec = 0;  
	FD_ZERO(&set);  
	FD_SET(*skt, &set);  
// 	ioctlsocket(*skt, FIONBIO, &ul); //设置为非阻塞模式  
// 	select(*skt, NULL, &set, NULL, &tm);
	if(connect(*skt,(sockaddr*)&addr,sizeof(sockaddr)) == SOCKET_ERROR) {
		printf("链接失败\n");
		ret =  ErrorSocket();
		if (ret == 10035) {
			ret = 0;
		}
	}
	ul = 0;
//	ioctlsocket(*skt, FIONBIO, &ul); //设置为阻塞模式  
	return ret;		
}

int ListenSocket(SOCKET *skt,const u_short port,u_short limit)
{	
	SOCKADDR_IN addr;
	
	addr.sin_family = AF_INET;      // Address family
    addr.sin_port = htons (port);
	addr.sin_addr.s_addr = htonl (INADDR_ANY);
	
	if(bind(*skt,(sockaddr*)&addr,sizeof(sockaddr))== SOCKET_ERROR) {
		printf("绑定失败错误\n");
		return ErrorSocket();
	}
	
	if(listen(*skt,limit) == SOCKET_ERROR ) {
		printf("侦听失败\n");
		return ErrorSocket();
	}
	return 0;
}

int SendSocket(SOCKET *skt,u_char *buf,u_int len, int *err)
{
	int ret = send(*skt,(char*)buf,len,0);
//	Sleep(0);
	if(ret == SOCKET_ERROR) {
		printf("发送失败\n");
		*err = ErrorSocket();
	}
	return ret;
}

int ReceiveSocket(SOCKET *skt,u_char *buf,u_int len, int *err)
{	
	int ret = recv(*skt,(char*)buf,len,0);
//	Sleep(0);
	if(ret == SOCKET_ERROR ) {
		printf("接收失败\n");
		*err = ErrorSocket();
	}
	return ret;
}
int ErrorSocket()
{
	int err = WSAGetLastError();
	
	if(err) //返回值不为0，有错误
		printf("错误码:%d\n",err);
	return err;
}
