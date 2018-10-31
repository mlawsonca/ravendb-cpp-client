#include "stdafx.h"
#include "GetDocumentsCommand.h"
#include "RequestExecutor.h"
#include "CurlHandlesHolder.h"

using namespace raven;

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


int main(int argc, char** argv)
{
	RavenGlobalInit::make_global_initializations();

	std::unique_ptr<RequestExecutor> rq;
	try
	{
		rq = RequestExecutor::create({ "http://127.0.0.1:8080" }, "Northwind", {}, { debug_curl_init, nullptr });
	}
	catch (RavenError &re)
	{
		std::cout << re.what() << std::endl;
		return 1;
	}

	//GetDocumentsCommand cmd(std::vector<std::string>{ "employees/8-A", "employees/1-A" }, {}, false);
	//GetDocumentsResult result;
	//try
	//{
	//	result = rq->execute(cmd);
	//}
	//catch (RavenError &re)
	//{
	//	std::cout << re.what() << std::endl;
	//	return 2;
	//}
	//for (auto& doc : result.results)
	//{
	//	if (std::string(doc.type_name()) == "null")
	//	{
	//		continue;
	//	}
	//	auto id = doc.at("@metadata").at("@id").get<std::string>();
	//	auto name = doc.at("FirstName").get<std::string>();
	//	std::cout << "id - " << id << ", name - " << name << std::endl;
	//}

	std::string sample_doc = R"(
		{
			"LastName": "Timoshenko",
			"FirstName" : "Alexander",
			"@metadata" :
				{
					"@collection": "Employees"
				}
		})";

	nlohmann::json j = nlohmann::json::parse(sample_doc);

	//PutDocumentCommand cmd("newID", {}, sample_doc);
	PutDocumentCommand cmd("newID", {}, j);


	PutResult result;
	try
	{
		result = rq->execute(cmd);
	}
	catch (RavenError &re)
	{
		std::cout << re.what() << std::endl;
		return 3;
	}

	std::cout << "Result id is :" << result.id << std::endl;
	std::cout << "Change vector is :" << result.change_vector << std::endl;

	return 0;
}