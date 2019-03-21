#include "pch.h"
#include "test_utils.h"
#include "DeleteDocumentCommand.h"
#include "GetDocumentsCommand.h"
#include "RequestExecutor.h"
#include "GetIndexNamesOperation.h"
#include "DeleteIndexOperation.h"

namespace ravendb::client::tests::utils
{
	void delete_all_documents(std::shared_ptr<documents::DocumentStore> store)
	{
		auto  get_docs_cmd = documents::commands::GetDocumentsCommand({}, {}, {}, {}, 0, 10000, true);
		auto re = store->get_request_executor();
		auto results = re->execute(get_docs_cmd);
		for (const auto& res : results->results)
		{
			auto del_doc_cmd = documents::commands::DeleteDocumentCommand(res["@metadata"]["@id"].get<std::string>());
			re->execute(del_doc_cmd);
		}
	}

	void delete_all_indexes(std::shared_ptr<documents::DocumentStore> store)
	{
		auto op1 = documents::operations::indexes::GetIndexNamesOperation(0, 10000);
		auto re = store->get_request_executor();
		auto&& results = re->execute(op1.get_command({}));
		for (auto& index_name : *results)
		{
			auto op = documents::operations::indexes::DeleteIndexOperation(index_name);
			re->execute(op.get_command({}));
		}
	}
}
