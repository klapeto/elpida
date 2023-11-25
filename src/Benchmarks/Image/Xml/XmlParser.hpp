//
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLPARSER_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLPARSER_HPP

#include "XmlElement.hpp"
namespace Elpida
{

	class XmlParser
	{
	public:
		static XmlElement Parse(const char* data, std::size_t size);
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLPARSER_HPP
