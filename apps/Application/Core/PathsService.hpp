//
// Created by klapeto on 26/5/2024.
//

#ifndef ELPIDA_PATHSSERVICE_HPP
#define ELPIDA_PATHSSERVICE_HPP

#include <filesystem>

namespace Elpida::Application
{

	class PathsService
	{
	public:
		[[nodiscard]]
		virtual const std::filesystem::path& GetDownloadStoragePath() const = 0;

		[[nodiscard]]
		virtual std::filesystem::path GetFullReportOutputPath() const = 0;

		[[nodiscard]]
		virtual const std::filesystem::path& GetExecutablePath() const = 0;

		virtual ~PathsService() = default;
	};

} // Application
// Elpida

#endif //ELPIDA_PATHSSERVICE_HPP
