//
// Created by klapeto on 19/4/20.
//

#ifndef APPS_QT_CONTROLLERS_QCUSTOMAPPLICATION_HPP
#define APPS_QT_CONTROLLERS_QCUSTOMAPPLICATION_HPP

#include <QApplication>

namespace Elpida
{
	class Mediator;

	class QCustomApplication : public QApplication
	{
	public:

		bool notify(QObject *, QEvent *) override;

		QCustomApplication(int& argc, char** argv, Mediator& mediator);
		~QCustomApplication() override = default;

	private:
		Mediator& _mediator;

	};

}


#endif //APPS_QT_CONTROLLERS_QCUSTOMAPPLICATION_HPP
