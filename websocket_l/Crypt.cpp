#include "Crypt.h"


Crypt::Crypt()
{

}


Crypt::~Crypt()
{

}


void Crypt::Hash(string value)
{
	string sbuf;
	sbuf.assign(WEBSOCKETKEY);
	sbuf.append(KEY);


	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
		printf("Error %x during CryptAcquireContext!\n", GetLastError());
		goto done;
	}
	// Create hash object.
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		printf("Error %x during CryptBeginHash!\n", GetLastError());
		goto done;
	}
	// Fill buffer with test data.
	for (i = 0; i < BUFFER_SIZE; i++) {
		pbBuffer[i] = (BYTE)i;
	}
	// Hash in buffer.
	if (!CryptHashData(hHash, (BYTE*)value.c_str(), value.size(), 0)) {
		printf("Error %x during CryptHashData!\n", GetLastError());
		goto done;
	}
	// Read hash value size and allocate memory.
	dwCount = sizeof(DWORD);
	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&dwHashLen,&dwCount, 0)) {
		printf("Error %x during reading hash size!\n", GetLastError());
		goto done;
	}
	if ((pbHash = (BYTE*)malloc(dwHashLen)) == NULL) {
		printf("Out of memory!\n");
		goto done;
	}
	ZeroMemory(pbHash, dwHashLen);
	// Read hash value.
	if (!CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0)) {
		printf("Error %x during reading hash value!\n", GetLastError());
		goto done;
	}
	// Print hash value.
	char buf[BUFFER_SIZE];
	ZeroMemory(buf, BUFFER_SIZE);

	for (i = 0; i < dwHashLen; i++) {
		sprintf_s(buf, "%s%2.2x ", buf, pbHash[i]);
	}
	printf("\n");
done:
	// Free memory.
	if (pbHash != NULL) free(pbHash);
	// Destroy hash object.
	if (hHash) CryptDestroyHash(hHash);
	// Release CSP handle.
	if (hProv) CryptReleaseContext(hProv, 0);
}

char* Crypt::HashIt(string value)
{
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	char *pbHash = NULL;
	DWORD dwHashLen;
	BYTE pbBuffer[BUFFER_SIZE];
	DWORD dwCount;
	DWORD i;

	char buf[BUFFER_SIZE];
	ZeroMemory(buf, BUFFER_SIZE);

	string sbuf;
	sbuf.assign(WEBSOCKETKEY);
	sbuf.append(KEY);

	char RetBuf[1024];
	ZeroMemory(RetBuf, 1024);

	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
		printf("Error %x during CryptAcquireContext!\n", GetLastError());
		goto done;
	}
	// Create hash object.
	if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) {
		printf("Error %x during CryptBeginHash!\n", GetLastError());
		goto done;
	}
	// Fill buffer with test data.
	for (i = 0; i < BUFFER_SIZE; i++) {
		pbBuffer[i] = (BYTE)i;
	}
	// Hash in buffer.
	if (!CryptHashData(hHash, (BYTE*)value.c_str(), value.size(), 0)) {
		printf("Error %x during CryptHashData!\n", GetLastError());
		goto done;
	}
	// Read hash value size and allocate memory.
	dwCount = sizeof(DWORD);
	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&dwHashLen, &dwCount, 0)) {
		printf("Error %x during reading hash size!\n", GetLastError());
		goto done;
	}
	if ((pbHash = (char*)malloc(dwHashLen)) == NULL) {
		printf("Out of memory!\n");
		goto done;
	}
	ZeroMemory(pbHash, dwHashLen);
	// Read hash value.
	if (!CryptGetHashParam(hHash, HP_HASHVAL, (BYTE*)pbHash, &dwHashLen, 0)) {
		printf("Error %x during reading hash value!\n", GetLastError());
		goto done;
	}
	// Print hash value.

	for (i = 0; i < dwHashLen; i++) {
		sprintf_s(buf, "%s%2.2x", buf, pbHash[i]);
	}
	printf("\n");
done:
	// Free memory.
	if (pbHash != NULL) free(pbHash);
	// Destroy hash object.
	if (hHash) CryptDestroyHash(hHash);
	// Release CSP handle.
	if (hProv) CryptReleaseContext(hProv, 0);
	//return string(_strdup(buf));
	return (char*)pbHash;
}


string Crypt::urlDecode(string str)
{
	string ret;
	char ch;
	int i, ii, len = str.length();

	for (i = 0; i < len; i++){
		if (str[i] != '%'){
			if (str[i] == '+')
				ret += ' ';
			else
				ret += str[i];
		}
		else{
			sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
	}
	return ret;
}

string Crypt::urlEncode(string str)
{
	string new_str = "";
	char c;
	int ic;
	const char* chars = str.c_str();
	char bufHex[10];
	int len = strlen(chars);

	for (int i = 0; i<len; i++){
		c = chars[i];
		ic = c;
		// uncomment this if you want to encode spaces with +
		/*if (c==' ') new_str += '+';
		else */if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
		else {
			sprintf(bufHex, "%X", c);
			if (ic < 16)
				new_str += "%0";
			else
				new_str += "%";
			new_str += bufHex;
		}
	}
	return new_str;
}