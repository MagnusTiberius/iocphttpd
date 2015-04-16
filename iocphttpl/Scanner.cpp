#include "Scanner.h"


Scanner::Scanner()
{
	start = 0;
	m_pos = 0;
}


Scanner::~Scanner()
{
}


void Scanner::Input(LPWSTR str)
{
	m_str = str;
}

size_t Scanner::Input(LPCSTR str)
{
	const size_t siz = strlen(str) + 1;
	m_str = new WCHAR[siz];
	memset(m_str, '\0', siz);
	size_t nsiz;
	mbstowcs_s(&nsiz, m_str, siz, str, siz);
	return nsiz;
}

void Scanner::Backup()
{
	m_pos = m_pos - 1;
	m_c = &m_str[m_pos];
}

WCHAR* Scanner::Next()
{
	m_pos = m_pos + 1;
	WCHAR *lpC = &m_str[m_pos];
	m_c = lpC;
	return lpC;
}

WCHAR* Scanner::Peek()
{
	WCHAR *c = Next();
	Backup();
	return c;
}

bool Scanner::Accept(LPWSTR str)
{
	auto chr = wcschr(str, *m_c);
	if (chr != NULL)
	{
		return false;
	}
	return true;
}

bool Scanner::AcceptRun(LPWSTR str)
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


bool Scanner::IsNumeric()
{
	if (*m_c > L'0' && *m_c < L'9')
	{
		return true;
	}
	return false;
}

bool Scanner::IsAlphabetic()
{
	if ((*m_c > L'a' && *m_c < L'z') || (*m_c > L'A' && *m_c < L'Z'))
	{
		return true;
	}
	return false;
}

bool Scanner::IsAlphanumeric()
{
	if (IsNumeric() || IsAlphabetic())
	{
		return true;
	}
	return false;
}