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

#ifndef APPS_QT_UI_TOPOLOGYNODEFRAME_TOPOLOGYNODEFRAME_HPP
#define APPS_QT_UI_TOPOLOGYNODEFRAME_TOPOLOGYNODEFRAME_HPP

#include <QFrame>

class QCheckBox;

namespace Elpida
{
	class ProcessorNode;

	namespace Ui
	{
		class TopologyNodeFrame;
	}

	class TopologyNodeFrame : public QFrame
	{
	Q_OBJECT

	signals:
		void clicked(const TopologyNodeFrame* node);

	public:
		void setDefaultStyle(const QString& defaultStyle);
		void setClickedStyle(const QString& clickedStyle);
		void setMouseOverStyle(const QString& mouseOverStyle);

		[[nodiscard]] const ProcessorNode& getProcessorNode() const
		{
			return _node;
		}

		void appendChild(TopologyNodeFrame* frame)
		{
			_children.push_back(frame);
		}

		[[nodiscard]] const std::vector<TopologyNodeFrame*>& getChildren() const
		{
			return _children;
		}

		[[nodiscard]] QCheckBox* getCheckBox() const
		{
			return _checkBox;
		}

		void setCheckBox(QCheckBox* checkBox)
		{
			_checkBox = checkBox;
		}

		explicit TopologyNodeFrame(const ProcessorNode& node);
		~TopologyNodeFrame() override;

	protected:
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;
		void enterEvent(QEvent* event) override;
		void leaveEvent(QEvent* event) override;
	private:
		QString _defaultStyle;
		QString _mouseOverStyle;
		QString _clickedStyle;

		std::vector<TopologyNodeFrame*> _children;

		const ProcessorNode& _node;

		Ui::TopologyNodeFrame* ui;
		QCheckBox* _checkBox;

		bool _mouseDown;
		bool _mouseOver;
	};

} // namespace Elpida

#endif //APPS_QT_UI_TOPOLOGYNODEFRAME_TOPOLOGYNODEFRAME_HPP
