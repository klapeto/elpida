#ifndef ELPIDA_LOGSDIALOG_HPP
#define ELPIDA_LOGSDIALOG_HPP

#include <QDialog>
#include "Elpida/Types/String.hpp"

namespace Elpida {

	namespace Ui {
		class LogsDialog;
	}

	class LogsDialog final: public QDialog
	{
			Q_OBJECT

		public:
			void setLogsText(const String& text);

			explicit LogsDialog(QWidget *parent = 0);
			~LogsDialog();

		private:
			Ui::LogsDialog *_ui;
	};


} // namespace Elpida
#endif // ELPIDA_LOGSDIALOG_HPP
