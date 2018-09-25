#ifndef IMAGETASKSPROPERIES_HPP
#define IMAGETASKSPROPERIES_HPP

#include <QWidget>

namespace Ui {
	class ImageTasksProperies;
}

class ImageTasksProperies : public QWidget
{
		Q_OBJECT

	public:
		explicit ImageTasksProperies(QWidget *parent = 0);
		~ImageTasksProperies();

	private slots:
		void on_pbSelectInput_clicked();

	private:
		Ui::ImageTasksProperies *ui;
};

#endif // IMAGETASKSPROPERIES_HPP
