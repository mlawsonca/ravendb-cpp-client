// Tryouts.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define DEBUG

#include <iostream>

#include "GetDocumentsCommand.h"
#include "PutDocumentCommand.h"
#include "RequestExecutor.h"
#include "GetDatabaseRecordCommand.h"
#include "CreateDatabaseCommand.h"

#include "GetDatabaseNamesCommand.h"
#include "DeleteDatabaseCommand.h"
#include "DeleteDocumentCommand.h"

namespace
{
	//using fiddler + verbose
	void set_for_fiddler(CURL* curl)
	{
		curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:8888");
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	}


	struct User
	{
		std::string FirstName;
		std::string LastName;
	};

	bool operator==(const User& lhs, const User& rhs)
	{
		return lhs.FirstName == rhs.FirstName &&
			lhs.LastName == rhs.LastName;
	}

	void to_json(nlohmann::json& j, const User& u)
	{
		j["FirstName"] = u.FirstName;
		j["LastName"] = u.LastName;
	}

	void from_json(const nlohmann::json& j, User& u)
	{
		u.FirstName = j.at("FirstName").get<std::string>();
		u.LastName = j.at("LastName").get<std::string>();
	}
}

int main()
{
	using namespace ravendb::client;

	//User user{ "Alexander","Timoshenko" };
	//nlohmann::json j = user;
	//std::string id{ "newID123" };
	//auto executor = ravendb::client::http::RequestExecutor::create({ "http://127.0.0.1:8080" }, "TEST__DocumentCommandsTests", {},
	//	{ debug_curl_init, nullptr });
	//{
	//	documents::commands::PutDocumentCommand cmd(id, {}, j);
	//	executor->execute(cmd);
	//}
	//{
	//	documents::commands::DeleteDocumentCommand cmd(id,"dssdf");
	//	executor->execute(cmd);
	//}

	auto exec = http::RequestExecutor::create({ "http://127.0.0.1:8080" }, {}, {}, set_for_fiddler);
	serverwide::DatabaseRecord rec;
	rec.database_name = "test";
	serverwide::operations::CreateDatabaseCommand cmd(rec,1);

	auto res1 = exec->execute(cmd);

	for (auto& it : res1.topology.promotables_status)
	{
		std::cout << static_cast<uint8_t>(it.second)<<" ";
	}

	//auto exec = http::RequestExecutor::create({ "http://127.0.0.1:8080" }, "Test", {}, set_for_fiddler);
	//std::string id1(500,'a'), id2 = id1+"aaa", id3 = id2+"AAA";

	//documents::commands::GetDocumentsCommand cmd({id1,id2,id3}, {}, true);
	//exec->execute(cmd);

	//auto exec = http::RequestExecutor::create({ "http://127.0.0.1:8080" }, {}, {}, set_for_fiddler);
	serverwide::operations::GetDatabaseRecordCommand cmd2("test");

	auto res2 = exec->execute(cmd2);

	getchar();

}

