#pragma once
#include <locale>
#include "CurlHandlesHolder.h"

namespace raven
{
	class GetDatabaseTopologyCommand :public RavenCommand<Topology>
	{
	public:

		~GetDatabaseTopologyCommand() override = default;
		GetDatabaseTopologyCommand() = default;

		void create_request(CURL* curl, const ServerNode& node, std::string& url) const override
		{
			std::ostringstream urlBuilder;
			urlBuilder << node.url << "/topology?name=" << node.database;

			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

			url = urlBuilder.str();
		}
		void set_response(CURL* curl, const nlohmann::json& response, bool from_cache) override
		{
			long statusCode;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
			if (statusCode == 200)
				_result = response;
		}

		bool is_read_request() const noexcept override
		{
			return true;
		}
	};
}
