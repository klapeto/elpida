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

#ifndef ELPIDA_NUMBERINPUTVIEW_HPP
#define ELPIDA_NUMBERINPUTVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"
#include <Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp>

namespace Elpida
{
	namespace Ui
	{
		class NumberInputView;
	}

	class NumberInputView : public ConfigurationValueViewBase
	{
	Q_OBJECT

	public:

		void setConfiguration(const std::shared_ptr<ConfigurationValueBase>& configurationValue) override;
		std::shared_ptr<ConfigurationValueBase> getConfiguration() override
		{
			return _configurationValue;
		}
		void saveSetting() override;

		explicit NumberInputView();
		~NumberInputView() override;
	private:
		Ui::NumberInputView* _ui;
		std::shared_ptr<ConfigurationValueBase> _configurationValue;
		double _currentRawValue;
		bool _updating;
		double getScaledValue() const;
		void reInitializePrefix();

	private slots:
		void onStandardChanged(int state);
		void onScaleChanged(int state);
		void onValueChanged(double newValue);

		void setValue(double value);
		double calculateScaledValue(int index, const double* scaleValues, size_t size) const;
	};

} // namespace Elpida

#endif // ELPIDA_NUMBERINPUTVIEW_HPP
