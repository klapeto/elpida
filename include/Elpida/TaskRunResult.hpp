/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * TaskRunResult.hpp
 *
 *  Created on: 14 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TASKRUNRESULT_HPP_
#define ELPIDA_TASKRUNRESULT_HPP_

#include <string>

namespace Elpida
{

	class TaskRunResult final
	{
	public:

		double getActualValue() const
		{
			return _originalValue * _multiplier;
		}

		double getOriginalValue() const
		{
			return _originalValue;
		}

		const std::string& getValueTypeName() const
		{
			return _valueTypeName;
		}

		const std::string& getResultDescription() const
		{
			return _resultDescription;
		}

		void setResultDescription(const std::string& resultDescription)
		{
			_resultDescription = resultDescription;
		}

		void setOriginalValue(double originalValue)
		{
			_originalValue = originalValue;
		}

		void setValueName(const std::string& valueName)
		{
			_valueTypeName = valueName;
		}

		double getMultiplier() const
		{
			return _multiplier;
		}

		void setMultiplier(double multiplier)
		{
			_multiplier = multiplier;
		}

		void operator=(double value)
		{
			_originalValue = value;
		}

		void operator+=(double value)
		{
			_originalValue += value;
		}

		void setCustom(bool custom)
		{
			_custom = custom;
		}

		bool isCustom() const
		{
			return _custom;
		}

		double getTestedDataValue() const
		{
			return _testedDataValue;
		}

		void setTestedDataValue(double testedDataValue)
		{
			_testedDataValue = testedDataValue;
		}

		TaskRunResult()
			: _valueTypeName("OPS"), _originalValue(0.0), _multiplier(1.0), _testedDataValue(0.0), _custom(false)
		{

		}

		TaskRunResult(const std::string& description, const std::string& valueTypeName, double multiplier = 1.0)
			:
			_valueTypeName(valueTypeName),
			_resultDescription(description),
			_originalValue(0.0),
			_multiplier(multiplier),
			_testedDataValue(0.0),
			_custom(false)
		{

		}

		~TaskRunResult()
		{

		}

	private:
		std::string _valueTypeName;
		std::string _resultDescription;
		double _originalValue;
		double _multiplier;
		double _testedDataValue;
		bool _custom;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKRUNRESULT_HPP_ */
