//
// Created by klapeto on 26/5/2024.
//

#ifndef ELPIDA_QTPATHSSERVICE_HPP
#define ELPIDA_QTPATHSSERVICE_HPP


#include "Core/PathsService.hpp"

namespace Elpida::Application
{

	class QtPathsService: public PathsService
	{
	public:
		[[nodiscard]]
		const std::filesystem::path& GetDownloadStoragePath() const override;

		[[nodiscard]]
		const std::filesystem::path& GetExecutablePath() const override;

		QtPathsService();
		~QtPathsService() override = default;

	private:
		std::filesystem::path _downloadPath;
		std::filesystem::path _executablePath;
	};

} // Application
// Elpida

#endif //ELPIDA_QTPATHSSERVICE_HPP
