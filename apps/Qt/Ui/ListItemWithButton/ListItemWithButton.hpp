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

#ifndef APPS_QT_UI_LISTITEMWITHBUTTON_LISTITEMWITHBUTTON_HPP
#define APPS_QT_UI_LISTITEMWITHBUTTON_LISTITEMWITHBUTTON_HPP

#include <QWidget>

namespace Elpida
{

	namespace Ui
	{
		class ListItemWithButton;
	}

	class ListItemWithButton : public QWidget
	{
	Q_OBJECT

	public:

		[[nodiscard]] QString getText() const;
		void setText(const QString& text);
		void setButtonText(const QString& text);
		void setButtonIcon(const QIcon& icon);

		explicit ListItemWithButton(const QString& text, QWidget* parent = nullptr);
		~ListItemWithButton() override;
	signals:
		void buttonClicked(const QString& name);
	protected:
		void enterEvent(QEvent* event) override;
		void leaveEvent(QEvent* event) override;

	private slots:
		void on_pbButton_clicked();

	private:
		Ui::ListItemWithButton* _ui;
	};

} // namespace Elpida
#endif // APPS_QT_UI_LISTITEMWITHBUTTON_LISTITEMWITHBUTTON_HPP
