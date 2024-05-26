//
// Created by klapeto on 26/5/2024.
//

#ifndef ELPIDA_QTDESKTOPSERVICE_HPP
#define ELPIDA_QTDESKTOPSERVICE_HPP


#include "Core/DesktopService.hpp"

namespace Elpida::Application
{

	class QtDesktopService: public DesktopService
	{
	public:
		void OpenUri(const std::string& uri) const override;
		void OpenFile(const std::filesystem::path& path) const override;

		QtDesktopService() = default;
		~QtDesktopService() override = default;
	};

} // Application
// Elpida

#endif //ELPIDA_QTDESKTOPSERVICE_HPP
