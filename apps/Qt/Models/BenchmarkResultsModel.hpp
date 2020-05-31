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
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKRESULTSMODEL_HPP
#define APPS_QT_MODELS_BENCHMARKRESULTSMODEL_HPP

#include <Elpida/Engine/Result/BenchmarkResult.hpp>
#include "Models/Abstractions/ListModel/ListModel.hpp"

namespace Elpida
{
	class BenchmarkResultsModel : public ListModel<BenchmarkResult>
	{

	};
}


#endif //APPS_QT_MODELS_BENCHMARKRESULTSMODEL_HPP
