#pragma once
#include "stdafx.h"

class IOCPHTTPL_API ScannerW
{
public:
	ScannerW();
	~ScannerW();

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
	bool IsEmpty();

private:
	LPWSTR m_str;
	int start;
	int m_pos;
	WCHAR *m_c;
};

