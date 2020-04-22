#ifndef ELPIDA_FILEINPUTVIEW_HPP
#define ELPIDA_FILEINPUTVIEW_HPP

#include <QWidget>

namespace Elpida
{

	namespace Ui
	{
		class FileInputView;
	}

	class FileInputView : public QWidget
	{
	Q_OBJECT

	public:
		explicit FileInputView();
		~FileInputView();

	private:
		Ui::FileInputView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_FILEINPUTVIEW_HPP
