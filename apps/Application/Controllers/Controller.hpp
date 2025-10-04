/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_CONTROLLER_HPP_
#define ELPIDA_CONTROLLER_HPP_

namespace Elpida::Application
{

	template<typename T>
	class Controller
	{
	 public:
		explicit Controller(T& model)
			: _model(model)
		{

		}
		Controller(const Controller&) = delete;
		Controller(Controller&&) noexcept = default;
		Controller& operator=(const Controller&) = delete;
		Controller& operator=(Controller&&) noexcept = default;
		virtual ~Controller() = default;
	 protected:
		T& _model;
	};

} // Application

#endif //ELPIDA_CONTROLLER_HPP_
