#pragma once
#include "stdafx.h"


class IOCPHTTPL_API ScannerA
{
public:
	ScannerA();
	~ScannerA();

	void Input(CHAR* str);
	bool Accept(CHAR* str);
	CHAR* AcceptRun(CHAR* str);
	void Backup();
	CHAR* Peek();
	CHAR* Next();
	bool IsNumeric();
	bool IsAlphabetic();
	bool IsAlphanumeric();
	bool IsEmpty();
	void SkipEmpty();

private:
	CHAR* m_str;
	int m_start;
	int m_end;
	int m_pos;
	CHAR *m_c;
	CHAR m_token[BUFSIZTOK];
};

/*
References:

1) http://cuddle.googlecode.com/hg/talk/lex.html#slide-31


*/