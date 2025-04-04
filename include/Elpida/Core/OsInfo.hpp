//
// Created by klapeto on 13/3/2023.
//

#ifndef ELPIDA_OSINFO_HPP_
#define ELPIDA_OSINFO_HPP_

#include "Elpida/Core/String.hpp"

namespace Elpida
{

	class OsInfo final
	{
	 public:
		[[nodiscard]]
		const String& GetCategory() const;

		[[nodiscard]]
		const String& GetName() const;

		[[nodiscard]]
		const String& GetVersion() const;

		OsInfo(String category, String name, String version);
		OsInfo(const OsInfo&) = default;
		OsInfo(OsInfo&&) noexcept = default;
		OsInfo& operator=(const OsInfo&) = default;
		OsInfo& operator=(OsInfo&&) noexcept = default;
		~OsInfo() = default;
	 private:
		String _category;
		String _name;
		String _version;
	};

} // Elpida

#endif //ELPIDA_OSINFO_HPP_
