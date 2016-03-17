#include "Base64.h"


namespace WebSocket
{

	Base64::Base64()
	{
	}


	Base64::~Base64()
	{
	}

	char *Base64::base64_encode2(const char *data, size_t input_length, size_t *output_length) 
	{

		*output_length = 4 * ((input_length + 2) / 3);

		char *encoded_data = (char*)malloc(*output_length);
		ZeroMemory(encoded_data, *output_length);
		if (encoded_data == NULL) return NULL;

		for (int i = 0, j = 0; i < input_length;) {

			uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
			uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
			uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

			uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

			encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
			encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
			encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
			encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
		}

		for (int i = 0; i < mod_table[input_length % 3]; i++)
			encoded_data[*output_length - 1 - i] = '=';

		return encoded_data;
	}


	void Base64::base64_encode(unsigned char* bytes_to_encode, unsigned int in_len, char* out)
	{
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];
		static const std::string base64_chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";

		while (in_len--) {
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i <4); i++)
					*out++ = base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				*out++ = base64_chars[char_array_4[j]];

			while ((i++ < 3))
				*out++ = '=';

		}
		*out = 0;
	}

	unsigned char *Base64::base64_decode(const char *data, size_t input_length, size_t *output_length) 
	{

		if (decoding_table == NULL) build_decoding_table();

		if (input_length % 4 != 0) return NULL;

		*output_length = input_length / 4 * 3;
		if (data[input_length - 1] == '=') (*output_length)--;
		if (data[input_length - 2] == '=') (*output_length)--;

		unsigned char *decoded_data = (unsigned char *)malloc(*output_length);
		ZeroMemory(decoded_data, *output_length);
		if (decoded_data == NULL) return NULL;

		for (int i = 0, j = 0; i < input_length;) {

			uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
			uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
			uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
			uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

			uint32_t triple = (sextet_a << 3 * 6)
				+ (sextet_b << 2 * 6)
				+ (sextet_c << 1 * 6)
				+ (sextet_d << 0 * 6);

			if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
			if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
			if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
		}

		return decoded_data;
	}


	void Base64::build_decoding_table() 
	{

		decoding_table = (char*)malloc(256);

		for (int i = 0; i < 64; i++)
			decoding_table[(unsigned char)encoding_table[i]] = i;
	}


	void Base64::base64_cleanup() 
	{
		free(decoding_table);
	}

}