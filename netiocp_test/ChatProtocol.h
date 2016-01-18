#pragma once
#include "stdafx.h"
#include "IProtocolHandler.h"

class ChatProtocol : public IProtocolHandler
{
public:
	ChatProtocol();
	~ChatProtocol();


	void HandleMessage(char* msg);
	void HandleMessage(char* msg, char*& reply);
};

