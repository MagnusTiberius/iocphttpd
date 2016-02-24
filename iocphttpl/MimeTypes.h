#pragma once
#include "stdafx.h"

namespace IOCPHTTPL
{

	class MimeTypes
	{
	public:
		MimeTypes();
		~MimeTypes();

		typedef std::pair<std::wstring, std::wstring> mimetype_t;


		typedef std::vector<mimetype_t> mimetypelist_t;

		typedef std::vector<mimetype_t>::iterator it_mimetype_t;

		void AddMimeType(std::wstring name, std::wstring val);
		bool Exists(std::wstring name, std::wstring *val);
		bool AssignContentType(std::wstring name, std::wstring *val);

	private:

		mimetypelist_t mimetypeList;

	};

}