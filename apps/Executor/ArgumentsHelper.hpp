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
#include "ResultFormatter.hpp"

namespace Elpida
{

	class ArgumentsHelper final
	{
	 public:
		[[nodiscard]]
		const String& GetModulePath() const;

		[[nodiscard]]
		const ResultFormatter& GetResultFormatter() const;

		[[nodiscard]]
		Size GetBenchmarkIndex() const;

		[[nodiscard]]
		const Vector<unsigned int>& GetAffinity() const;

		[[nodiscard]]
		const Vector<String>& GetConfigurationValues() const;

		String ParseAndGetExitText(int argC, char* argV[]);

		ArgumentsHelper() = default;
		~ArgumentsHelper() = default;
	 private:
		Vector<unsigned int> _affinity;
		Vector<String> _configurationValues;
		UniquePtr<ResultFormatter> _resultFormatter;
		String _modulePath;
		Size _benchmarkIndex;

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