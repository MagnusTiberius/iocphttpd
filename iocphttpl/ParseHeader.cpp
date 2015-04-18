#include "stdafx.h"
#include "ParseHeader.h"


ParseHeader::ParseHeader()
{
	m_token = NULL;
}


ParseHeader::~ParseHeader()
{
}

void ParseHeader::Parse()
{
	DWORD dwThreadId = GetCurrentThreadId();
	Reset();
	headermap_t *item = NULL;
	int previous_token = NULL;
	int token = Token();
	while (token != token_t::ENDTOKEN)
	{
		printf(".");
		token = Token();
		if (token == ParseHeader::token_t::GET || token == ParseHeader::token_t::POST)
		{
			item = new headermap_t{};
			item->name = _strdup("METHOD");
			item->value = _strdup(m_token);
			printf("%d::Parse-POST: %s=%s \n", dwThreadId, item->name, item->value);
			m_headermap.push_back(item);
			item = NULL;
		}
		if (token == ParseHeader::token_t::URL)
		{
			item = new headermap_t{};
			item->name = _strdup("URL");
			item->value = _strdup(m_token);
			printf("%d::Parse-URL: %s=%s \n", dwThreadId, item->name, item->value);
			m_headermap.push_back(item);
			item = NULL;
		}
		if (token == ParseHeader::token_t::HTTPVERSION)
		{
			item = new headermap_t{};
			item->name = _strdup("VERSION");
			item->value = _strdup(m_token);
			//printf("%d::Parse-HTTPVERSION: %s=%s \n", dwThreadId, item->name, item->value);
			m_headermap.push_back(item);
			item = NULL;
		}
		if (token == ParseHeader::token_t::QUERYSTRING)
		{
			item = new headermap_t{};
			item->name = _strdup("QUERYSTRING");
			item->value = _strdup(m_token);
			//printf("%d::Parse-QUERYSTRING: %s=%s \n", dwThreadId, item->name, item->value);
			m_headermap.push_back(item);
			item = NULL;
		}
		
		if (token == ParseHeader::token_t::PROPERTYNAME)
		{
			item = new headermap_t{};
			item->name = _strdup(m_token);

			CHAR *v = AcceptUntil("\n");

			item->value = _strdup(v);
			//printf("%d::Parse-PROPERTYNAME: %s=%s \n", dwThreadId, item->name, item->value);
			m_headermap.push_back(item);
			item = NULL;

			Backup();
			previous_token = token;
			continue;
		}
		if (token == '\n')
		{
			printf("%d::NewLine\n", dwThreadId);
			if (previous_token == '\n')
			{
				printf("%d::Content Follows Next\n", dwThreadId);
				token = token_t::ENDTOKEN;
				//ParseContent();
				continue;
			}
			previous_token = token;
		}
		if (token > 32)
		{
			previous_token = token;
		}
	}
}

void ParseHeader::ParseContent()
{
	CHAR *buf;
	HEADERMAPLIST::iterator itr;
	for (itr = m_headermap.begin(); itr != m_headermap.end(); itr++)
	{
		lpheadermap_t pitem = *itr;
		std::string s = std::string(pitem->name);
		if (s.compare("Content-Length:") == 0)
		{
			std::string n = std::string(pitem->value);
			int len = atoi(pitem->value)+1;
			buf = (CHAR*)malloc(len * sizeof(&pitem->value[0]));
			memset(buf, 0, len);
			//const CHAR *s = StripOut(pitem->value, "\r\n\t");
			const CHAR *p = &m_str[m_pos+1];
			strncpy_s(buf, len, p, len);
			headermap_t *item = NULL;
			item = new headermap_t{};
			item->name = _strdup("Content:");
			item->value = buf;
			m_headermap.push_back(item);
			item = NULL;
			memset(buf, 0, len);
		}
	}
}

