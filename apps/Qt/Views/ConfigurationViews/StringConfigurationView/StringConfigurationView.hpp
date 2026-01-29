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

#ifndef ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationView.hpp"

namespace Elpida::Application
{

	namespace Ui
	{
		class StringConfigurationView;
	}

	class StringConfigurationView : public ConfigurationView
	{
	 Q_OBJECT

	 public:
		explicit StringConfigurationView(QWidget* parent = nullptr);
		~StringConfigurationView() override;
	 protected:
		void OnModelSet() override;
	 private slots:
		void on_leValue_textChanged(const QString& value);
	 private:
		Ui::StringConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP
