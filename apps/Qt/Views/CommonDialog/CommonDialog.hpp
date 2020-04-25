#ifndef ELPIDA_COMMONDIALOG_HPP
#define ELPIDA_COMMONDIALOG_HPP

#include <QDialog>

namespace Elpida
{

	namespace Ui
	{
		class CommonDialog;
	}

	class CommonDialog : public QDialog
	{
	Q_OBJECT

	public:

		DialogCode show(QWidget* widget);

		explicit CommonDialog(QWidget* parent);
		~CommonDialog() override;

	private:
		Ui::CommonDialog* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_COMMONDIALOG_HPP
