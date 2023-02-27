//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_DATAINFO_HPP
#define ELPIDA_DATAINFO_HPP

#include <string>
#include <vector>

namespace Elpida
{

	class DataInfo
	{
	public:

		[[nodiscard]]
		const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]]
		const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]]
		const std::string& getUnit() const
		{
			return _unit;
		}

		[[nodiscard]]
		const std::vector<std::string>& getMetadata() const
		{
			return _metadata;
		}

		[[nodiscard]]
		bool isUsed() const
		{
			return _isUsed;
		}

		DataInfo();

		DataInfo(std::string name,
				std::string description,
				std::string unit,
				const std::vector<std::string>& metadata);

		DataInfo(const DataInfo&) = default;

		DataInfo(DataInfo&&) = default;

		~DataInfo() = default;

	private:
		std::string _name;
		std::string _description;
		std::string _unit;
		std::vector<std::string> _metadata;

		bool _isUsed;
	};

} // Elpida

#endif //ELPIDA_DATAINFO_HPP
