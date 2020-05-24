//
// Created by klapeto on 8/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATASPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATASPECIFICATION_HPP

#include <string>

namespace Elpida
{
	class DataSpecification
	{
	public:
		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]] const std::string& getUnit() const
		{
			return _unit;
		}

		[[nodiscard]] size_t getSizeShouldBeDivisibleBy() const
		{
			return _sizeShouldBeDivisibleBy;
		}

		DataSpecification();
		DataSpecification(std::string name, std::string unit, std::string description = std::string());
		DataSpecification(std::string name, std::string unit, size_t sizeShouldBeDivisibleBy, std::string description = std::string());
		DataSpecification(DataSpecification&&) = default;
		DataSpecification(const DataSpecification&) = default;
		DataSpecification& operator=(DataSpecification&&) = default;
		DataSpecification& operator=(const DataSpecification&) = default;
		~DataSpecification() = default;
	private:
		std::string _name;
		std::string _description;
		std::string _unit;
		size_t _sizeShouldBeDivisibleBy;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATASPECIFICATION_HPP
