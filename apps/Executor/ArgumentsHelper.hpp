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
		const ResultFormatter& GetResultFormatter() const;

		[[nodiscard]]
		Size GetBenchmarkIndex() const;

		void ValidateAndAssignConfiguration(const Vector<String>& configurationValues, Vector<TaskConfiguration>& taskConfigurations) const;

		[[nodiscard]]
		Vector<Ref<const ProcessingUnitNode>> ValidateAndGetProcessingUnits(const TopologyInfo& topologyInfo) const;

		ArgumentsHelper(
			const String& modulePath,
			const String& benchmarkIndex,
			const String& affinity,
			const String& format);
		~ArgumentsHelper() = default;
	 private:
		UniquePtr<ResultFormatter> _resultFormatter;
		Vector<unsigned int> _parsedAffinity;
		Size _benchmarkIndex;
		static void ValidateAffinity(const Vector<unsigned int>& affinity);
		static Vector<unsigned int> Parse(const String& value);
	};

} // Elpida

#endif //ELPIDA_ARGUMENTSVALIDATOR_HPP_
