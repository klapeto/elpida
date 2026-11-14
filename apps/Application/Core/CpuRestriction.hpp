/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
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

#ifndef ELPIDA_APPLICATION_CPURESTRICTION_HPP_
#define ELPIDA_APPLICATION_CPURESTRICTION_HPP_

#include "Elpida/Platform/CpuAvailabilityLoader.hpp"

#include <optional>
#include <string>

namespace Elpida::Application
{
	/// Where users report a restriction they think is detected by mistake
	constexpr const char* ReportIssueUrl = "https://gitlab.com/dev-hood/elpida/elpida/-/issues";

	class CpuRestriction final
	{
	 public:
		/// The current availability if it is restricted; nullopt if not, or if it can't be read (unknown doesn't block)
		[[nodiscard]]
		static std::optional<CpuAvailability> Check();

		/// The more restricted of the two (fewer allowed CPUs, or a smaller quota)
		[[nodiscard]]
		static const CpuAvailability& Worse(const CpuAvailability& a, const CpuAvailability& b);

		/// Before a run: what is restricted and why, that the results would be lower (and not uploaded), and where to
		/// report a false positive
		[[nodiscard]]
		static std::string GetWarning(const CpuAvailability& availability, bool uploadEnabled);

		/// After a run that was restricted at some point: why the results were not uploaded
		[[nodiscard]]
		static std::string GetNotUploadedMessage(const CpuAvailability& availability);

	 private:
		static std::string Describe(const CpuAvailability& availability);
	};

} // Elpida::Application

#endif //ELPIDA_APPLICATION_CPURESTRICTION_HPP_
