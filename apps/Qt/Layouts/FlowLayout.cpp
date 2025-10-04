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

//
// Created by klapeto on 19/3/2023.
//

#include "FlowLayout.hpp"
#include <QtWidgets>
#include "Views/TopologyNodeView/TopologyNodeView.hpp"


FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
		: _hSpace(hSpacing), _vSpace(vSpacing)
{
	setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
	QLayoutItem* item;
	while ((item = takeAt(0)))
	{
		delete item;
	}
}

void FlowLayout::addItem(QLayoutItem* item)
{
	_itemList.append(item);
}

int FlowLayout::GetHorizontalSpacing() const
{
	if (_hSpace >= 0)
	{
		return _hSpace;
	}
	else
	{
		return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
	}
}

int FlowLayout::GetVerticalSpacing() const
{
	if (_vSpace >= 0)
	{
		return _vSpace;
	}
	else
	{
		return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
	}
}

int FlowLayout::count() const
{
	return _itemList.size();
}

QLayoutItem* FlowLayout::itemAt(int index) const
{
	return _itemList.value(index);
}

QLayoutItem* FlowLayout::takeAt(int index)
{
	if (index >= 0 && index < _itemList.size())
	{
		return _itemList.takeAt(index);
	}
	return nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
	return Qt::Horizontal;
}

bool FlowLayout::hasHeightForWidth() const
{
	return true;
}

int FlowLayout::heightForWidth(int width) const
{
	if (_cachedHeight == -1 || _previousWidth != width)
	{
		_previousWidth = width;
		_cachedHeight = doLayout(QRect(0, 0, width, 0), true);
	}

	return _cachedHeight;
}

void FlowLayout::setGeometry(const QRect& rect)
{
	QLayout::setGeometry(rect);
	doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
	return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
	QSize size;
	for (const QLayoutItem* item: qAsConst(_itemList))
	{
		size = size.expandedTo(item->minimumSize());
	}

	const QMargins margins = contentsMargins();
	size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
	return size;
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
	QObject* parent = this->parent();
	if (!parent)
	{
		return -1;
	}
	else if (parent->isWidgetType())
	{
		auto* pw = static_cast<QWidget*>(parent);
		return pw->style()->pixelMetric(pm, nullptr, pw);
	}
	else
	{
		return static_cast<QLayout*>(parent)->spacing();
	}
}

void FlowLayout::invalidate()
{
	_cachedHeight = -1;
}

int FlowLayout::doLayout(const QRect& rect, bool testOnly) const
{
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);

	int x = effectiveRect.x();
	int y = effectiveRect.y();

	int lineHeight = 0;
	for (QLayoutItem* item: qAsConst(_itemList))
	{
		const QWidget* wid = item->widget();

		int spaceX = GetHorizontalSpacing();
		if (spaceX == -1)
		{
			spaceX = wid->style()->layoutSpacing(
					QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
		}

		int spaceY = GetVerticalSpacing();
		if (spaceY == -1)
		{
			spaceY = wid->style()->layoutSpacing(
					QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
		}

		int nextX = x + item->sizeHint().width() + spaceX;
		if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
		{
			x = effectiveRect.x();
			y = y + lineHeight + spaceY;
			nextX = x + item->sizeHint().width() + spaceX;
			lineHeight = 0;
		}

		if (!testOnly)
		{
			item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
		}
		x = nextX;
		lineHeight = qMax(lineHeight, item->sizeHint().height());
	}
	return y + lineHeight - rect.y() + bottom;
}
