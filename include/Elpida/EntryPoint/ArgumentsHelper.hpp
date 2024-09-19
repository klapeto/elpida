//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 12/3/2023.
//

#ifndef ELPIDA_ARGUMENTSVALIDATOR_HPP_
#define ELPIDA_ARGUMENTSVALIDATOR_HPP_

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"
#include "Elpida/Core/ConcurrencyMode.hpp"
#include "ResultFormatter.hpp"

namespace Elpida
{

	class ArgumentsHelper final
	{
	 public:
		[[nodiscard]]
		const ResultFormatter& GetResultFormatter() const;

		[[nodiscard]]
		Size GetBenchmarkIndex() const;

		[[nodiscard]]
		const Vector<unsigned int>& GetAffinity() const;

		[[nodiscard]]
		const Vector<String>& GetConfigurationValues() const;

		[[nodiscard]]
		double GetNowOverhead() const;

		[[nodiscard]]
		double GetLoopOverhead() const;

		[[nodiscard]]
		double GetMicroTaskDuration() const;

		[[nodiscard]]
		bool GetNumaAware() const;

		[[nodiscard]]
		bool GetPinThreads() const;

		[[nodiscard]]
		bool GetDumpInfo() const;

		[[nodiscard]]
		ConcurrencyMode GetConcurrencyMode() const;

		bool ParseAndGetExitText(int argC, char* argV[], std::string& returnText);

		ArgumentsHelper();
		~ArgumentsHelper() = default;
	 private:
		Vector<unsigned int> _affinity;
		Vector<String> _configurationValues;
		UniquePtr<ResultFormatter> _resultFormatter;
		Size _benchmarkIndex;
		double _nowOverhead;
		double _loopOverhead;
		double _microTaskDuration;
		ConcurrencyMode _concurrencyMode;
		bool _numaAware;
		bool _pinThreads;
		bool _dumpInfo;

		void ParseAffinity(const String& value);
		void ParseFormat(const String& value);
		void ParseIndex(const String& value);
		void ParseModulePath(const String& value);

		void ValidateAffinity();
		static String GetHelpString();
		static String GetVersionString() ;
	};

} // Elpida

#endif //ELPIDA_ARGUMENTSVALIDATOR_HPP_
