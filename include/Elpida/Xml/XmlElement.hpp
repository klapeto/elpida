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
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_XML_XMLELEMENT_HPP
#define ELPIDA_XML_XMLELEMENT_HPP

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "XmlMap.hpp"

namespace Elpida
{
	class XmlElement final
	{
	public:
		const std::string& GetName() const
		{
			return _name;
		}

		const XmlMap& GetAttributes() const
		{
			return _attributes;
		}

		const std::string& GetContent() const
		{
			return _content;
		}

		const std::vector<XmlElement>& GetChildren() const
		{
			return _children;
		}

		const std::string& GetAttributeValue(const std::string& name) const
		{
			return _attributes.GetValue(name);
		}

		XmlElement() = default;

		XmlElement(std::string&& name,
		           XmlMap&& attributes,
		           std::string&& content,
		           std::vector<XmlElement>&& children)
			: _name(std::move(name)), _attributes(std::move(attributes)), _content(std::move(content)),
			  _children(std::move(children))
		{
		}

		XmlElement(const XmlElement&) = default;
		XmlElement(XmlElement&&) noexcept = default;
		XmlElement& operator =(XmlElement&&) noexcept = default;
		XmlElement& operator =(const XmlElement&) = default;
		~XmlElement() = default;

	private:
		std::string _name;
		XmlMap _attributes;
		std::string _content;
		std::vector<XmlElement> _children;
	};
} // Elpida

#endif //ELPIDA_XML_XMLELEMENT_HPP
