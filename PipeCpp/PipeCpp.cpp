#include "PipeCpp.h"
#include <stdio.h>

/**********************************************************************************/
/* Pipe Server                                                                    */
/**********************************************************************************/
PipeServer::PipeServer()
{
}

PipeServer::~PipeServer()
{
}

int PipeServer::CreatePipe()
{
	while (true) {
		BYTE sd[SECURITY_DESCRIPTOR_MIN_LENGTH];
		SECURITY_ATTRIBUTES sa;

		sa.nLength = sizeof(sa);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = &sd;

		InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&sd, TRUE, (PACL)0, FALSE);

		m_hPipe = CreateNamedPipe(
			PIPENAME,
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			PIPE_BUF_SIZE,
			PIPE_BUF_SIZE,
			0,
			&sa);
		if (m_hPipe == INVALID_HANDLE_VALUE) {
			m_hPipe = NULL;
			m_bConnected = FALSE;
			return false;
		}
		m_bConnected = ConnectNamedPipe(m_hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (m_bConnected)
		{
			char readbuf[PIPE_BUF_SIZE];
			ReadPipe(readbuf);
			ProcessIO(readbuf);
			WritePipe(readbuf, strlen(readbuf));

			FlushFileBuffers(m_hPipe);
			DisconnectNamedPipe(m_hPipe);
			CloseHandle(m_hPipe);
		}
		else {
			CloseHandle(m_hPipe);
		}
	}

	return 0;
}

int PipeServer::ClosePipe() {
	FlushFileBuffers(m_hPipe);
	DisconnectNamedPipe(m_hPipe);
	CloseHandle(m_hPipe);
	return 0;
}

int PipeServer::ReadPipe(char *retbuf)
{
	if (!m_bConnected)
		return -1;

	char szBuf[PIPE_BUF_SIZE] = { 0, };
	DWORD dwRead = 0;

	bool bResult = ReadFile(
		m_hPipe,             // handle to pipe 
		szBuf,               // buffer to receive data 
		PIPE_BUF_SIZE,	     // size of buffer 
		&dwRead,             // number of bytes read 
		NULL);               // not overlapped I/O 
	if (!bResult || dwRead == 0) {
		return -2;
	}
	memcpy_s(retbuf, PIPE_BUF_SIZE, szBuf, dwRead);
	retbuf[dwRead] = 0;

	return 0;
}

int PipeServer::IsReadable() {
	return 0;
}

int PipeServer::WritePipe(char *write, int writelen) {
	if (!m_bConnected)
		return -1;

	char *buf = new char[writelen + 1];
	memcpy_s(buf, writelen + 1, write, writelen);
	buf[writelen] = '\r';
	DWORD dwWritten = 0;
	bool bResult = WriteFile(
		m_hPipe,
		buf,
		writelen+1,
		&dwWritten,
		NULL);
	if (!bResult || dwWritten != writelen+1) {
		return -2;
	}
	return 0;
}

int PipeServer::IsWritable() {
	return 0;
}

int PipeServer::ProcessIO(char *read) {
	//TBD: FIX HERE AS WHAT YOU WANT
	printf("%s\n", read);
	return 0;
}

/**********************************************************************************/
/* Pipe Client                                                                    */
/**********************************************************************************/

PipeClient::PipeClient() {

}
PipeClient::~PipeClient() {
	CloseHandle(m_hPipe);
}

int PipeClient::ConnectPipe() {
	m_hPipe = CreateFile(
		PIPENAME,       // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		0,              // default attributes 
		NULL);          // no template file 
	if (m_hPipe == INVALID_HANDLE_VALUE) {
		m_hPipe = NULL;
		m_bConnected = false;

		return false;
	}
	m_bConnected = true;

	return true;
}

int PipeClient::ReadPipe(char *retbuf) {
	if (!m_bConnected)
		return -1;
	DWORD dwRead;
	char szBuf[PIPE_BUF_SIZE] = { 0, };

	BOOL bResult = ReadFile(
		m_hPipe,          // handle to pipe 
		szBuf,            // buffer to receive data 
		PIPE_BUF_SIZE,	  // size of buffer 
		&dwRead,          // number of bytes read 
		NULL);            // not overlapped I/O 
	if (!bResult || dwRead == 0) {
		free(szBuf);
		return -2;
	}
	memset(retbuf, 0, PIPE_BUF_SIZE);
	memcpy_s(retbuf, PIPE_BUF_SIZE, szBuf, dwRead);
	
	return 0;
}

int PipeClient::IsReadable() {
	return 0;
}

int PipeClient::WritePipe(char *write, int writelen) {
	if (!m_bConnected)
		return -1;
	DWORD dwWritten;

	char *buf = new char[writelen + 2];
	memset(buf, 0, writelen + 1);
	memcpy_s(buf, writelen + 1, write, writelen);
	buf[writelen] = '\r';
	buf[writelen + 1] = 0;
	BOOL bResult = WriteFile(
		m_hPipe,
		buf,
		writelen+2,
		&dwWritten,
		NULL);
	if (!bResult || dwWritten != writelen+2) {
		return -2;
	}
	
	return 0;
}
int PipeClient::IsWritable() {
	return 0;
}