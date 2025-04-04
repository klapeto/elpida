//
// Created by klapeto on 13/3/2023.
//

#include "Elpida/Core/OsInfo.hpp"

#include <utility>

namespace Elpida
{
	OsInfo::OsInfo(String category, String name, String version)
		: _category(std::move(category)), _name(std::move(name)), _version(std::move(version))
	{
	}
	const String& OsInfo::GetCategory() const
	{
		return _category;
	}
	const String& OsInfo::GetName() const
	{
		return _name;
	}
	const String& OsInfo::GetVersion() const
	{
		return _version;
	}
} // Elpida