#pragma once
#include <windows.h>

#define PIPE_BUF_SIZE   1024
#define  PIPENAME       L"\\\\.\\pipe\\CanastaPipe"

class PipeServer
{
public:
	PipeServer();
	~PipeServer();

	int CreatePipe();
	int ClosePipe();
	int ReadPipe(char *retbuf);
	int IsReadable();
	int WritePipe(char *write, int writelen);
	int IsWritable();
private:
	HANDLE m_hPipe;
	BOOL m_bConnected;

	int ProcessIO(char *read);
};

class PipeClient
{
public:
	PipeClient();
	~PipeClient();

	int ConnectPipe();
	int ReadPipe(char *retbuf);
	int IsReadable();
	int WritePipe(char *write, int writelen);
	int IsWritable();
private:
	HANDLE m_hPipe;
	BOOL m_bConnected;
};