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
 * Logger.hpp
 *
 *  Created on: 28 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_LOGGER_HPP_
#define ELPIDA_UTILITIES_LOGGER_HPP_

#include "Elpida/Utilities/Singleton.hpp"
#include "Elpida/Types/String.hpp"

#include <ostream>

namespace Elpida
{

	class Logger: public Singleton<Logger>
	{
		public:
			enum class LogType
			{
				Info, Warning, Error
			};

			void setOutput(std::ostream& output)
			{
				_output = &output;
			}

			bool isTimestampsEnabled() const
			{
				return _timestampsEnabled;
			}

			void setTimestampsEnabled(bool timestampsEnabled)
			{
				_timestampsEnabled = timestampsEnabled;
			}

			template<typename ... TArgs>
			void log(LogType type, TArgs ... args)
			{
				if (_output != nullptr)
				{
					auto& out = *_output;
					if (_timestampsEnabled)
					{
						appendTimestamp(out);
					}
					appendLogType(type, out);
					log(args...);
				}
			}

		private:
			std::ostream* _output;
			bool _timestampsEnabled;
			Logger()
					: _output(nullptr), _timestampsEnabled(true)
			{
			}

			template<typename T, typename ... TArgs>
			void log(const T& str, TArgs ... args)
			{
				log(str);
				log(args...);
			}

			template<typename T>
			inline void log(const T& str)
			{
				(*_output) << str << ' ';
			}

			inline void log()
			{

			}

			void static appendTimestamp(std::ostream& out);
			void static appendLogType(LogType type, std::ostream& out);

			friend class Singleton<Logger> ;
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_LOGGER_HPP_ */
