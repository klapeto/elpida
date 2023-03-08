//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_DATAINFO_HPP
#define ELPIDA_DATAINFO_HPP

#include "Elpida/String.hpp"
#include "Elpida/Vector.hpp"

namespace Elpida
{

	class DataInfo
	{
	public:

		[[nodiscard]]
		const String& getName() const
		{
			return _name;
		}

		[[nodiscard]]
		const String& getDescription() const
		{
			return _description;
		}

		[[nodiscard]]
		const String& getUnit() const
		{
			return _unit;
		}

		[[nodiscard]]
		const Vector<String>& getMetadata() const
		{
			return _metadata;
		}

		[[nodiscard]]
		bool isUsed() const
		{
			return _isUsed;
		}

		DataInfo();
		DataInfo(String name,
				String description,
				String unit,
				const Vector<String>& metadata);
		DataInfo(const DataInfo&) = default;
		DataInfo(DataInfo&&) = default;
		~DataInfo() = default;
	private:
		String _name;
		String _description;
		String _unit;
		Vector<String> _metadata;

		bool _isUsed;
	};

} // Elpida

#endif //ELPIDA_DATAINFO_HPP
