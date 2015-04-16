#include "ScannerA.h"


ScannerA::ScannerA()
{
	start = 0;
	m_pos = 0;
}


ScannerA::~ScannerA()
{
}


void ScannerA::Input(CHAR* str)
{
	m_str = str;
}


void ScannerA::Backup()
{
	m_pos = m_pos - 1;
	m_c = &m_str[m_pos];
}

CHAR* ScannerA::Next()
{
	m_pos = m_pos + 1;
	CHAR *lpC = &m_str[m_pos];
	m_c = lpC;
	return lpC;
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
		return false;
	}
	return true;
}

bool ScannerA::AcceptRun(CHAR *str)
{
	bool inLoop = false;
	
	while (Accept(str))
	{
		Next();
		inLoop = true;
	}

	if (inLoop)
		Backup();

	return true;
} 

bool ScannerA::IsEmpty()
{
	if (*m_c == '\t' || *m_c < '\n' || *m_c < '\r' || *m_c < '\0' || *m_c < ' ')
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