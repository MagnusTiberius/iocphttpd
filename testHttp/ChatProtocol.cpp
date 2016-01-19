#include "stdafx.h"
#include "ChatProtocol.h"


ChatProtocol::ChatProtocol()
{
}


ChatProtocol::~ChatProtocol()
{
}


void ChatProtocol::HandleMessage(char* msg)
{
	char* s = msg;
}

void ChatProtocol::HandleMessage(char* msg, char*& reply)
{
	char* s = msg;
	reply = _strdup("I got the message.\n");
}