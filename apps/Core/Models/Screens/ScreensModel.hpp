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
// Created by klapeto on 8/6/20.
//

#ifndef APPS_QT_MODELS_SCREENSMODEL_HPP
#define APPS_QT_MODELS_SCREENSMODEL_HPP

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/Screens/ScreenItem.hpp"

namespace Elpida
{
	/**
	 * Weird model to use for QT only
	 */
	class ScreensModel : public ListModel<ScreenItem>
	{
	public:

		Event<ScreenItem&> selectionChanged;

		ScreenItem* getSelectedScreen() const
		{
			return _currentScreen;
		}

		void setSelectedScreen(ScreenItem* currentIndex)
		{
			_currentScreen = currentIndex;
			onDataChanged();
			selectionChanged.raise(*_currentScreen);
		}

		ScreensModel()
			: _currentScreen(nullptr)
		{
		}

		~ScreensModel() override = default;
	private:
		ScreenItem* _currentScreen;
	};
}


#endif //APPS_QT_MODELS_SCREENSMODEL_HPP
