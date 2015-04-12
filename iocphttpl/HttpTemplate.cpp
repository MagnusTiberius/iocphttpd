#include "HttpTemplate.h"


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
		memset(item, '\0', sizeof(TEMPLATEITEM));
		item->code = _strdup(code);
		item->filename = _strdup(filepath);
		if (item->count == 0)
		{
			LoadContent(item->filename, item->content);
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
	for (TEMPLATEITERATOR i = m_TemplateList.begin(); i != m_TemplateList.end(); ++i)
	{
		LPTEMPLATEITEM item = *i;
		if (strcmp(item->code, code) == 0)
		{
			if (item->count == 0)
			{
				LoadContent(item->filename, item->content);
				item->count++;
			}
			std::string str = std::string(item->content);
			return str;
		}
	}
	return NULL;
}

bool HttpTemplate::FindTemplate(char *code)
{
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

void HttpTemplate::LoadContent(char* filename, char *content)
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

	if (DATA_BUFSIZE < result) {
		fputs("DATA_BUFSIZE oversize error", stderr);
		return;
	}

	memset(content,  '\0', DATA_BUFSIZE);
	sprintf_s(content, DATA_BUFSIZE, "%s", buffer);

	fclose(pFile);
	free(buffer);
}