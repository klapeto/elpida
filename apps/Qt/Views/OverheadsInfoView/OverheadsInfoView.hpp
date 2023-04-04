#ifndef ELPIDA_OVERHEADSINFOVIEW_HPP
#define ELPIDA_OVERHEADSINFOVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{
	namespace Ui
	{
		class OverheadsInfoView;
	}

	class OverheadsModel;

	class OverheadsInfoView : public QWidget
	{
	Q_OBJECT

	public:
		explicit OverheadsInfoView(const OverheadsModel& overheadsInfo, QWidget* parent = nullptr);

		~OverheadsInfoView() override;

	private:
		Ui::OverheadsInfoView* _ui;
	};
}

#endif // ELPIDA_OVERHEADSINFOVIEW_HPP
