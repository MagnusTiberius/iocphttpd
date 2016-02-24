#pragma once
#include "stdafx.h"

namespace IOCPHTTPL
{

	class IOCPHTTPL_API EventLog
	{
	public:
		EventLog()
		{

		}

		EventLog(PWSTR name)
		{
			m_name = name;
		}

		~EventLog()
		{

		}

		void WriteEventLogEntry2(char* pszMessage, WORD wType);

	private:
		// The status of the service
		SERVICE_STATUS m_status;

		// The service status handle
		SERVICE_STATUS_HANDLE m_statusHandle;

		// The name of the service
		PWSTR m_name;
	};


	//
	//   FUNCTION: CServiceBase::WriteEventLogEntry(PWSTR, WORD)
	//
	//   PURPOSE: Log a message to the Application event log.
	//
	//   PARAMETERS:
	//   * pszMessage - string message to be logged.
	//   * wType - the type of event to be logged. The parameter can be one of 
	//     the following values.
	//
	//     EVENTLOG_SUCCESS
	//     EVENTLOG_AUDIT_FAILURE
	//     EVENTLOG_AUDIT_SUCCESS
	//     EVENTLOG_ERROR_TYPE
	//     EVENTLOG_INFORMATION_TYPE
	//     EVENTLOG_WARNING_TYPE
	//
	void EventLog::WriteEventLogEntry2(char* pszMessage, WORD wType)
	{
		HANDLE hEventSource = NULL;
		LPCWSTR lpszStrings[2] = { NULL, NULL };
		char buf[1024];

		time_t now = time(0);
		tm *ltm = localtime(&now);

		sprintf(buf, "c:\\packages\\log_%d_%d_%d_%d.txt", ltm->tm_year, ltm->tm_mon, ltm->tm_mday, ltm->tm_hour);

		ofstream myfile;
		myfile.open(buf, std::ios_base::app);
		myfile << pszMessage << "\n";
		myfile.close();

		//hEventSource = RegisterEventSource(NULL, m_name);
		//if (hEventSource)
		//{
		//	lpszStrings[0] = m_name;
		//	lpszStrings[1] = pszMessage;

		//	ReportEvent(hEventSource,  // Event log handle
		//		wType,                 // Event type
		//		0,                     // Event category
		//		0,                     // Event identifier
		//		NULL,                  // No security identifier
		//		2,                     // Size of lpszStrings array
		//		0,                     // No binary data
		//		lpszStrings,           // Array of strings
		//		NULL                   // No binary data
		//		);

		//	DeregisterEventSource(hEventSource);
		//}
	}

}