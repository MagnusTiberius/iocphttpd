#include "SocketIocpController.h"


SocketIocpController::SocketIocpController()
{

	m_DataBuffer = (LPSOCKET_IO_DATA)malloc(sizeof(SOCKET_IO_DATA) * DATASIZMIN);

	for (int i = 0; i < DATASIZMIN; i++)
	{
		m_ActiveFlag[i] = false;
		LPSOCKET_IO_DATA rv = &m_DataBuffer[i];
		rv->sequence = i;
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

void SocketIocpController::Free(LPSOCKET_IO_DATA data)
{
	::WaitForSingleObject(ghMutex, INFINITE);
	if (m_ActiveFlag[data->sequence])
	{
		ZeroMemory(&(data->operationData.Overlapped), sizeof(OVERLAPPED));
		data->operationData.Buffer = NULL;
		data->operationData.BytesRECV = 0;
		data->operationData.BytesSEND = 0;
		data->operationData.LPBuffer = NULL;
		data->operationData.DataBuf.buf = NULL;
		data->operationData.DataBuf.len = 0;
		data->handleData.Socket = 0;
		m_ActiveFlag[data->sequence] = false;
	}
	::ReleaseMutex(ghMutex);
}


void SocketIocpController::Free(int index)
{
	::WaitForSingleObject(ghMutex, INFINITE);
	if (index < m_DataBufferSize && m_ActiveFlag[index])
	{
		LPSOCKET_IO_DATA rv = &m_DataBuffer[index];
		ZeroMemory(&(rv->operationData.Overlapped), sizeof(OVERLAPPED));
		rv->operationData.Buffer = NULL;
		rv->operationData.BytesRECV = 0;
		rv->operationData.BytesSEND = 0;
		rv->operationData.LPBuffer = NULL;
		rv->operationData.DataBuf.buf = NULL;
		rv->operationData.DataBuf.len = 0;
		rv->handleData.Socket = 0;
		m_ActiveFlag[index] = false;
	}
	::ReleaseMutex(ghMutex);
}