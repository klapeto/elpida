project("Elpida Xml")

add_library(elpida-xml
        CharacterStream.cpp
        ParseException.cpp
        XmlElement.cpp
        XmlParser.cpp
        XmlMap.cpp)

set_property(TARGET elpida-xml PROPERTY POSITION_INDEPENDENT_CODE ON)