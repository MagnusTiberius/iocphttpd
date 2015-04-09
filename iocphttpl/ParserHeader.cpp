#include "ParserHeader.h"


ParserHeader::ParserHeader()
{
}


ParserHeader::~ParserHeader()
{
}


void ParserHeader::Parse(const char* content)
{
	char buf1[DATA_BUFSIZE];
	ZeroMemory(buf1, DATA_BUFSIZE);
	int i = 0;
	char c = content[i];
	int k = i;
	while (c != '\0')
	{
		if (c == '\r')
		{
			c = content[++i];
			continue;
		}
		if (c == '\n')
		{
			ParseLine(buf1);
			ZeroMemory(buf1, DATA_BUFSIZE);
			k = 0;
		}
		buf1[k++] = c;
		i++;
		c = content[i];
	}

}

void ParserHeader::ParseLine(char *content)
{
	char buf1[DATA_BUFSIZE];
	ZeroMemory(buf1, DATA_BUFSIZE);
	int i = 0;
	char c = content[i];
	int k = i;
	while (c != '\0')
	{
		if (c == ' ')
		{
			if (strcmp(buf1, "GET") == 0)
			{
				m_methodType = HTTP_GET;
				m_ps = PSTATE_GET;
			}
			if (strcmp(buf1, "POST") == 0)
			{
				m_methodType = HTTP_POST;
				m_ps = PSTATE_POST;
			}
			c = content[++i];
			continue;
		}
		buf1[k++] = c;
		i++;
		c = content[i];
	}
}