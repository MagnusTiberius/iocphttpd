#include "WebSocketRoute.h"

namespace IOCPHTTPL
{

	WebSocketRoute::WebSocketRoute()
	{
	}


	WebSocketRoute::~WebSocketRoute()
	{
	}

	void WebSocketRoute::AddWebSocketRoute(char *url, void* lpFunc)
	{
		ROUTEURL w = std::make_pair(url, lpFunc);
		urlRoutes.push_back(w);
	}

	void WebSocketRoute::DispatchWebSocket(char* message, char* reply)
	{
		if (urlRoutes.size() == 0)
			return;

		for (ROUTEMAPITERATOR i = urlRoutes.begin(); i != urlRoutes.end(); ++i)
		{
			char* t_url = i->first;
			LPSTATICFUNCWEBSOCKET lpFunc = (LPSTATICFUNCWEBSOCKET)i->second;
			if (lpFunc != NULL)
			{
				(*lpFunc)(message, reply);
			}
			int a = 1;
		}
	}

}