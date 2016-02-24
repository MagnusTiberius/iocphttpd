#include "MimeTypes.h"


namespace IOCPHTTPL
{

	MimeTypes::MimeTypes()
	{
		AddMimeType(L".avi", L"video/avi");
		AddMimeType(L".jpg", L"image/jpg");
		AddMimeType(L".jpeg", L"image/jpg");
		AddMimeType(L".png", L"image/png");
		AddMimeType(L".css", L"text/css");
		AddMimeType(L".js", L"application/javascript");
		AddMimeType(L".bmp", L"image/bmp");
		AddMimeType(L".bm", L"image/bmp");
		AddMimeType(L".bz", L"application/x-bzip");
		AddMimeType(L".js", L"application/javascript");
		AddMimeType(L".json", L"application/javascript");
		AddMimeType(L".midi", L"audio/midi");
		AddMimeType(L".mov", L"video/quicktime");
		AddMimeType(L".mp3", L"audio/mpeg3");
		AddMimeType(L".mpeg", L"video/mpeg");
		AddMimeType(L".mpg", L"video/mpeg");
		AddMimeType(L".pdf", L"application/pdf");
		AddMimeType(L".qt", L"video/quicktime");
		AddMimeType(L".rtf", L"application/rtf");
		AddMimeType(L".text", L"application/plain");
		AddMimeType(L".txt", L"text/plain");
		AddMimeType(L".htm", L"text/html");
		AddMimeType(L".html", L"text/html");
		AddMimeType(L".ico", L"image/x-icon");
		AddMimeType(L".java", L"text/plain");
		AddMimeType(L".log", L"text/plain");
		AddMimeType(L".mp2", L"audio/mpeg");
		AddMimeType(L".sgm", L"text/sgml");
		AddMimeType(L".sgml", L"text/sgml");
		AddMimeType(L".shtml", L"text/html");
		AddMimeType(L".snd", L"audio/basic");
		//AddMimeType(L"", L"");
	}


	MimeTypes::~MimeTypes()
	{
	}


	void MimeTypes::AddMimeType(std::wstring name, std::wstring val)
	{
		mimetype_t pair(name, val);
		mimetypeList.push_back(pair);
	}

	bool MimeTypes::Exists(std::wstring name, std::wstring *val)
	{
		it_mimetype_t it;
		for (it = mimetypeList.begin(); it != mimetypeList.end(); it++)
		{
			auto item = *it;
			auto first = item.first;
			if (first.compare(name.c_str()) == 0)
			{
				val->assign(item.second);
				return true;
			}
		}
		return false;
	}

	bool MimeTypes::AssignContentType(std::wstring name, std::wstring *val)
	{
		it_mimetype_t it;
		for (it = mimetypeList.begin(); it != mimetypeList.end(); it++)
		{
			auto item = *it;
			auto first = item.first;
			if (first.compare(name.c_str()) == 0)
			{
				val->assign(item.second);
				return true;
			}
		}
		return false;
	}

}