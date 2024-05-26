//
// Created by klapeto on 26/5/2024.
//

#ifndef ELPIDA_DESKTOPSERVICE_HPP
#define ELPIDA_DESKTOPSERVICE_HPP

#include <string>
#include <filesystem>

namespace Elpida::Application
{

	class DesktopService
	{
	public:
		virtual void OpenUri(const std::string& uri) const = 0;
		virtual void OpenFile(const std::filesystem::path& path) const = 0;

		DesktopService() = default;
		virtual ~DesktopService() = default;
	};

} // Application
// Elpida

#endif //ELPIDA_DESKTOPSERVICE_HPP
