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

#ifndef ELPIDA_FILEINPUTVIEW_HPP
#define ELPIDA_FILEINPUTVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"
#include <Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp>
#include <Elpida/Engine/Configuration/ConfigurationType.hpp>

namespace Elpida
{

	namespace Ui
	{
		class FileInputView;
	}

	class FileInputView : public ConfigurationValueViewBase
	{
	Q_OBJECT

	public:
		void setConfiguration(ConfigurationValueBase* configurationValue) override;
		ConfigurationValueBase* getConfiguration() override;
		void saveSetting() override;

		explicit FileInputView();
		~FileInputView() override;
	private:
		Ui::FileInputView* _ui;
		ConfigurationValue<ConfigurationType::FilePath>* _configurationValue;

	public slots:
		void onEditingFinished();
		void onBrowseClicked(bool checked);
	};

} // namespace Elpida

#endif // ELPIDA_FILEINPUTVIEW_HPP
