#include "UrlParam.h"


UrlParam::UrlParam()
{
}


UrlParam::~UrlParam()
{
}

bool UrlParam::IsMatching(char *path, char* urlRoute)
{
	const DWORD BUFSZ = 2000;
	char buf[BUFSZ];
	DWORD dwSize = strlen(urlRoute);
	urlParams.clear();

	int j = 0;
	int i = 0;
	bool good = false;
	for (i = 0; i < dwSize; i++)
	{
		char c1 = path[j++];
		char c2 = urlRoute[i];
		if (c2 == '<')
		{
			good = true;
			while (c2 != '/')
			{ 
				i++;
				c2 = urlRoute[i];
			}
			int k = 0;
			memset(buf, 0, BUFSZ);
			while (c1 != '/')
			{
				buf[k++] = c1;
				c1 = path[j++];
			}
			urlParams.push_back(_strdup(buf));
			int b = 1;
		}
		if (c1 != c2)
		{
			return false;
		}
		if (c2 != '<')
		{
			if (c1 == c1)
			{
				
			}
			else
			{
				return false;
			}
		}

	}

	if (dwSize == i && good)
	{
		return true;
	}

	return false;
}

void UrlParam::Parse(char *path, char* url, std::vector<std::string> *list)
{
	if (IsMatching(path, url))
	{
		list->assign(urlParams.begin(), urlParams.end()); 
	}
}

void UrlParam::find_and_replace(string& source, string const& find, string const& replace)
{
	for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;)
	{
		source.replace(i, find.length(), replace);
		i += replace.length();
	}
}