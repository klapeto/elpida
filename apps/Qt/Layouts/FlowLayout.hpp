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
