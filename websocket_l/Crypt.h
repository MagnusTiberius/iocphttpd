#pragma once
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#include "stdafx.h"

#include <wincrypt.h>

#define KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define BUFFER_SIZE 256
#define WEBSOCKETKEY  "dGhlIHNhbXBsZSBub25jZQ=="
#define WEBSOCKETKEY2 "+1bfVHYyBdlCWc4j0J27og=="

class Crypt
{
public:
	Crypt();
	~Crypt();


	void Hash(string value);
	static string HashIt(string value);
	static string urlEncode(string str);
	static string urlDecode(string str);

private:
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	BYTE *pbHash = NULL;
	DWORD dwHashLen;
	BYTE pbBuffer[BUFFER_SIZE];
	DWORD dwCount;
	DWORD i;
};

