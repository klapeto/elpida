#ifndef ELPIDA_MEMORYINFOVIEW_HPP
#define ELPIDA_MEMORYINFOVIEW_HPP

#include <QWidget>

namespace Ui
{
	class MemoryInfoView;
}

namespace Elpida
{
	class MemoryInfo;

	namespace Application
	{
		class MemoryInfoView : public QWidget
		{
		Q_OBJECT

		public:
			explicit MemoryInfoView(const MemoryInfo& memoryInfo, QWidget* parent = nullptr);

			~MemoryInfoView() override;

		private:
			Ui::MemoryInfoView* _ui;
		};
	}
}

#endif // ELPIDA_MEMORYINFOVIEW_HPP
