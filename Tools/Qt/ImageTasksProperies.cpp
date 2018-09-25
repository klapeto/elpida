#include "ImageTasksProperies.hpp"
#include "ui_ImageTasksProperies.h"
#include <QFileDialog>

ImageTasksProperies::ImageTasksProperies(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ImageTasksProperies)
{
	ui->setupUi(this);
}

ImageTasksProperies::~ImageTasksProperies()
{
	delete ui;
}

void ImageTasksProperies::on_pbSelectInput_clicked()
{
	auto filename = QFileDialog::getOpenFileName(this, "Open Png Image","", "");
}
