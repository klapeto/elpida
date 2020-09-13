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
// Created by klapeto on 22/8/20.
//

#ifndef APPS_CORE_MODELS_GLOBALCONFIGURATIONMODEL_HPP
#define APPS_CORE_MODELS_GLOBALCONFIGURATIONMODEL_HPP

#include "Models/Abstractions/Model.hpp"

namespace Elpida
{

	class GlobalConfigurationModel final : public Model
	{
	public:

		const std::string& getBenchmarksPath() const
		{
			return _benchmarksPath;
		}

		void setBenchmarksPath(const std::string& benchmarksPath)
		{
			_benchmarksPath = benchmarksPath;
			onDataChanged();
		}

		bool isUploadResults() const
		{
			return _uploadResults;
		}

		void setUploadResults(bool uploadResults)
		{
			_uploadResults = uploadResults;
			onDataChanged();
		}

		bool isOpenResultsWebPage() const
		{
			return _openResultsWebPage;
		}

		void setOpenResultsWebPage(bool openResultsWebPage)
		{
			_openResultsWebPage = openResultsWebPage;
			onDataChanged();
		}

		GlobalConfigurationModel() = default;
		~GlobalConfigurationModel() override = default;
	private:
		std::string _benchmarksPath;
		bool _uploadResults = true;
		bool _openResultsWebPage = false;
	};
}

#endif //APPS_CORE_MODELS_GLOBALCONFIGURATIONMODEL_HPP
