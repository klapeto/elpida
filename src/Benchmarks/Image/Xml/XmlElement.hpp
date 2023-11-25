//
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLELEMENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLELEMENT_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace Elpida
{

	class XmlElement final
	{
	public:

		XmlElement() = default;
		XmlElement(std::string&& name,
			std::unordered_map<std::string, std::string>&& attributes,
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
		std::unordered_map<std::string, std::string> _attributes;
		std::string _content;
		std::vector<XmlElement> _children;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLELEMENT_HPP
