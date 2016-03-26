#pragma once
#include "stdafx.h"
#include <map>
#include <vector>
#include <regex>


namespace IOCPHTTPL
{
	class IOCPHTTPL_API WebSocketRoute
	{
	public:
		WebSocketRoute();
		~WebSocketRoute();

		typedef std::pair<char*, void*> ROUTEURL;
		typedef std::vector<ROUTEURL> ROUTEMAP;
		typedef ROUTEMAP::iterator ROUTEMAPITERATOR;

		void AddWebSocketRoute(char *url, void* lpFunc);
		void DispatchWebSocket(char* message, char* reply);

		typedef /*static*/ void(*LPSTATICFUNCWEBSOCKET)(char* message, char* reply);

	protected:
		ROUTEMAP urlRoutes;

	};

}