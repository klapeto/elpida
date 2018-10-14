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
			Float64 getMeasuredValue() const
			{
				return _measuredValue;
			}

			const String& getMeasuredValueName() const
			{
				return _measuredValueName;
			}

			const String& getResultDescription() const
			{
				return _resultDescription;
			}

			void setResultDescription(const String& resultDescription)
			{
				_resultDescription = resultDescription;
			}

			void setMeasuredValue(Float64 measuredValue)
			{
				_measuredValue = measuredValue;
			}

			void setMeasuredValueName(const String& measuredValueName)
			{
				_measuredValueName = measuredValueName;
			}

			TaskRunResult()
					: _measuredValueName("OPS"), _measuredValue(0.0)
			{

			}

			TaskRunResult(const String& description, const String& measuredValueName)
					: _measuredValueName(measuredValueName), _resultDescription(description), _measuredValue(0.0)
			{

			}

			~TaskRunResult()
			{

			}
		private:
			String _measuredValueName;
			String _resultDescription;
			Float64 _measuredValue;

	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKRUNRESULT_HPP_ */
