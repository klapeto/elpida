project("Elpida Xml")

add_library(elpida-xml SHARED
        CharacterStream.cpp
        ParseException.cpp
        XmlElement.cpp
        XmlParser.cpp
        XmlMap.cpp)

install(TARGETS elpida-xml)