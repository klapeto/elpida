include(${CMAKE_CURRENT_LIST_DIR}/generate_product_version.cmake)

function(append_product_version files name description)
    if (MSYS OR MINGW OR MSVC)
        generate_product_version(
                files
                NAME ${name}
                ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
                VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
                VERSION_MINOR ${ELPIDA_VERSION_MINOR}
                VERSION_PATCH ${ELPIDA_VERSION_REVISION}
                VERSION_REVISION ${ELPIDA_VERSION_BUILD}
                COMPANY_NAME ${ELPIDA_COMPANY_NAME}
                COMPANY_COPYRIGHT ${ELPIDA_COMPANY_NAME}
                FILE_DESCRIPTION ${description}
        )
    endif()
    set(${files} ${${files}} PARENT_SCOPE)
endfunction()