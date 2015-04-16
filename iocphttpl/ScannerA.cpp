#include "ScannerA.h"


ScannerA::ScannerA()
{
	m_start = 0;
	m_end = 0;
	m_pos = 0;
}


ScannerA::~ScannerA()
{
}


void ScannerA::Input(CHAR* str)
{
	m_str = str;
	CHAR *lpC = &m_str[m_pos];
	m_c = lpC;
}


void ScannerA::Backup()
{
	m_pos = m_pos - 1;
	m_c = &m_str[m_pos];
}

CHAR* ScannerA::Next()
{
	m_pos = m_pos + 1;
	m_c = &m_str[m_pos];

	return m_c;
}

void ScannerA::SkipEmpty()
{
	bool inLoop = false;

	Next();
	if (!IsEmpty())
	{
		Backup();
		return;
	}

	while (IsEmpty())
	{
		Next();
		inLoop = true;
	}
	
}

CHAR* ScannerA::Peek()
{
	CHAR *c = Next();
	Backup();
	return c;
}

bool ScannerA::Accept(CHAR *str)
{
	auto chr = strchr(str, *m_c);
	if (chr != NULL)
	{
		return true;
	}
	return false;
}

CHAR* ScannerA::AcceptRun(CHAR *str)
{
	bool inLoop = false;
	memset(m_token, '\0', BUFSIZTOK);
	
	m_start = m_pos;
	CHAR *p = &m_str[m_start];

	while (Accept(str))
	{
		Next();
		inLoop = true;
	}

	if (inLoop)
	{
		Backup();
	}
	else
	{
		return NULL;
	}

	m_end = m_pos;
	strncpy_s(m_token, BUFSIZTOK, p, m_end - m_start + 1);
	p = NULL;

	return m_token;
} 

bool ScannerA::IsEmpty()
{
	char c = *m_c;
	if (c == '\t' || c < '\n' || c < '\r' || c < '\0' || c < ' ' || c == 32)
	{
		return true;
	}
	return false;
}


bool ScannerA::IsNumeric()
{
	if (*m_c > '0' && *m_c < '9')
	{
		return true;
	}
	return false;
}

bool ScannerA::IsAlphabetic()
{
	if ((*m_c > 'a' && *m_c < 'z') || (*m_c > 'A' && *m_c < 'Z'))
	{
		return true;
	}
	return false;
}

bool ScannerA::IsAlphanumeric()
{
	if (IsNumeric() || IsAlphabetic())
	{
		return true;
	}
	return false;
}