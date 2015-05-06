#include "SocketIocpController.h"


SocketIocpController::SocketIocpController()
{

	m_DataBuffer = (LPSOCKET_IO_DATA)malloc(sizeof(SOCKET_IO_DATA));

	for (int i = 0; i < BUFSIZMIN; i++)
	{
		m_ActiveFlag[i] = false;
		LPSOCKET_IO_DATA rv = &m_DataBuffer[i];
		//ZeroMemory(&(rv->operationData.Overlapped), sizeof(OVERLAPPED));
		//rv->operationData.Buffer = NULL;
		//rv->operationData.BytesRECV = 0;
		//rv->operationData.BytesSEND = 0;
		//rv->operationData.LPBuffer = NULL;
		rv->operationData.DataBuf.buf = NULL;
		rv->operationData.DataBuf.len = 0;
		rv->handleData.Socket = 0;
	}

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	m_DataBufferSize = BUFSIZMIN;
}


SocketIocpController::~SocketIocpController()
{
	::CloseHandle(ghMutex);
}


SocketIocpController::LPSOCKET_IO_DATA SocketIocpController::Allocate()
{
	::WaitForSingleObject(ghMutex, INFINITE);
	for (int i = 0; i < m_DataBufferSize; i++)
	{
		if (m_ActiveFlag[i] == false)
		{
			LPSOCKET_IO_DATA rv = &m_DataBuffer[i];
			ZeroMemory(&(rv->operationData.Overlapped), sizeof(OVERLAPPED));
			rv->operationData.Buffer = NULL;
			rv->operationData.BytesRECV = 0;
			rv->operationData.BytesSEND = 0;
			rv->operationData.LPBuffer = NULL;
			rv->operationData.DataBuf.buf = NULL;
			rv->operationData.DataBuf.len = 0;
			rv->handleData.Socket = 0;
			m_ActiveFlag[i] = true;
			::ReleaseMutex(ghMutex);
			return rv;
		}
	}
	::ReleaseMutex(ghMutex);
	return NULL;
}