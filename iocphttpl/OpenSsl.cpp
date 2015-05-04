#include "OpenSsl.h"


OpenSsl::OpenSsl()
{
}


OpenSsl::~OpenSsl()
{
}


void OpenSsl::Init()
{
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();

	ctx = SSL_CTX_new(SSLv23_server_method());

	cctx = SSL_CONF_CTX_new();
	SSL_CONF_CTX_set_flags(cctx, SSL_CONF_FLAG_SERVER);
	SSL_CONF_CTX_set_flags(cctx, SSL_CONF_FLAG_CERTIFICATE);
	SSL_CONF_CTX_set_ssl_ctx(cctx, ctx);
}
