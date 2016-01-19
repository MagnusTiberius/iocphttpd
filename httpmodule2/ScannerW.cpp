#include "ScannerW.h"


ScannerW::ScannerW()
{
}


ScannerW::~ScannerW()
{
}

void ScannerW::Input(LPWSTR str)
{
	m_str = str;
}

size_t ScannerW::Input(LPCSTR str)
{
	const size_t siz = strlen(str) + 1;
	m_str = new WCHAR[siz];
	memset(m_str, '\0', siz);
	size_t nsiz;
	mbstowcs_s(&nsiz, m_str, siz, str, siz);
	return nsiz;
}

void ScannerW::Backup()
{
	m_pos = m_pos - 1;
	m_c = &m_str[m_pos];
}

WCHAR* ScannerW::Next()
{
	m_pos = m_pos + 1;
	WCHAR *lpC = &m_str[m_pos];
	m_c = lpC;
	return lpC;
}

WCHAR* ScannerW::Peek()
{
	WCHAR *c = Next();
	Backup();
	return c;
}

bool ScannerW::Accept(LPWSTR str)
{
	auto chr = wcschr(str, *m_c);
	if (chr != NULL)
	{
		return false;
	}
	return true;
}

bool ScannerW::AcceptRun(LPWSTR str)
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

bool ScannerW::IsEmpty()
{
	if (*m_c == L'\t' || *m_c < L'\n' || *m_c < L'\r' || *m_c < L'\0' || *m_c < L' ')
	{
		return true;
	}
	return false;
}

bool ScannerW::IsNumeric()
{
	if (*m_c > L'0' && *m_c < L'9')
	{
		return true;
	}
	return false;
}

bool ScannerW::IsAlphabetic()
{
	if ((*m_c > L'a' && *m_c < L'z') || (*m_c > L'A' && *m_c < L'Z'))
	{
		return true;
	}
	return false;
}

bool ScannerW::IsAlphanumeric()
{
	if (IsNumeric() || IsAlphabetic())
	{
		return true;
	}
	return false;
}