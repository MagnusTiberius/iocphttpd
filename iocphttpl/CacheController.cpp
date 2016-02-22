#include "CacheController.h"


CacheController::CacheController()
{
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
}


CacheController::~CacheController()
{
	::CloseHandle(ghMutex);

	for (CACHELIST::iterator i = m_cacheList.begin(); i != m_cacheList.end(); ++i)
	{
		LPCACHEITEM item = *i;
		free(item->code);
		free(item->filename);
		free(item->pcontent);
		free(item);
	}
}

CacheController::CacheController(char *rootdir)
{
	SetTemplateRootDir(rootdir);
}

void CacheController::AddTemplate(char *code, char* filepath)
{
	if (FindTemplate(code) == true)
	{

	}
	else
	{
		if (::WaitForSingleObject(ghMutex, 10000) == WAIT_OBJECT_0)
		{
			LPCACHEITEM item = (LPCACHEITEM)malloc(sizeof(CACHEITEM));
			assert(item != NULL);
			memset(item, 0, sizeof(CACHEITEM));
			item->code = _strdup(code);
			item->filename = _strdup(filepath);
			if (item->count == 0)
			{
				LoadContent(item->filename, &item->pcontent);
				item->count++;
			}
			m_cacheList.push_back(item);
		}
		::ReleaseMutex(ghMutex);
	}
}

void CacheController::SetTemplateRootDir(char* rootdir)
{
	memset(m_RootDir, '\0', DATA_BUFSIZE);
	sprintf_s(m_RootDir, DATA_BUFSIZE, "%s", rootdir);
}

std::string CacheController::GetTemplateContent(char* code)
{
	if (code == NULL) return false;

	int siz = m_cacheList.size();

	for (CACHELIST::iterator i = m_cacheList.begin(); i != m_cacheList.end(); ++i)
	{
		LPCACHEITEM item = *i;
		if (strcmp(item->code, code) == 0)
		{
			if (item->count == 0)
			{
				LoadContent(item->filename, &item->pcontent);
				item->count++;
			}
			std::string str = std::string(item->pcontent);
			return str;
		}
	}
	return NULL;
}


bool CacheController::FindTemplate(char *code)
{
	if (code == NULL) return false;

	if (::WaitForSingleObject(ghMutex, 10000) == WAIT_OBJECT_0)
	{
		for (CACHELIST::iterator i = m_cacheList.begin(); i != m_cacheList.end(); ++i)
		{
			LPCACHEITEM item = *i;
			if (strcmp(item->code, code) == 0)
			{
				::ReleaseMutex(ghMutex);
				return true;
			}
		}
	}
	::ReleaseMutex(ghMutex);
	return false;
}

std::string CacheController::GetTemplateFilepath(char* code)
{
	if (code == NULL) return false;

	if (::WaitForSingleObject(ghMutex, 10000) == WAIT_OBJECT_0)
	{
		for (CACHELIST::iterator i = m_cacheList.begin(); i != m_cacheList.end(); ++i)
		{
			LPCACHEITEM item = *i;
			if (strcmp(item->code, code) == 0)
			{
				std::string str;
				str.assign(item->filename);
				//std::wstring wstr;
				//wstr.assign(str.begin(), str.end());
				return str;
			}
		}
	}
	::ReleaseMutex(ghMutex);
	return NULL;
}


void CacheController::LoadContent(char* filename, char **content)
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	errno_t err = fopen_s(&pFile, filename, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		return;
	}

	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return;
	}

	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		return;
	}

	if (*content != NULL)
	{
		free(*content);
	}

	//*content = (char*)malloc(result+1);
	*content = buffer;

	if (DATA_BUFSIZE < result) {
		fputs("\nXXXXXXXXXXXXXXXXXX\nDATA_BUFSIZE oversize error\nXXXXXXXXXXXXXXXXX\n", stderr);
		//return;
	}

	//memset(*content, '\0', result);
	//sprintf_s(*content, result, "%s", buffer);

	fclose(pFile);
	//free(buffer);
}
