#include "stdafx.h"
#include "CreateDatabaseOperation.h"
#include "DatabaseRecord.h"

namespace ravendb::client::serverwide::operations
{
	CreateDatabaseOperation::~CreateDatabaseOperation() = default;

	CreateDatabaseOperation::CreateDatabaseOperation(DatabaseRecord database_record, int32_t replication_factor)
		: _database_record(std::move(database_record))
		, _replication_factor(replication_factor)
	{}

	std::unique_ptr<http::RavenCommand<DatabasePutResult>> CreateDatabaseOperation::get_command(
		const std::shared_ptr<documents::conventions::DocumentConventions> conventions) const
	{
		return std::make_unique<CreateDatabaseCommand>(conventions, _database_record, _replication_factor);
	}

	CreateDatabaseOperation::CreateDatabaseCommand::~CreateDatabaseCommand() = default;

	CreateDatabaseOperation::CreateDatabaseCommand::CreateDatabaseCommand(
		std::shared_ptr<documents::conventions::DocumentConventions> conventions,
		DatabaseRecord database_record, int32_t replication_factor, std::optional<int64_t> etag)
		: _conventions(conventions)
		, _database_record(std::move(database_record))
		, _replication_factor(replication_factor)
		, _etag(etag)
		, _database_name([this]()-> const std::string&
	{
		if (_database_record.database_name.empty())
		{
			throw std::invalid_argument("Database name is required");
		}
		else
		{
			return _database_record.database_name;
		}
	}())
		, _database_document_stream(nlohmann::json(_database_record).dump())
	{}

	void CreateDatabaseOperation::CreateDatabaseCommand::create_request(impl::CurlHandlesHolder::CurlReference& curl_ref,
		std::shared_ptr<const http::ServerNode> node,
		std::optional<std::string>& url_ref)
	{
		auto curl = curl_ref.get();
		std::ostringstream path_builder;

		path_builder << node->url << "/admin/databases?name=" << _database_name
			<< "&replicationFactor=" << _replication_factor;

		if(_etag)
		{
			std::ostringstream etag{};
			etag << '"' << *_etag << '"';
			curl_ref.headers.emplace(constants::headers::ETAG, etag.str());
		}

		curl_ref.method = constants::methods::PUT;
		curl_ref.headers.insert_or_assign("Transfer-Encoding", "chunked");
		curl_ref.headers.emplace(constants::headers::CONTENT_TYPE, "application/json");

		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, stream_read_callback);
		curl_easy_setopt(curl, CURLOPT_READDATA, &_database_document_stream);

		_database_document_stream.clear();
		_database_document_stream.seekg(0);

		url_ref.emplace(path_builder.str());
	}

	void CreateDatabaseOperation::CreateDatabaseCommand::set_response(const std::optional<nlohmann::json>& response, bool from_cache)
	{
		if(!response)
		{
			throw_invalid_response();
		}
		_result = std::make_shared<ResultType>(response->get<ResultType>());
	}

	bool CreateDatabaseOperation::CreateDatabaseCommand::is_read_request() const
	{
		return false;
	}

}