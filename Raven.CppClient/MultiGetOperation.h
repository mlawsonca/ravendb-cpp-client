#pragma once
#include "InMemoryDocumentSessionOperations.h"
#include "MultiGetCommand.h"

namespace ravendb::client::documents::session::operations
{
	class MultiGetOperation
	{
	private:
		const std::shared_ptr<InMemoryDocumentSessionOperations> _session;

	public:
		explicit MultiGetOperation(std::shared_ptr<InMemoryDocumentSessionOperations> session)
			: _session(session)
		{}

		commands::multi_get::MultiGetCommand create_request(const std::vector<commands::multi_get::GetRequest>& requests)
		{
			//TODO use real cache
			http::HttpCache cache{};//temporal
			return commands::multi_get::MultiGetCommand(cache/*_session->get_request_executor().get_cache()*/, requests);
		}
	};
}
