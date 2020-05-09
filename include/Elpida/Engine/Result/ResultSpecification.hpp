//
// Created by klapeto on 8/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_RESULTSPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_RESULTSPECIFICATION_HPP

#include <string>
#include "TaskResult.hpp"

namespace Elpida
{
	class ResultSpecification final
	{
	public:
		enum Type
		{
			Throughput,
			Raw
		};

		enum AggregationType
		{
			Accumulative,
			Average
		};

		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] const std::string& getUnit() const
		{
			return _unit;
		}

		[[nodiscard]] AggregationType getAggregationType() const
		{
			return _aggregationType;
		}
		[[nodiscard]] const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]] Type getType() const
		{
			return _type;
		}

		explicit ResultSpecification(std::string name,
			std::string unit,
			Type type,
			AggregationType aggregationType,
			std::string description = std::string());
		ResultSpecification(ResultSpecification&&) = default;
		ResultSpecification(const ResultSpecification&) = default;
		ResultSpecification& operator=(ResultSpecification&&) = default;
		ResultSpecification& operator=(const ResultSpecification&) = default;
		~ResultSpecification() = default;
	private:
		std::string _name;
		std::string _unit;
		std::string _description;
		Type _type;
		AggregationType _aggregationType;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_RESULT_RESULTSPECIFICATION_HPP
