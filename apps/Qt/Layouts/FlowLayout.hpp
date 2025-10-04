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

// modified version of https://doc.qt.io/qt-5/qtwidgets-layouts-flowlayout-example.html
#ifndef ELPIDA_FLOWLAYOUT_HPP
#define ELPIDA_FLOWLAYOUT_HPP

#include <QLayout>
#include <QRect>
#include <QStyle>

class FlowLayout : public QLayout
{
public:
	void addItem(QLayoutItem* item) override;
	int GetHorizontalSpacing() const;
	int GetVerticalSpacing() const;
	Qt::Orientations expandingDirections() const override;
	bool hasHeightForWidth() const override;
	int heightForWidth(int) const override;
	int count() const override;
	QLayoutItem* itemAt(int index) const override;
	QSize minimumSize() const override;
	void setGeometry(const QRect& rect) override;
	QSize sizeHint() const override;
	QLayoutItem* takeAt(int index) override;
	void invalidate() override;

	explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
	~FlowLayout() override;
private:
	int doLayout(const QRect &rect, bool testOnly) const;

	int smartSpacing(QStyle::PixelMetric pm) const;

	QList<QLayoutItem*> _itemList;

	mutable int _cachedHeight;
	mutable int _previousWidth;

	int _hSpace;
	int _vSpace;
};

#endif //ELPIDA_FLOWLAYOUT_HPP
