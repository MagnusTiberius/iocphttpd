#include "HttpTemplate.h"


namespace IOCPHTTPL
{

	HttpTemplate::HttpTemplate()
	{
	}


	HttpTemplate::~HttpTemplate()
	{
	}


	void HttpTemplate::AddTemplate(char *code, char* filepath)
	{
		if (FindTemplate(code) == true)
		{

		}
		else
		{
			LPTEMPLATEITEM item = (LPTEMPLATEITEM)malloc(sizeof(TEMPLATEITEM));
			assert(item != NULL);
			memset(item, 0, sizeof(TEMPLATEITEM));
			item->code = _strdup(code);
			item->filename = _strdup(filepath);
			if (item->count == 0)
			{
				LoadContent(item->filename, &item->pcontent);
				item->count++;
			}
			m_TemplateList.push_back(item);
		}
	}

	HttpTemplate::HttpTemplate(char *rootdir)
	{
		SetTemplateRootDir(rootdir);
	}

	void HttpTemplate::SetTemplateRootDir(char* rootdir)
	{
		memset(m_RootDir, '\0', DATA_BUFSIZE);
		sprintf_s(m_RootDir, DATA_BUFSIZE, "%s", rootdir);
	}

	std::string HttpTemplate::GetTemplateFilepath(char* code)
	{
		if (code == NULL) return false;

		for (TEMPLATEITERATOR i = m_TemplateList.begin(); i != m_TemplateList.end(); ++i)
		{
			LPTEMPLATEITEM item = *i;
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


	std::string HttpTemplate::GetTemplateContent(char* code)
	{
		if (code == NULL) return false;

		int siz = m_TemplateList.size();

		for (TEMPLATEITERATOR i = m_TemplateList.begin(); i != m_TemplateList.end(); ++i)
		{
			LPTEMPLATEITEM item = *i;
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

	bool HttpTemplate::FindTemplate(char *code)
	{
		if (code == NULL) return false;

		for (TEMPLATEITERATOR i = m_TemplateList.begin(); i != m_TemplateList.end(); ++i)
		{
			LPTEMPLATEITEM item = *i;
			if (strcmp(item->code, code) == 0)
			{
				return true;
			}
		}
		return false;
	}

	void HttpTemplate::LoadContent(char* filename, char **content)
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

}