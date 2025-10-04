/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
