project("Elpida Xml")

add_library(elpida-xml
        CharacterStream.cpp
        ParseException.cpp
        XmlElement.cpp
        XmlParser.cpp
        XmlMap.cpp)

install(TARGETS elpida-xml)