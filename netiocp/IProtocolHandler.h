#pragma once

namespace NETIOCP
{

	class IProtocolHandler
	{
	public:
		IProtocolHandler();
		~IProtocolHandler();

	public:
		virtual void HandleMessage(char* msg);
		virtual void HandleMessage(char* msg, char*& reply) = 0;
	};

}