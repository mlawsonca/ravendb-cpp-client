
#include "stdafx.h"
#include "RequestExecutor.h"
#include "GetDatabaseTopologyCommand.h"

using  
	ravendb::client::serverwide::commands::GetDatabaseTopologyCommand;

namespace ravendb::client::http
{
	void RequestExecutor::first_topology_update()
	{
		GetDatabaseTopologyCommand getTopology;

		std::ostringstream errors;

		for (const auto& url : _initial_urls)
		{
			Topology result;
			try
			{
				auto server_node = ServerNode(url, _db_name, "?");
				result = execute_internal<Topology>(server_node, getTopology);
			}
			catch (RavenError& re)
			{
				errors << re.what() << '\n';
				if (re.get_error_type() == RavenError::ErrorType::database_does_not_exist)
				{
					throw RavenError(errors.str(), RavenError::ErrorType::database_does_not_exist);
				}
				continue;
			}
			// success
			std::atomic_store(&_topology, std::make_shared<Topology>(result));
			return;
		}

		throw RavenError(errors.str(), RavenError::ErrorType::generic);
	}


	RequestExecutor::RequestExecutor(
		std::string db_name,
		std::vector<std::string> initial_urls,
		std::string certificate,
		ravendb::client::impl::SetCurlOptions set_before_perform,
		ravendb::client::impl::SetCurlOptions set_after_perform)
		: _db_name(std::move(db_name))
		, _initial_urls(std::move(initial_urls))
		, _topology(std::make_shared<Topology>())
		, _certificate(std::move(certificate))
		, _curl_holder(new impl::CurlHandlesHolder)
		, _set_before_perform(set_before_perform)
		, _set_after_perform(set_after_perform)
	{
		_topology->nodes.reserve(_initial_urls.size());

		for (const auto& url : _initial_urls)
		{
			_topology->nodes.emplace_back(url, _db_name, "?");
		}
		_topology->etag = -1;
	}

	void RequestExecutor::validate_urls()
	{
		const bool certificateHasHttps = ! _certificate.empty();

		if (_initial_urls.empty())
			throw RavenError("No urls has been defined", RavenError::ErrorType::bad_url);

		for (auto& url : _initial_urls)
		{
			if (url.empty())
			{
				throw RavenError("Empty url is not supported", RavenError::ErrorType::bad_url);
			}
			if (url.back() == '/')
			{
				url.pop_back();
			}

			if (certificateHasHttps)
			{
				if (url.compare(0, 5, "https") != 0) // not a match
				{
					throw RavenError("Unable to use url '" + url + "', a certificate was specified, so url must use 'https' as the url scheme.",
						RavenError::ErrorType::bad_url);
				}
			}
			else if (url.compare(0, 5, "http:") != 0) // not a match
			{
				throw RavenError("Unable to use url '" + url + "', a certificate was NOT specified, so url must use 'http' as the url scheme.",
					RavenError::ErrorType::bad_url);
			}
		}
	}


	std::unique_ptr<RequestExecutor> RequestExecutor::create(
		std::vector<std::string> urls,
		std::string db,
		std::string certificate,
		ravendb::client::impl::SetCurlOptions set_before_perform,
		ravendb::client::impl::SetCurlOptions set_after_perform)
	{
		//using explicit call to PRIVATE ctor
		auto rq = std::unique_ptr<RequestExecutor>(new RequestExecutor(
			std::move(db),
			std::move(urls),
			std::move(certificate),
			set_before_perform,
			set_after_perform));

		rq->validate_urls();

		if (! rq->_db_name.empty())
		{
			rq->first_topology_update();
		}
		return rq;
	}
}