//
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_XML_XMLPARSER_HPP
#define ELPIDA_XML_XMLPARSER_HPP

#include "XmlElement.hpp"

namespace Elpida
{
	class XmlParser
	{
	public:
		XmlElement Parse(const char* data, std::size_t size);
		XmlParser() = default;
	};

} // Elpida

#endif //ELPIDA_XML_XMLPARSER_HPP
