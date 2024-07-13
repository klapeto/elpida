//
// Created by klapeto on 8/4/2023.
//

#include "OsInfoModel.hpp"

#include <utility>

namespace Elpida::Application
{
	OsInfoModel::OsInfoModel(std::string category, std::string name, std::string version)
		: _category(std::move(category)), _name(std::move(name)), _version(std::move(version))
	{
	}
	const std::string& OsInfoModel::GetCategory() const
	{
		return _category;
	}
	const std::string& OsInfoModel::GetName() const
	{
		return _name;
	}
	const std::string& OsInfoModel::GetVersion() const
	{
		return _version;
	}
} // Application