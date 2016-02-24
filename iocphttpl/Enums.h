#pragma once
#include "stdafx.h"

namespace IOCPHTTPL
{

	typedef enum { HTTP_NONE, HTTP_GET, HTTP_POST } MethodType;


	typedef enum {
		PSTATE_GET, PSTATE_POST, PSTATE_URL, PSTATE_HTTP_VER
		, PSTATE_HOST_DETECTED, PSTATE_HOST_VALUE
		, PSTATE_CONNECTION_DETECTED, PSTATE_CONNECTION_VALUE
	} ParseState;

}