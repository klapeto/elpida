/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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


//
// Created by klapeto on 7/12/20.
//

#ifndef APPS_CORE_MODELS_AFFINITYMODEL_HPP
#define APPS_CORE_MODELS_AFFINITYMODEL_HPP

#include <Elpida/Engine/Task/TaskAffinity.hpp>
#include "Abstractions/Model.hpp"

namespace Elpida
{
	class AffinityModel: public Model
	{
	public:
		const TaskAffinity& getCurrentAffinity() const
		{
			return _currentAffinity;
		}

		void setCurrentAffinity(TaskAffinity&& currentAffinity)
		{
			_currentAffinity = std::move(currentAffinity);
			onDataChanged();
		}

		AffinityModel() = default;
		~AffinityModel() override = default;
	private:
		TaskAffinity _currentAffinity;
	};
}

#endif //APPS_CORE_MODELS_AFFINITYMODEL_HPP
