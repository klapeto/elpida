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

#include "Elpida/Types/Float.hpp"
#include "Elpida/Types/String.hpp"

namespace Elpida
{

	class TaskRunResult final
	{
		public:

			Float64 getActualValue() const
			{
				return _originalValue * _multiplier;
			}

			Float64 getOriginalValue() const
			{
				return _originalValue;
			}

			const String& getValueTypeName() const
			{
				return _valueTypeName;
			}

			const String& getResultDescription() const
			{
				return _resultDescription;
			}

			void setResultDescription(const String& resultDescription)
			{
				_resultDescription = resultDescription;
			}

			void setOriginalValue(Float64 originalValue)
			{
				_originalValue = originalValue;
			}

			void setValueName(const String& valueName)
			{
				_valueTypeName = valueName;
			}

			Float64 getMultiplier() const
			{
				return _multiplier;
			}

			void setMultiplier(Float64 multiplier)
			{
				_multiplier = multiplier;
			}

			void operator=(Float64 value)
			{
				_originalValue = value;
			}

			void operator+=(Float64 value)
			{
				_originalValue += value;
			}

			TaskRunResult()
					: _valueTypeName("OPS"), _originalValue(0.0), _multiplier(1.0)
			{

			}

			TaskRunResult(const String& description, const String& valueTypeName, Float64 multiplier = 1.0)
					:
					  _valueTypeName(valueTypeName),
					  _resultDescription(description),
					  _originalValue(0.0),
					  _multiplier(multiplier)
			{

			}

			~TaskRunResult()
			{

			}

		private:
			String _valueTypeName;
			String _resultDescription;
			Float64 _originalValue;
			Float64 _multiplier;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKRUNRESULT_HPP_ */
