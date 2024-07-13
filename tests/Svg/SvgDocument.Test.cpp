//
// Created by klapeto on 20/12/2023.
//


#include <Elpida/Xml/XmlParser.hpp>

#include "Elpida/Svg/SvgDocument.hpp"

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgDocumentTests, Success)
{
	std::string data = R"--(<?xml version="1.0" encoding="UTF-8"?>
<!-- Created with Inkscape (http://www.inkscape.org/) -->
<svg width="40.279mm" height="40.279mm" version="1.1" viewBox="0 0 40.279 40.279" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"><defs><radialGradient id="radialGradient2" cx="49.071" cy="76.061" r="20.14" gradientUnits="userSpaceOnUse"><stop offset="0"/><stop stop-opacity="0" offset="1"/></radialGradient></defs><g transform="translate(-28.931 -55.921)"><circle cx="49.071" cy="76.061" r="20.007" fill="url(#radialGradient2)" stroke="#9cf" stroke-width=".26458"/></g></svg>
)--";

	XmlParser parser;
	const auto element = parser.Parse(data.c_str(), data.size());
	SvgDocument document(element);
}