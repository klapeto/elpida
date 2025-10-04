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
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_XML_XMLMAP_HPP
#define ELPIDA_XML_XMLMAP_HPP

#include <string>
#include <unordered_map>

namespace Elpida
{
	class XmlMap : public std::unordered_map<std::string, std::string>
	{
	public:
		const std::string& GetValue(const std::string& key) const
		{
			if (const auto itr = find(key); itr != end())
			{
				return itr->second;
			}
			return Empty;
		}

		void Set(const std::string_view key, std::string value)
		{
			this->operator[](std::string(key)) = std::move(value);
		}

	private:
		static inline const std::string Empty;
	};
} // Elpida

#endif //ELPIDA_XML_XMLMAP_HPP
