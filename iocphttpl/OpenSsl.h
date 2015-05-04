#pragma once

#include "stdafx.h"

class OpenSsl
{
public:
	OpenSsl();
	~OpenSsl();

	void Init();

private:
	char *port = "*:4433";
	BIO *ssl_bio, *tmp;
	SSL_CTX *ctx;
	SSL_CONF_CTX *cctx;
	char buf[512];
	BIO *in = NULL;
	int ret = 1, i;
};

/*

http://p-nand-q.com/programming/windows/building_openssl_with_visual_studio_2013.html

http://developer.covenanteyes.com/building-openssl-for-visual-studio/

*/