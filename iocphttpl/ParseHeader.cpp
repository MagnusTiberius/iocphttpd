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
	headermap_t *item = NULL;
	int previous_token = NULL;
	int token = Token();
	while (token != token_t::ENDTOKEN)
	{
		token = Token();
		if (token == ParseHeader::token_t::GET || token == ParseHeader::token_t::POST)
		{
			item = new headermap_t{};
			item->name = _strdup("METHOD");
			item->value = _strdup(m_token);
			m_headermap.push_back(item);
			item = NULL;
		}
		if (token == ParseHeader::token_t::URL)
		{
			item = new headermap_t{};
			item->name = _strdup("URL");
			item->value = _strdup(m_token);
			m_headermap.push_back(item);
			item = NULL;
		}
		if (token == ParseHeader::token_t::HTTPVERSION)
		{
			item = new headermap_t{};
			item->name = _strdup("VERSION");
			item->value = _strdup(m_token);
			m_headermap.push_back(item);
			item = NULL;
		}
		if (token == ParseHeader::token_t::PROPERTYNAME)
		{
			item = new headermap_t{};
			item->name = _strdup(m_token);

			CHAR *v = AcceptUntil("\n");

			item->value = _strdup(v);
			m_headermap.push_back(item);
			item = NULL;

			Backup();
			previous_token = token;
			continue;
		}
		if (token == '\n')
		{
			printf("NewLine\n");
			if (previous_token == '\n')
			{
				printf("Content Follows Next\n");
			}
			previous_token = token;
		}
		if (token >= 32)
		{
			previous_token = token;
		}
	}
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
	c1 = AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\/-0123456789");
	if (c1 != NULL)
	{
		m_token = _strdup(c1);
		return ParseHeader::token_t::URL;
	}
	int end_marker = m_pos;
	m_pos = begin_marker;
	const CHAR *c = Next();
	return *c;
}