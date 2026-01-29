project("Elpida Xml")

add_library(elpida-xml
        CharacterStream.cpp
        ParseException.cpp
        XmlElement.cpp
        XmlParser.cpp
        XmlMap.cpp
)

if (ELPIDA_INSTALL_CORE_LIBRARIES)
    install(TARGETS elpida-xml ARCHIVE)
endif ()