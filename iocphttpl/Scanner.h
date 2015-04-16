#pragma once
#include "stdafx.h"


class Scanner
{
public:
	Scanner();
	~Scanner();

	void Input(LPWSTR str);
	bool Accept(LPWSTR str);
	bool AcceptRun(LPWSTR str);
	size_t Input(LPCSTR str);
	void Backup();
	WCHAR* Peek();
	WCHAR* Next();
	bool IsNumeric();
	bool IsAlphabetic();
	bool IsAlphanumeric();

private:
	LPWSTR m_str;
	int start;
	int m_pos;
	WCHAR *m_c;
};

/*
References:

1) http://cuddle.googlecode.com/hg/talk/lex.html#slide-31


*/