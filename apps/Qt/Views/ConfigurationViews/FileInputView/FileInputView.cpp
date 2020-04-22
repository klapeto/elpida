#include "FileInputView.hpp"
#include "ui_FileInputView.h"
#include <Elpida/Engine/Configuration/ConfigurationValueBase.hpp>

namespace Elpida
{

	FileInputView::FileInputView()
		: QWidget(), _ui(new Ui::FileInputView), _configurationValue(nullptr)
	{
		_ui->setupUi(this);
	}

	FileInputView::~FileInputView()
	{
		delete _ui;
	}

	void FileInputView::setConfiguration(ConfigurationValueBase& configurationValue)
	{
		_configurationValue = &configurationValue;
		//_ui->lblPropertyName->setText(QString::fromStdString(_configurationValue.));
	}

} // namespace Elpida
