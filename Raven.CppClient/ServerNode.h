#pragma once
#include "stdafx.h"
#include "json_utils.h"

namespace ravendb::client::http
{
	enum class Role
	{
		NONE,
		PROMOTABLE,
		MEMBER,
		REHAB,
		INVALID_VALUE = -1
	};

	NLOHMANN_JSON_SERIALIZE_ENUM(Role,
		{
			{Role::INVALID_VALUE, nullptr},
			{Role::NONE, "None"},
			{Role::PROMOTABLE , "Promotable"},
			{Role::MEMBER, "Member"},
			{Role::REHAB, "Rehab"}
		});

	struct ServerNode
	{
		std::string url{};
		std::string database{};
		std::string clusterTag{};
		Role server_role = Role::NONE;

		ServerNode() = default;

		ServerNode(std::string url, std::string db, std::string tag, Role role = Role::NONE)
			: url(std::move(url))
			, database(std::move(db))
			, clusterTag(std::move(tag))
			, server_role(role)
		{}

		bool operator==(const ServerNode& other)
		{
			return (&other == this)||(this->url == other.url && this->database == other.database);
		}

	};

	inline void from_json(const nlohmann::json& j, ServerNode& sn)
	{
		using ravendb::client::impl::utils::json_utils::get_val_from_json;

		get_val_from_json(j, "Url", sn.url);
		get_val_from_json(j, "Database", sn.database);
		get_val_from_json(j, "ClusterTag", sn.clusterTag);
		get_val_from_json(j, "ServerRole", sn.server_role);
	}
}