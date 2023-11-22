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

	class XmlElement
	{
	public:

	private:
		std::string _name;
		std::unordered_map<std::string, std::string> _attributes;
		std::vector<XmlElement> _children;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_XML_XMLELEMENT_HPP
