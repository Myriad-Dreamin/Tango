# pragma once

# ifdef __cplusplus
extern "C"{
# endif

#include "openssl/md5.h"
#pragma comment(lib, "libcrypto.lib")
# ifdef __cplusplus
}
# endif
#include <string>

namespace app_space {

	unsigned char * (*md5_origin)(const unsigned char * d, size_t n, unsigned char * md) = MD5;

	void md5_string(const std::string &src, std::string &dst);
	void md5_string(const std::string &src, char *dst);
	void md5_string(const std::string &src, unsigned char *dst);
	void md5_string(const char *src, std::string &dst);
	void md5_string(const char *src, char *dst);
	void md5_string(const char *src, unsigned char *dst);
	void md5_string(const unsigned char *src, size_t src_len, std::string &dst);
	void md5_string(const unsigned char *src, size_t src_len, char *dst);

}