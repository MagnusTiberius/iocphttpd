#pragma once
#include <intrin.h>
#include <stdint.h>

#include "stdafx.h"

namespace HTTPMODULE2
{

	class CacheController
	{

	public:
		CacheController()
		{
			ghMutex = CreateMutex(
				NULL,              // default security attributes
				FALSE,             // initially not owned
				NULL);
		};

		~CacheController()
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
		};

	public:
		typedef struct {
			char *code;
			char *filename;
			//char content[DATA_BUFSIZE];
			char *pcontent;
			DWORD count;
		}CACHEITEM, *LPCACHEITEM;

		typedef vector<LPCACHEITEM> CACHELIST;


	private:
		CACHELIST m_cacheList;
		//typedef CACHELIST::iterator CACHELISTITERATOR;
		HANDLE ghMutex;

	public:
		CacheController(char *rootdir)
		{
			SetTemplateRootDir(rootdir);
		}


		void AddTemplate(char *code, char* filepath)
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


		void SetTemplateRootDir(char* rootdir)
		{
			memset(m_RootDir, '\0', DATA_BUFSIZE);
			sprintf_s(m_RootDir, DATA_BUFSIZE, "%s", rootdir);
		}

		std::string GetTemplateContent(char* code)
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

		bool FindTemplate(char *code)
		{
			if (code == NULL) return false;

			for (CACHELIST::iterator i = m_cacheList.begin(); i != m_cacheList.end(); ++i)
			{
				LPCACHEITEM item = *i;
				if (strcmp(item->code, code) == 0)
				{
					return true;
				}
			}
			return false;
		}

		std::string GetTemplateFilepath(char* code)
		{
			if (code == NULL) return false;

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
			return NULL;
		}


	private:
		char m_RootDir[DATA_BUFSIZE];
		void LoadContent(char* filename, char **content)
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

	};

	//template <class T>
	//CacheController::CacheController()
	//{
	//}
	//
	//template <class T>
	//CacheController::~CacheController()
	//{
	//}

	//template <class T>
	//void CacheController::AddTemplate(char *code, char* filepath)
	//{
	//	if (FindTemplate(code) == true)
	//	{
	//
	//	}
	//	else
	//	{
	//		LPCACHEITEM item = (LPCACHEITEM)malloc(sizeof(CACHEITEM));
	//		assert(item != NULL);
	//		memset(item, 0, sizeof(CACHEITEM));
	//		item->code = _strdup(code);
	//		item->filename = _strdup(filepath);
	//		if (item->count == 0)
	//		{
	//			LoadContent(item->filename, &item->pcontent);
	//			item->count++;
	//		}
	//		m_cacheList.push_back(item);
	//	}
	//}

	//template <class T>
	//CacheController::CacheController(char *rootdir)
	//{
	//	SetTemplateRootDir(rootdir);
	//}

	//template <class T>
	//void CacheController::SetTemplateRootDir(char* rootdir)
	//{
	//	memset(m_RootDir, '\0', DATA_BUFSIZE);
	//	sprintf_s(m_RootDir, DATA_BUFSIZE, "%s", rootdir);
	//}

	//template <class T>
	//std::string CacheController::GetTemplateFilepath(char* code)
	//{
	//	if (code == NULL) return false;
	//
	//	for (CACHELISTITERATOR i = m_cacheList.begin(); i != m_cacheList.end(); ++i)
	//	{
	//		LPCACHEITEM item = *i;
	//		if (strcmp(item->code, code) == 0)
	//		{
	//			std::string str;
	//			str.assign(item->filename);
	//			//std::wstring wstr;
	//			//wstr.assign(str.begin(), str.end());
	//			return str;
	//		}
	//	}
	//	return NULL;
	//}

	//template <class T>
	//std::string CacheController::GetTemplateContent(char* code)
	//{
	//	if (code == NULL) return false;
	//
	//	int siz = m_cacheList.size();
	//
	//	for (CACHELISTITERATOR i = m_cacheList.begin(); i != m_cacheList.end(); ++i)
	//	{
	//		LPCACHEITEM item = *i;
	//		if (strcmp(item->code, code) == 0)
	//		{
	//			if (item->count == 0)
	//			{
	//				LoadContent(item->filename, &item->pcontent);
	//				item->count++;
	//			}
	//			std::string str = std::string(item->pcontent);
	//			return str;
	//		}
	//	}
	//	return NULL;
	//}

	//template <class T>
	//bool CacheController::FindTemplate(char *code)
	//{
	//	if (code == NULL) return false;
	//
	//	for (CACHELISTITERATOR i = m_cacheList.begin(); i != m_cacheList.end(); ++i)
	//	{
	//		LPCACHEITEM item = *i;
	//		if (strcmp(item->code, code) == 0)
	//		{
	//			return true;
	//		}
	//	}
	//	return false;
	//}

	//template <class T>
	//void CacheController::LoadContent(char* filename, char **content)
	//{
	//	FILE * pFile;
	//	long lSize;
	//	char * buffer;
	//	size_t result;
	//
	//	errno_t err = fopen_s(&pFile, filename, "rb");
	//	if (pFile == NULL)
	//	{
	//		fputs("File error", stderr);
	//		return;
	//	}
	//
	//	fseek(pFile, 0, SEEK_END);
	//	lSize = ftell(pFile);
	//	rewind(pFile);
	//
	//	buffer = (char*)malloc(sizeof(char)*lSize);
	//	if (buffer == NULL)
	//	{
	//		fputs("Memory error", stderr);
	//		return;
	//	}
	//
	//	result = fread(buffer, 1, lSize, pFile);
	//	if (result != lSize)
	//	{
	//		fputs("Reading error", stderr);
	//		return;
	//	}
	//
	//	if (*content != NULL)
	//	{
	//		free(*content);
	//	}
	//
	//	//*content = (char*)malloc(result+1);
	//	*content = buffer;
	//
	//	if (DATA_BUFSIZE < result) {
	//		fputs("\nXXXXXXXXXXXXXXXXXX\nDATA_BUFSIZE oversize error\nXXXXXXXXXXXXXXXXX\n", stderr);
	//		//return;
	//	}
	//
	//	//memset(*content, '\0', result);
	//	//sprintf_s(*content, result, "%s", buffer);
	//
	//	fclose(pFile);
	//	//free(buffer);
	//}

}