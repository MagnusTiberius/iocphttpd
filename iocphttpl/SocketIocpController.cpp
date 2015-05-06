#include "SocketIocpController.h"


SocketIocpController::SocketIocpController()
{

	int n = sizeof(SOCKET_IO_DATA);
	int siz = n * DATASIZMIN;

	for (int i = 0; i < DATASIZMIN; i++)
	{
		m_ActiveFlag[i] = 0;
		LPSOCKET_IO_DATA rv = &m_DataBuffer[i];
		ZeroMemory(&(m_DataBuffer[i].operationData.Overlapped), sizeof(OVERLAPPED));
		m_DataBuffer[i].operationData.sequence = i;
		m_DataBuffer[i].sequence = i;
		m_DataBuffer[i].handleData.Socket = 0;
	}

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	m_DataBufferSize = DATASIZMIN;
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
		if (m_ActiveFlag[i] == 0)
		{
			LPSOCKET_IO_DATA rv = &m_DataBuffer[i];
			ZeroMemory(&(rv->operationData.Overlapped), sizeof(OVERLAPPED));
			memset(rv->operationData.Buffer, 0, DATA_BUFSIZE);
			rv->operationData.BytesRECV = 0;
			rv->operationData.BytesSEND = 0;
			rv->operationData.LPBuffer = NULL;
			rv->operationData.DataBuf.buf = NULL;
			rv->operationData.DataBuf.len = 0;
			rv->handleData.Socket = 0;
			rv->sequence = i;
			rv->operationData.sequence = i;
			m_ActiveFlag[i] = 1;
			::ReleaseMutex(ghMutex);
			return rv;
		}
	}
	::ReleaseMutex(ghMutex);
	return NULL;
}

void SocketIocpController::Free(LPSOCKET_IO_DATA data)
{
	::WaitForSingleObject(ghMutex, INFINITE);
	if (m_ActiveFlag[data->sequence])
	{
		ZeroMemory(&(data->operationData.Overlapped), sizeof(OVERLAPPED));
		memset(data->operationData.Buffer, 0, DATA_BUFSIZE);
		data->operationData.BytesRECV = 0;
		data->operationData.BytesSEND = 0;
		data->operationData.LPBuffer = NULL;
		data->operationData.DataBuf.buf = NULL;
		data->operationData.DataBuf.len = 0;
		data->handleData.Socket = 0;
		m_ActiveFlag[data->sequence] = 0;
	}
	::ReleaseMutex(ghMutex);
}


void SocketIocpController::Free(int index)
{
	::WaitForSingleObject(ghMutex, INFINITE);
	LPSOCKET_IO_DATA rv = NULL;

	if (index < m_DataBufferSize && m_ActiveFlag[index])
	{
		rv = &m_DataBuffer[index];
		ZeroMemory(&(rv->operationData.Overlapped), sizeof(OVERLAPPED));
		memset(rv->operationData.Buffer, 0, DATA_BUFSIZE);
		rv->operationData.BytesRECV = 0;
		rv->operationData.BytesSEND = 0;
		rv->operationData.LPBuffer = NULL;
		rv->operationData.DataBuf.buf = NULL;
		rv->operationData.DataBuf.len = 0;
		rv->handleData.Socket = 0;
		m_ActiveFlag[rv->sequence] = 0;
	}
	::ReleaseMutex(ghMutex);
}

void SocketIocpController::FreeBySocket(SOCKET index)
{
	LPSOCKET_IO_DATA rv = NULL;
	for (int i = 0; i < m_DataBufferSize; i++)
	{
		if (m_ActiveFlag[i] == 1)
		{
			rv = &m_DataBuffer[i];
			if (rv->handleData.Socket != NULL)
			{
				if (rv->handleData.Socket == index)
				{
					Free(i);
				}
			}
		}
	}
}