#pragma once

#include "stdafx.h"
#include <stdint.h>
#include <stdlib.h>

namespace WebSocket
{
	static char encoding_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3',
		'4', '5', '6', '7', '8', '9', '+', '/' };
	static char *decoding_table = NULL;
	static int mod_table[] = { 0, 2, 1 };


	class Base64
	{
	public:
		Base64();
		~Base64();

		static char *base64_encode2(const char *data, size_t input_length, size_t *output_length);
		static unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);
		static void build_decoding_table();
		void base64_cleanup();
		static void base64_encode(unsigned char* bytes_to_encode, unsigned int in_len, char* out);
	private:


	};

}