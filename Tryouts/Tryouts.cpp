// Tryouts.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
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
	CURL* debug_curl_init(void*)
	{
		auto curl = curl_easy_init();
		if (curl == nullptr)
		{
			std::runtime_error("debug_curl_init failed");
		}
		//for fiddler
		curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:8888");

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		return curl;
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

	User user{ "Alexander","Timoshenko" };
	nlohmann::json j = user;
	std::string id{ "newID123" };
	auto executor = ravendb::client::http::RequestExecutor::create({ "http://127.0.0.1:8080" }, "TEST__DocumentCommandsTests", {},
		{ debug_curl_init, nullptr });
	{
		ravendb::client::PutDocumentCommand cmd(id, {}, j);
		executor->execute(cmd);
	}
	{
		ravendb::client::DeleteDocumentCommand cmd(id,"dssdf");
		executor->execute(cmd);
	}



}

