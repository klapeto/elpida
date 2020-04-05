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
 * Plugin.cpp
 *
 *  Created on: 3 Ιουλ 2018
 *      Author: klapeto
 */

#include "TaskBatches/Image/ImageTaskBatch.hpp"
#include "TaskBatches/Image/ImageTasksProperties.hpp"


extern "C" Elpida::QtTaskBatchWrapper* createQtBatchWrapper()
{
	return new Elpida::ImageTasksProperties();
}

extern "C" std::vector<Elpida::QtTaskBatchWrapper*>* createQtBatchWrappers()
{
	return new std::vector<Elpida::QtTaskBatchWrapper*>{ new Elpida::ImageTasksProperties() };
}