const CHAR* ParseHeader::StripOut(CHAR* item, const CHAR *strip)
{
	std::string rv = "";
	int len = strlen(item);
	for (int i= 0; i < len; i++)
	{
		CHAR c = item[i];
		int striplen = strlen(strip);
		bool ismatch = false;
		for (int j = 0; j < striplen; j++)
		{
			CHAR u = strip[i];
			if (c == u)
			{
				ismatch = true;
			}
		}
		if (!ismatch)
		{
			rv.append(_strdup(&c));
		}
	}
	return rv.c_str();
}

int ParseHeader::Token()
{
	CHAR *c1 = NULL;
	CHAR *d1 = NULL;
	if (m_token != NULL)
	{
		free(m_token);
		m_token = NULL;
	}
	std::string value = "";
	int begin_marker = m_pos;
	c1 = AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	if (c1 != NULL)
	{
		d1 = _strdup(c1);
		value.append(d1);
		if (strcmp(d1, "GET") == 0)
		{
			m_token = d1;
			m_token = _strdup("GET");
			return ParseHeader::token_t::GET;
		}
		if (strcmp(d1, "POST") == 0)
		{
			m_token = d1;
			m_token = _strdup("POST");
			return ParseHeader::token_t::POST;
		}
		if (strcmp(d1, "HTTP") == 0)
		{
			c1 = AcceptRun("\/.0123456789");
			value.append(c1);
			m_token = _strdup(value.c_str());
			return ParseHeader::token_t::HTTPVERSION;
		}
		c1 = AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-:");
		if (c1 != NULL)
		{
			value.append(c1);
			m_token = _strdup(value.c_str());
			return ParseHeader::token_t::PROPERTYNAME;
		}
	}
	c1 = AcceptRun("\/");
	if (c1 != NULL)
	{
		printf("Slash detected.............................\n");
		Backup();
		c1 = AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\/-0123456789._");
		if (c1 != NULL)
		{
			m_token = _strdup(c1);
			printf("URL ====> %s\n", c1);
			return ParseHeader::token_t::URL;
		}
	}
	c1 = AcceptRun("\?\&ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_=0123456789");
	if (c1 != NULL)
	{
		value.append(c1);
		m_token = _strdup(value.c_str());
		return ParseHeader::token_t::QUERYSTRING;
	}
	int end_marker = m_pos;
	m_pos = begin_marker;
	const CHAR *c = Next();
	if (c == NULL)
	{
		return ParseHeader::token_t::ENDTOKEN;
	}
	return *c;
}

CHAR* ParseHeader::GetUrl()
{
	DWORD dwThreadId = GetCurrentThreadId();
	HEADERMAPLIST::iterator itr;
	for (itr = m_headermap.begin(); itr != m_headermap.end(); itr++)
	{
		lpheadermap_t pitem = *itr;
		std::string s = std::string(pitem->name);
		//printf("%d::GetUrl  %s\n", dwThreadId, s.c_str());
		if (s.compare("URL") == 0)
		{
			printf("GetUrl Found %s\n", pitem->value);
			return pitem->value;
		}
	}
	printf("GetUrl Not Found \n");
	return NULL;
}

MethodType ParseHeader::GetMethod()
{
	HEADERMAPLIST::iterator itr;
	for (itr = m_headermap.begin(); itr != m_headermap.end(); itr++)
	{
		lpheadermap_t pitem = *itr;
		std::string s = std::string(pitem->name);
		if (s.compare("METHOD") == 0)
		{
			if (strcmp(pitem->value, "GET") == 0)
			{
				return MethodType::HTTP_GET;
			}
			if (strcmp(pitem->value, "POST") == 0)
			{
				return MethodType::HTTP_POST;
			}
		}
	}
	return MethodType::HTTP_NONE;
}

void ParseHeader::Reset()
{
	HEADERMAPLIST::iterator itr;
	for (itr = m_headermap.begin(); itr != m_headermap.end(); itr++)
	{
		lpheadermap_t pitem = *itr;
		free(pitem->name);
		pitem->name = NULL;
		free(pitem->value);
		pitem->value = NULL;
		free(pitem);
		pitem = NULL;
	}
	m_headermap.clear();
}