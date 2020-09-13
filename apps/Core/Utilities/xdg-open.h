//
// Created by klapeto on 13/9/20.
//

#ifndef APPS_CORE_UTILITIES_XDG_OPEN_H
#define APPS_CORE_UTILITIES_XDG_OPEN_H

#include <Elpida/Config.hpp>

#ifdef ELPIDA_LINUX
extern "C"
{
int xdg_open(const char* file_or_url);
}

#endif

#endif //APPS_CORE_UTILITIES_XDG_OPEN_H
