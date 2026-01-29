/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
		std::filesystem::path GetFullReportOutputPath() const override;

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
