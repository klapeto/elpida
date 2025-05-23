#ifndef ELPIDA_OSINFOVIEW_HPP
#define ELPIDA_OSINFOVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{
	namespace Ui
	{
		class OsInfoView;
	}

	class OsInfoModel;

	class OsInfoView : public QWidget
	{
	Q_OBJECT
	public:
		explicit OsInfoView(const OsInfoModel& osInfo, QWidget* parent = nullptr);

		~OsInfoView() override;

	private:
		Ui::OsInfoView* _ui;
	};
}

#endif // ELPIDA_OSINFOVIEW_HPP
