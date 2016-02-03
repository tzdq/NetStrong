#ifndef _NET_STRONG_H_
#define _NET_STRONG_H_
struct con_thread
{
	int en_run;
	int en_send;
	int usecount;// 线程计数
	unsigned long s_byte;
	unsigned long r_byte;
	char d_ip[16];
	unsigned short d_port;
	char fname[256];
	unsigned long thread_fake_id;

	unsigned long sleep_tick;

	char *prbuf;
	unsigned long rbuf_len;
	unsigned long rbuf_use;
	HANDLE hThread;
	SOCKET sockfd;
};
struct recv_thread
{
	int en_run;
	SOCKET client;

	char *prbuf;
	unsigned long rbuf_len;
	unsigned long *rbuf_use;
};

struct test_netpacket
{
	char fname[16];
	unsigned long save;
	unsigned long id;
	unsigned long thread_id;
	char time[20];
};

DWORD WINAPI OneConnect(LPVOID lpParameter);
#endif