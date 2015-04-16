#pragma once
#include "stdafx.h"


class ScannerA
{
public:
	ScannerA();
	~ScannerA();

	void Input(CHAR* str);
	bool Accept(CHAR* str);
	bool AcceptRun(CHAR* str);
	void Backup();
	CHAR* Peek();
	CHAR* Next();
	bool IsNumeric();
	bool IsAlphabetic();
	bool IsAlphanumeric();
	bool IsEmpty();

private:
	CHAR* m_str;
	int start;
	int m_pos;
	CHAR *m_c;
};

/*
References:

1) http://cuddle.googlecode.com/hg/talk/lex.html#slide-31


*/