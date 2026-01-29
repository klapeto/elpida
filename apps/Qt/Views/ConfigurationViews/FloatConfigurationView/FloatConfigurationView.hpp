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

#ifndef ELPIDA_APPLICATION_FLOATCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_FLOATCONFIGURATIONVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationView.hpp"

namespace Elpida::Application
{

	namespace Ui
	{
		class FloatConfigurationView;
	}


	class BenchmarkConfigurationModel;
	class FloatConfigurationView : public ConfigurationView
	{
	 Q_OBJECT

	 public:

		explicit FloatConfigurationView(QWidget* parent = nullptr);
		~FloatConfigurationView() override;
	 protected:
		void OnModelSet() override;
	 private slots:
		void on_spnValue_valueChanged(double value);
	 private:
		Ui::FloatConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_FLOATCONFIGURATIONVIEW_HPP
