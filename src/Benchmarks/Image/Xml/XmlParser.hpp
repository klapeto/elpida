//
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLPARSER_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLPARSER_HPP

#include "Elpida/Core/Allocator.hpp"
#include "XmlElement.hpp"
#include "Elpida/Core/SharedPtr.hpp"
namespace Elpida
{

	class XmlParser
	{
	public:
		XmlElement Parse(const char* data, std::size_t size);
		XmlParser() = default;
		explicit XmlParser(SharedPtr<Allocator> allocator);
	private:
		SharedPtr<Allocator> _allocator;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLPARSER_HPP
