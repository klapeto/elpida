//
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLELEMENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLELEMENT_HPP

#include "Elpida/Core/StlAllocator.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Elpida
{



	class XmlElement final
	{
	public:

		using Children = std::vector<XmlElement>;
		using String = std::string;
		using Attributes = std::unordered_map<String, String>;


		XmlElement() = default;
		XmlElement(String&& name,
			Attributes && attributes,
			String&& content,
			Children&& children)
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
		String _name;
		Attributes _attributes;
		String _content;
		Children _children;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLELEMENT_HPP
