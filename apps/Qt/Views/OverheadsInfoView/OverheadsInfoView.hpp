#ifndef ELPIDA_OVERHEADSINFOVIEW_HPP
#define ELPIDA_OVERHEADSINFOVIEW_HPP

#include <QWidget>

namespace Ui
{
	class OverheadsInfoView;
}

namespace Elpida
{
	class OverheadsInfo;

	namespace Application
	{
		class OverheadsInfoView : public QWidget
		{
		Q_OBJECT

		public:
			explicit OverheadsInfoView(const OverheadsInfo& overheadsInfo, QWidget* parent = nullptr);
			~OverheadsInfoView() override;
		private:
			Ui::OverheadsInfoView* _ui;
		};
	}
}

#endif // ELPIDA_OVERHEADSINFOVIEW_HPP
