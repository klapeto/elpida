#ifndef ELPIDA_MEMORYINFOVIEW_HPP
#define ELPIDA_MEMORYINFOVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{
	namespace Ui
	{
		class MemoryInfoView;
	}

	class MemoryInfoModel;

	class MemoryInfoView : public QWidget
	{
	Q_OBJECT

	public:
		explicit MemoryInfoView(const MemoryInfoModel& memoryInfo, QWidget* parent = nullptr);

		~MemoryInfoView() override;

	private:
		Ui::MemoryInfoView* _ui;
	};
}

#endif // ELPIDA_MEMORYINFOVIEW_HPP
