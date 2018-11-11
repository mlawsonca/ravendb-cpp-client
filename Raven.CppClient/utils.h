#pragma once
#include "stdafx.h"

namespace ravendb::client::impl 
{
	class Utils final
	{
	public:
		Utils() = delete;
		~Utils() = delete;

		static size_t push_headers(char *buffer, size_t size, size_t nitems, void *userdata);
		static size_t push_to_buffer(char* contents, size_t size, size_t nmemb, void* output_buffer_void);
		static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
		static std::string url_escape(CURL* curl, const std::string& val);
	};
}

