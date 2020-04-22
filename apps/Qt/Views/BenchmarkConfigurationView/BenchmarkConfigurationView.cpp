#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QSpacerItem>

namespace Elpida
{

	BenchmarkConfigurationView::BenchmarkConfigurationView()
		: QWidget(nullptr), _ui(new Ui::BenchmarkConfigurationView)
	{
		_ui->setupUi(this);

//		auto layout = new QGridLayout;
//		layout->addWidget(new QLabel("Input Prop:"), 0,0,Qt::AlignLeft);
//		layout->addWidget(new QLineEdit(), 0,1,Qt::AlignLeft);
//		layout->addWidget(new QCheckBox("Bool Prop"), 1,0,1,3,Qt::AlignLeft);
//		layout->addWidget(new QLabel("File Prop:"), 2,0,Qt::AlignLeft);
//		layout->addWidget(new QLineEdit(), 2,1,Qt::AlignLeft);
//		layout->addWidget(new QPushButton("File.."), 2,3,Qt::AlignLeft);
//		_ui->scrollAreaWidgetContents->setLayout(layout);
	}

	BenchmarkConfigurationView::~BenchmarkConfigurationView()
	{
		delete _ui;
	}

} // namespace Elpida
