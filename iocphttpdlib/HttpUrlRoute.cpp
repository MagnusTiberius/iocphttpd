#include "HttpUrlRoute.h"


HttpUrlRoute::HttpUrlRoute()
{
}


HttpUrlRoute::~HttpUrlRoute()
{
}

void HttpUrlRoute::AddRoute(char *url, void* lpFunc)
{
	ROUTEURL w = std::make_pair(url, lpFunc);
	urlRoutes.push_back(w);
}

void* HttpUrlRoute::GetRoute(char* url)
{
	for (ROUTEMAPITERATOR i = urlRoutes.begin(); i != urlRoutes.end(); ++i)
	{
		char* t_url = i->first;
		if (strcmp(url, t_url) == 0)
		{
			void* rv = i->second;
			return rv;
		}
	}
	return NULL;
}