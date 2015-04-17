#include "stdafx.h"
#include "ParseHeader.h"


ParseHeader::ParseHeader()
{
	token = NULL;
}


ParseHeader::~ParseHeader()
{
}

void ParseHeader::Parse()
{
	ParseHeader::token_t content_marker = UNDEFINED;
	ParseHeader::token_t prev_token = UNDEFINED;
	ParseHeader::token_t token = Token();
	while (token != token_t::ENDTOKEN)
	{
		token = Token();
		if (token == ParseHeader::token_t::PROPERTYNAME)
		{
			CHAR *s = AcceptUntil("\n");
		}
		if (token == ParseHeader::token_t::NEWLINE)
		{
			if (token == ParseHeader::token_t::NEWLINE)
			{
				content_marker = ParseHeader::token_t::CONTENTFOLLOWSNEXT;
			}
		}
		prev_token = ParseHeader::token_t::NEWLINE;
	}
}

ParseHeader::token_t ParseHeader::Token()
{
	CHAR *c1 = NULL;
	CHAR *d1 = NULL;
	SkipEmpty();
	if (token != NULL)
	{
		free(token);
		token = NULL;
	}
	c1 = AcceptRun("\n");
	if (c1 != NULL)
	{
		d1 = _strdup(c1);
		if (strcmp(d1, "\n") == 0)
		{
			token = d1;
			return ParseHeader::token_t::NEWLINE;
		}
	}
	c1 = AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	if (c1 != NULL)
	{
		d1 = _strdup(c1);
		if (strcmp(d1, "GET") == 0)
		{
			token = d1;
			return ParseHeader::token_t::GET;
		}
		if (strcmp(d1, "POST") == 0)
		{
			token = d1;
			return ParseHeader::token_t::POST;
		}
		if (strcmp(d1, "HTTP") == 0)
		{
			c1 = AcceptRun("\/.0123456789");
			return ParseHeader::token_t::HTTPVERSION;
		}
		c1 = AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-:");
		if (c1 != NULL)
		{
			return ParseHeader::token_t::PROPERTYNAME;
		}
	}
	c1 = AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\/-0123456789");
	if (c1 != NULL)
	{
		token = _strdup(c1);
		return ParseHeader::token_t::URL;
	}
}

bool ScannerA::IsEmpty()
{
	char c = *m_c;
	if (c == '\t' || c < '\r' || c < '\0' || c < ' ' || c == 32)
	{
		return true;
	}
	return false;
}