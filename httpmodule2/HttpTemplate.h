#pragma once
#include "stdafx.h"

namespace HTTPMODULE2
{


	class HttpTemplate
	{
	public:
		HttpTemplate();
		~HttpTemplate();

		HttpTemplate(char *rootdir);

		typedef struct {
			char *code;
			char *filename;
			//char content[DATA_BUFSIZE];
			char *pcontent;
			DWORD count;
		}TEMPLATEITEM, *LPTEMPLATEITEM;

		typedef vector<LPTEMPLATEITEM> TEMPLATELIST;

		void AddTemplate(char *code, char* filepath);
		void SetTemplateRootDir(char* filepath);
		std::string GetTemplateContent(char* code);
		bool FindTemplate(char *code);
		std::string GetTemplateFilepath(char* code);
	protected:

		TEMPLATELIST m_TemplateList;

		typedef TEMPLATELIST::iterator TEMPLATEITERATOR;


	private:
		char m_RootDir[DATA_BUFSIZE];
		void LoadContent(char* filename, char **content);

	};

}