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
