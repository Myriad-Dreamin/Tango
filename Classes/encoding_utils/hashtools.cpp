
#pragma once


#include "hashtools.h"

namespace app_space {

	void md5_string(const std::string & src, std::string & dst)
	{
		// static is not safe in multi-process
		unsigned char md_dst[33] = {0};
		md5_origin(reinterpret_cast<const unsigned char*>(src.c_str()), src.length(), md_dst);
		dst = std::move(std::string(reinterpret_cast<const char*>(md_dst)));
	}
	void md5_string(const char * src, std::string & dst)
	{
		// static is not safe in multi-process
		unsigned char md_dst[33] = {0};
		md5_origin(reinterpret_cast<const unsigned char*>(src), strlen(src), md_dst);
		dst = std::move(std::string(reinterpret_cast<const char*>(md_dst)));
	}
	void md5_string(const std::string & src, char * dst)
	{
		md5_origin(
			reinterpret_cast<const unsigned char*>(src.c_str()), src.length(),
			reinterpret_cast<unsigned char*>(dst)
		);
	}
	void md5_string(const std::string & src, unsigned char * dst)
	{
		md5_origin(
			reinterpret_cast<const unsigned char*>(src.c_str()), src.length(),
			dst
		);
	}
	void md5_string(const char * src, char * dst)
	{
		md5_origin(
			reinterpret_cast<const unsigned char*>(src), strlen(src),
			reinterpret_cast<unsigned char*>(dst)
		);
	}
	void md5_string(const char * src, unsigned char * dst)
	{
		md5_origin(
			reinterpret_cast<const unsigned char*>(src), strlen(src),
			dst
		);
	}
	void md5_string(const unsigned char * src, size_t src_len, std::string & dst)
	{
		unsigned char md_dst[33] = {0};
		md5_origin(
			reinterpret_cast<const unsigned char*>(src), src_len,
			md_dst
		);
		dst = std::move(std::string(reinterpret_cast<const char*>(md_dst)));
	}
	void md5_string(const unsigned char * src, size_t src_len, char * dst)
	{
		md5_origin(
			reinterpret_cast<const unsigned char*>(src), src_len,
			reinterpret_cast<unsigned char*>(dst)
		);
	}
}