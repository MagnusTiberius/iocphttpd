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


void ScannerA::Input(const CHAR* str)
{
	m_start = 0;
	m_end = 0;
	m_pos = 0;

	m_str = str;
	const CHAR *lpC = &m_str[m_pos];
	m_c = lpC;
}

bool ScannerA::IsEOS()
{
	bool b = (m_pos >= strlen(m_str));
	return b;
}

void ScannerA::Backup()
{
	m_pos = m_pos - 1;
	m_c = &m_str[m_pos];
}

const CHAR* ScannerA::Next()
{
	if (m_str == NULL)
		return NULL;

	if (m_pos >= strlen(m_str))
	{
		return NULL;
	}
	m_pos = m_pos + 1;
	m_c = &m_str[m_pos];

	return m_c;
}

void ScannerA::SkipEmpty()
{
	bool inLoop = false;

	if (!IsEmpty())
	{
		return;
	}

	Next();
	if (!IsEmpty())
	{
		return;
	}

	while (IsEmpty())
	{
		Next();
		inLoop = true;
	}

}

const CHAR* ScannerA::Peek()
{
	const CHAR *c = Next();
	if (c != NULL)
		Backup();
	return c;
}

bool ScannerA::Accept(CHAR *str)
{
	if (m_c == NULL)
		return false;

	char c = *m_c;
	auto chr = strchr(str, c);
	if (chr != NULL)
	{
		return true;
	}
	return false;
}

CHAR* ScannerA::AcceptUntil(CHAR *str)
{
	auto chr = strchr(str, *m_c);
	m_start = m_pos;
	const CHAR *p = &m_str[m_start];
	while (chr == NULL)
	{
		Next();
		chr = strchr(str, *m_c);
	}
	m_end = m_pos;
	strncpy_s(m_token, BUFSIZMIN, p, m_end - m_start + 1);
	return m_token;
}

CHAR* ScannerA::AcceptRun(CHAR *str)
{
	bool inLoop = false;
	memset(m_token, '\0', BUFSIZMIN);

	m_start = m_pos;
	const CHAR *p = &m_str[m_start];

	while (Accept(str))
	{
		const CHAR* c = Next();
		if (c == NULL)
		{
			return NULL;
		}
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
	strncpy_s(m_token, BUFSIZMIN, p, m_end - m_start + 1);
	p = NULL;
	Next();
	return m_token;
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