#ifndef ELPIDA_OVERHEADSINFOVIEW_HPP
#define ELPIDA_OVERHEADSINFOVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{
	namespace Ui
	{
		class TimingInfoView;
	}

	class TimingModel;

	class TimingInfoView : public QWidget
	{
	Q_OBJECT

	public:
		explicit TimingInfoView(const TimingModel& model, QWidget* parent = nullptr);

		~TimingInfoView() override;

	private:
		Ui::TimingInfoView* _ui;
	};
}

#endif // ELPIDA_OVERHEADSINFOVIEW_HPP
