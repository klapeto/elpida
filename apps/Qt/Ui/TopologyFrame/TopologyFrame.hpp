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

#ifndef APPS_QT_UI_TOPOLOGYFRAME_TOPOLOGYFRAME_HPP
#define APPS_QT_UI_TOPOLOGYFRAME_TOPOLOGYFRAME_HPP

#include <QFrame>

class QCheckBox;

namespace Elpida
{
	class ProcessorNode;

	namespace Ui
	{
		class TopologyFrame;
	}

	class TopologyFrame : public QFrame
	{
	Q_OBJECT

	signals:
		void clicked(const TopologyFrame* node);

	public:
		bool isClickAble() const
		{
			return _clickAble;
		}

		void setClickAble(bool clickAble)
		{
			_clickAble = clickAble;
		}

		void setDefaultStyle(const QString& defaultStyle)
		{
			_defaultStyle = defaultStyle;
			if (_clickAble && !_mouseDown && !_mouseOver)
			{
				setStyleSheet(_defaultStyle);
			}
		}

		void setClickedStyle(const QString& clickedStyle)
		{
			_clickedStyle = clickedStyle;
			if (_clickAble && _mouseDown)
			{
				setStyleSheet(_clickedStyle);
			}
		}

		void setMouseOverStyle(const QString& mouseOverStyle)
		{
			_mouseOverStyle = mouseOverStyle;
			if (_clickAble && _mouseOver && !_mouseDown)
			{
				setStyleSheet(_mouseOverStyle);
			}
		}

		const QString& getDefaultStyle() const
		{
			return _defaultStyle;
		}

		const QString& getMouseOverStyle() const
		{
			return _mouseOverStyle;
		}

		const QString& getClickedStyle() const
		{
			return _clickedStyle;
		}

		const ProcessorNode& getProcessorNode() const
		{
			return _node;
		}

		void appendChild(TopologyFrame* frame)
		{
			_children.push_back(frame);
		}

		const std::vector<TopologyFrame*>& getChildren() const
		{
			return _children;
		}

		QCheckBox* getCheckBox() const
		{
			return _checkBox;
		}

		void setCheckBox(QCheckBox* checkBox)
		{
			_checkBox = checkBox;
		}

		explicit TopologyFrame(const ProcessorNode& node);
		~TopologyFrame();

	protected:
		virtual void mousePressEvent(QMouseEvent* event) override;
		virtual void mouseReleaseEvent(QMouseEvent* event) override;
		virtual void enterEvent(QEvent* event) override;
		virtual void leaveEvent(QEvent* event) override;
	private:
		QString _defaultStyle;
		QString _mouseOverStyle;
		QString _clickedStyle;

		std::vector<TopologyFrame*> _children;

		const ProcessorNode& _node;

		Ui::TopologyFrame* ui;
		QCheckBox* _checkBox;

		bool _clickAble;
		bool _mouseDown;
		bool _mouseOver;
	};

} // namespace Elpida
#endif // APPS_QT_UI_TOPOLOGYFRAME_TOPOLOGYFRAME_HPP
