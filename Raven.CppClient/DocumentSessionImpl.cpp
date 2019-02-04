#include "stdafx.h"
#include "DocumentSessionImpl.h"
#include "BatchOperation.h"
#include "BatchCommand.h"
#include "LoadOperation.h"
#include "SessionOptions.h"
#include "LoaderWithInclude.h"

namespace ravendb::client::documents::session
{
	DocumentSessionImpl::DocumentSessionImpl(DocumentStoreBase& document_store, SessionOptions options)
		: InMemoryDocumentSessionOperations(document_store, std::move(options))
	{}

	operations::LoadOperation DocumentSessionImpl::load_impl(const std::string& id)
	{
		auto load_op = operations::LoadOperation(*this);
		load_op.by_id(id);
		auto cmd = load_op.create_request();

		if (cmd)
		{
			_request_executor->execute(*cmd);
			load_op.set_result(cmd->get_result());
		}

		return load_op;
	}

	operations::LoadOperation DocumentSessionImpl::load_impl(const std::vector<std::reference_wrapper<const std::string>>& ids)
	{
		auto load_op = operations::LoadOperation(*this);
		load_internal(ids, load_op);

		return load_op;
	}

	operations::LoadOperation DocumentSessionImpl::load_impl(
		const std::vector<std::reference_wrapper<const std::string>>& ids,
		const std::vector<std::string>& includes)
	{
		auto load_op = operations::LoadOperation(*this);
		load_op.by_ids(ids);
		load_op.with_includes(includes);

		//TODO take care of counters;

		auto cmd = load_op.create_request();
		if (cmd)
		{
			_request_executor->execute(*cmd);
			load_op.set_result(cmd->get_result());
		}

		return load_op;
	}


	void DocumentSessionImpl::load_internal(const std::vector<std::reference_wrapper<const std::string>>& ids,
		operations::LoadOperation& operation)
	{
		operation.by_ids(ids);

		auto cmd = operation.create_request();

		if (cmd)
		{
			_request_executor->execute(*cmd);
			operation.set_result(cmd->get_result());
		}
	}

	loaders::LoaderWithInclude DocumentSessionImpl::include(const std::string& path)
	{
		return loaders::MultiLoaderWithInclude::create(*this).include(path);
	}

	bool DocumentSessionImpl::exists(const std::string& id)
	{
		if (_known_missing_ids.find(id) != _known_missing_ids.end())
		{
			return false;
		}
		if (_documents_by_id.find(id) != _documents_by_id.end())
		{
			return true;
		}

		//TODO execute HeadDocumentCommand , for now->
		throw std::runtime_error("Not implemented");
	}

	void DocumentSessionImpl::save_changes()
	{
		auto save_changes_operation = operations::BatchOperation(*this);
		if (auto command = save_changes_operation.create_request();
			!command)
		{
			return;
		}
		else
		{
			if (no_tracking)
			{
				throw std::runtime_error("Cannot execute save_changes() when entity tracking is disabled in session.");
			}
			_request_executor->execute(*command);
			update_session_after_changes(command->get_result());
			save_changes_operation.set_result(command->get_result());
		}
	}
}
