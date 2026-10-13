/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 12/3/2023.
//

#ifndef ELPIDA_INFODUMPER_ARGUMENTSVALIDATOR_HPP_
#define ELPIDA_INFODUMPER_ARGUMENTSVALIDATOR_HPP_

#include <string>
#include <filesystem>

namespace Elpida::Application
{
	class ArgumentsHelper final
	{
	public:
		[[nodiscard]]
		const std::filesystem::path& GetBenchmarksPath() const;

		[[nodiscard]]
		const std::string& GetBenchmarkSuffix() const;

		[[nodiscard]]
		bool IsNoThreadPinning() const;

		[[nodiscard]]
		bool ParseAndGetExitText(int argC, char* argV[], std::string& returnText);

		ArgumentsHelper();
		~ArgumentsHelper() = default;

	private:
		std::filesystem::path _benchmarksPath;
		std::string _benchmarkSuffix;
		bool _noThreadPinning;
		static std::string GetHelpString();
		static std::string GetVersionString();
	};

} // Elpida::Application

#endif //ELPIDA_INFODUMPER_ARGUMENTSVALIDATOR_HPP_
