#include "FileInputView.hpp"
#include "ui_FileInputView.h"

namespace Elpida
{

	FileInputView::FileInputView()
		: QWidget(), _ui(new Ui::FileInputView)
	{
		_ui->setupUi(this);
	}

	FileInputView::~FileInputView()
	{
		delete _ui;
	}

} // namespace Elpida
