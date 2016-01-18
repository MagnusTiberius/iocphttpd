#pragma once
#include <winsock2.h>
#include "stdafx.h"


#include "Ws2tcpip.h"

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_COUNT       1
#define DEFAULT_PORT        5150
#define DEFAULT_BUFFER      2048
#define DEFAULT_MESSAGE     "\'A test message from client\'"
#define LOCALHOST			"localhost"


class SocketClient
{
public:
	SocketClient();
	~SocketClient();

private:
	SOCKET        sClient;
	WSADATA       wsd;
	char          szBuffer[DEFAULT_BUFFER];
	int           ret, i;
	struct sockaddr_in server;
	struct hostent    *host = NULL;

	char  szServer[128],                          // Server to connect to
		szMessage[1024];                      // Message to send to sever
	int   iPort = DEFAULT_PORT;    // Port on server to connect to
	DWORD dwCount = DEFAULT_COUNT; // Number of times to send message
	BOOL  bSendOnly = FALSE;                        // Send data only; don't receive


	void Usage();
	void ValidateArgs(int argc, char **argv);
	//int Run(int argc, char **argv);

public:

	void Send(const char *msg);
	void Send(const char *msg, char*& reply);
	int Connect();
	int SocketClient::Connect(int portNum);
	void Close();

};

