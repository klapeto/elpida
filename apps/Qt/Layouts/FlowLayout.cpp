//
// Created by klapeto on 19/3/2023.
//

#include "FlowLayout.hpp"
#include <QtWidgets>


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

int FlowLayout::horizontalSpacing() const
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

int FlowLayout::verticalSpacing() const
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
	return {};
}

bool FlowLayout::hasHeightForWidth() const
{
	return true;
}

int FlowLayout::heightForWidth(int width) const
{
	return doLayout(QRect(0, 0, width, 0), true);
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
		const QWidget* widget = item->widget();
		int spaceX = horizontalSpacing();
		if (spaceX == -1)
		{
			spaceX = widget->style()->layoutSpacing(
					QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
		}

		int spaceY = verticalSpacing();
		if (spaceY == -1)
		{
			spaceY = widget->style()->layoutSpacing(
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