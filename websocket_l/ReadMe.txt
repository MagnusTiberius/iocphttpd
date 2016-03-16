========================================================================
    STATIC LIBRARY : websocket_l Project Overview
========================================================================

Ref:
https://tools.ietf.org/html/rfc6455


Concretely, if as in the example above, the |Sec-WebSocket-Key|
   header field had the value "dGhlIHNhbXBsZSBub25jZQ==", the server
   would concatenate the string "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
   to form the string "dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-
   C5AB0DC85B11".  The server would then take the SHA-1 hash of this,
   giving the value 0xb3 0x7a 0x4f 0x2c 0xc0 0x62 0x4f 0x16 0x90 0xf6
   0x46 0x06 0xcf 0x38 0x59 0x45 0xb2 0xbe 0xc4 0xea.  This value is
   then base64-encoded (see Section 4 of [RFC4648]), to give the value
   "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=".  This value would then be echoed in
   the |Sec-WebSocket-Accept| header field.


Sample Input
=======================================
GET /test HTTP/1.1
Host: localhost:9090
Connection: Upgrade
Pragma: no-cache
Cache-Control: no-cache
Upgrade: websocket
Origin: http://127.0.0.1:8080
Sec-WebSocket-Version: 13
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.87 Safari/537.36
Accept-Encoding: gzip, deflate, sdch
Accept-Language: en-US,en;q=0.8
Sec-WebSocket-Key: fu8Ku2zinSODpJOMT+J+FA==
Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits



AppWizard has created this websocket_l library project for you.

This file contains a summary of what you will find in each of the files that
make up your websocket_l application.


websocket_l.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

websocket_l.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).


/////////////////////////////////////////////////////////////////////////////

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named websocket_l.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
