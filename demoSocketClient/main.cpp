//////////////////////////////////////////////////////////////////////////
/*************************************************************************
�ļ�˵����
	��cs���򣺿ͻ���
	�ͻ����������ʵ��˫���ļ�����
��ʽΪ:
	���ݰ������Ƕ�����2000�ֽڣ���Щ����
	0 - 4  5 - 8  9 - 1999
	Cmd    Size    FullInf
ʹ�÷�����
	�����ִ���ļ�Ŀ¼����Ҫ���͵��ļ���hello zepp.mp3��send.txt������һ��Ŀ¼�²鿴��
���ߣ�DreagonWoo
��ϵ��83746227@qq.com
ά����¼��
	��ԭ��������������Ļ���������쳣������Ҳֻ��ĳ�����ļ������ģ�����
	��ģ��Ĳ���֮������ֻ������һ�οͻ���,�ͻ���ֻ���ӻ��ص�ַ,���Ͷ˲��ܺܿ�
	�Ľ��յ��Է��Ͽ����ӵ��쳣���������������⣬�ڹ����������������ļ����ִ�	С��һ�£�����С
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

#define LEN_PACK     1460                         //���ݰ�����
#define LEN_CMD      5                            //���ݰ�ͷ���������
#define LEN_SIZE     (sizeof(long))
#define LEN_FULLINF  (LEN_PACK-LEN_CMD - LEN_SIZE)//4���ֽڱ�ʾ���ݰ�����Ϣ����

u_char tbuf[LEN_PACK] = {0x55,0xaa,0x10,0x20,0x33};
u_char rbuf[LEN_PACK] = {0};
u_char fname[LEN_FULLINF] = {0}; 

u_char CmdFileHead[LEN_CMD]     = {0xaa,0x55,0x00,0x00,0x01};//�ļ���ͷ
u_char CmdFileContent[LEN_CMD]  = {0xaa,0x55,0x00,0x00,0x02};//�ļ�����
u_char CmdFileEnd[LEN_CMD]      = {0xaa,0x55,0x00,0x00,0x04};//�ļ�����

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
	//�����ͻ���
	//SOCKET client;
	struct recv_thread rt;
	char remoteIP[17];

	rt.prbuf = ct->prbuf;
	rt.rbuf_len = ct->rbuf_len;
	rt.rbuf_use = &ct->rbuf_use;
_WaitForIP:
	// 	printf("����Զ��IP��");
	// 	scanf("%16s",remoteIP);
	//strcpy(remoteIP,"192.168.1.252");
	strcpy(remoteIP,ct->d_ip);
	CreateSocket(&rt.client);
	printf("���������%s��%d ������������...\n",remoteIP, ct->d_port);
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

	printf("���ӳɹ�\n\n");
	
	//���������߳�
	unsigned long tID = 1;
	HANDLE hThread;
	DWORD ExitCode;	
	rt.en_run = 1;
	hThread = CreateThread(0,0x1000,RecvThread,(void*)&rt,0,&tID);


	//�����ļ��������
	FILE *fd;
	long lenFile,lenInf;//�ļ����Ⱥ����ݰ������ݳ���
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
		// �ٴ������ļ����ļ��ı�
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
			case 10053://�쳣
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
	
	//�汾����
	printf("Client\n");
	VirsionRequest();

	//�����ͻ���
	SOCKET client;
	char remoteIP[17];
	
_WaitForIP:
// 	printf("����Զ��IP��");
// 	scanf("%16s",remoteIP);
	strcpy(remoteIP,"192.168.1.251");
	CreateSocket(&client);
	printf("���������%s��1000������������...\n",remoteIP);
	if(ConnectSocket(&client,6000,remoteIP)) {
		system("pause");
		exit(0);
	}
	printf("���ӳɹ�\n\n");

	//���������߳�
	unsigned long tID = 1;
	HANDLE hThread;
	DWORD ExitCode;	
	hThread = CreateThread(0,0x1000,RecvThread,(void*)&client,0,&tID);

	//�����ļ��������
	FILE *fd;
	long lenFile,lenInf;//�ļ����Ⱥ����ݰ������ݳ���
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
		printf("����Ҫ���͵��ļ���:\n");
		gets((char*)fname);
		GetExitCodeThread(hThread,&ExitCode);
		if(ExitCode == 0) {
			printf("�Է��Ͽ�����\n");
			closesocket(client);
			goto _WaitForIP;
		}
		if(*fname == '\0')
			continue;
		if( (fd = fopen((char*)fname,"rb")) == NULL) {//�����Ʒ�ʽ�򿪣���Ȼ����ֽ��ն˺��Ի��з����س������ǵã��Ĵ���
			printf("��ǰĿ¼û�ҵ��ļ�%s!\n",fname);
			continue;
		}
		
		fseek(fd,0,SEEK_END);
		lenFile = ftell(fd);
		printf("�ļ�����%d�ֽ�\n",lenFile);
		fseek(fd,0,SEEK_SET);

		//�����ͷ��͡��ļ���Ϣͷ�����ݰ�������ʱ���������ݰ����ȣ�packSize��λ������
		memcpy(tbuf,CmdFileHead,LEN_CMD);                //�������ݰ�ͷ
		memcpy(tbuf+LEN_CMD+LEN_SIZE,fname,LEN_FULLINF); //�����ļ���Ϣ������ֻ�����ļ������֣���ü���ԭ�ļ���С������packSize����fileSize��
		//ret = SendSocket(&client,tbuf,LEN_PACK);         //����

		//�����ͷ��͡��ļ����ݡ����ݰ�
		memset(tbuf,0,LEN_PACK);
		while(lenFile > 0 && (ret != 10054 && ret != 10038) ) {//�ļ�̫����һ�η������
			lenInf = LEN_FULLINF;                 //Ĭ�ϳ���������Ϣ��
			if(lenFile < LEN_FULLINF) {           //�ļ�С��һ�����ݰ�����Ϣ������
				memset(tbuf,0,LEN_PACK);          //������ջ��棬������ݰ����ֲ�0�������������ļ��������ݰ���fread����������memset
				lenInf = lenFile;                 //���߳����ļ���󳤶ȶ���
			}
			memcpy(tbuf,CmdFileContent,LEN_CMD);  //�������ݰ�ͷ
			memcpy(tbuf+LEN_CMD,&lenInf,LEN_SIZE);//���ݰ�����,ע��inter�洢��ʽ�ǵ����
			fread (tbuf+LEN_CMD+LEN_SIZE,sizeof(unsigned char),LEN_FULLINF,fd);//�������ݰ����ݣ���ʹ�ļ����ݲ���LEN_FULLINFҲ�������
			lenFile -= LEN_FULLINF;      
		//	ret = SendSocket(&client,tbuf,LEN_PACK);
		}

		//�����ͷ��͡��ļ����������ݰ�
		memcpy(tbuf,CmdFileEnd,LEN_CMD);          //�������ݰ�ͷ
		//ret = SendSocket(&client,tbuf,LEN_PACK);
		if(fd)
			fclose(fd);
		printf("�ļ��������\n");
	}
	printf("�˳�\n");
	WSACleanup ();
	return 0;
}

DWORD WINAPI RecvThread(LPVOID lpParameter)
{
	long   ret = 0,lenInf;
	struct recv_thread *rt = (struct recv_thread *)lpParameter;
	SOCKET *skt = &rt->client;
	u_char recvFileName[LEN_PACK];//�յ��ļ�������
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

		
		lenInf = rbuf[LEN_CMD+3] << 24 | rbuf[LEN_CMD+2] << 16 | rbuf[LEN_CMD+1] << 8 | rbuf[LEN_CMD];//��ȡ��Ϣ������
		
		if(CompareCmd(rbuf,CmdFileContent,LEN_CMD)) {   //*********�ļ�����*********
			fwrite(rbuf+LEN_CMD+LEN_SIZE,sizeof(u_char),lenInf,fd);
		}		
		else if(CompareCmd(rbuf,CmdFileHead,LEN_CMD)) { //*********�ļ���Ϣͷ*********
			tmp = 0;//debug//////////
			memcpy(recvFileName,rbuf+LEN_CMD+LEN_SIZE,LEN_FULLINF);
			printf("��ʼ����һ���ļ�:%s\n",recvFileName);
			fd = fopen((char*)recvFileName,"w");        //���ַ�ʽ���ļ���������������գ�û�ļ�����
			fclose(fd);
			fd = fopen((char*)recvFileName,"ab");       //����һ���ļ��������Ʒ�ʽ�򿪣���Ȼ����ֽ��ն˺��Ի��з����س������ǵã��Ĵ���
		}
		else if(CompareCmd(rbuf,CmdFileEnd,LEN_CMD)) {  //*********�ļ�����*********
			printf("�ļ�%s�������\n",recvFileName);
			fclose(fd);
		}
		else {
			printf("�޷�ʶ��   %d\n",tmp);//debug//////////
		}
	}
	closesocket(*skt);//������֪ͨ���̣߳���������ѭ���������Ͳ��õȵ����̷߳���һ�����ݺ����������ô�죿����
					  //����̨�ڲ��������ⴰ�ڵ��������ô������Ϣ���ѵ���SetConsoleCtrlHandler
	if(fd)
		fclose(fd);
	printf("�����ж�\n");
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
		printf("�����׽��ִ���\n");
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
// 	ioctlsocket(*skt, FIONBIO, &ul); //����Ϊ������ģʽ  
// 	select(*skt, NULL, &set, NULL, &tm);
	if(connect(*skt,(sockaddr*)&addr,sizeof(sockaddr)) == SOCKET_ERROR) {
		printf("����ʧ��\n");
		ret =  ErrorSocket();
		if (ret == 10035) {
			ret = 0;
		}
	}
	ul = 0;
//	ioctlsocket(*skt, FIONBIO, &ul); //����Ϊ����ģʽ  
	return ret;		
}

int ListenSocket(SOCKET *skt,const u_short port,u_short limit)
{	
	SOCKADDR_IN addr;
	
	addr.sin_family = AF_INET;      // Address family
    addr.sin_port = htons (port);
	addr.sin_addr.s_addr = htonl (INADDR_ANY);
	
	if(bind(*skt,(sockaddr*)&addr,sizeof(sockaddr))== SOCKET_ERROR) {
		printf("��ʧ�ܴ���\n");
		return ErrorSocket();
	}
	
	if(listen(*skt,limit) == SOCKET_ERROR ) {
		printf("����ʧ��\n");
		return ErrorSocket();
	}
	return 0;
}

int SendSocket(SOCKET *skt,u_char *buf,u_int len, int *err)
{
	int ret = send(*skt,(char*)buf,len,0);
//	Sleep(0);
	if(ret == SOCKET_ERROR) {
		printf("����ʧ��\n");
		*err = ErrorSocket();
	}
	return ret;
}

int ReceiveSocket(SOCKET *skt,u_char *buf,u_int len, int *err)
{	
	int ret = recv(*skt,(char*)buf,len,0);
//	Sleep(0);
	if(ret == SOCKET_ERROR ) {
		printf("����ʧ��\n");
		*err = ErrorSocket();
	}
	return ret;
}
int ErrorSocket()
{
	int err = WSAGetLastError();
	
	if(err) //����ֵ��Ϊ0���д���
		printf("������:%d\n",err);
	return err;
}
