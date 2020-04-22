#ifndef ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
#define ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>

namespace Elpida
{

	namespace Ui
	{
		class BenchmarkConfigurationView;
	}

	class BenchmarkConfigurationView : public QWidget
	{
	Q_OBJECT

	public:
		explicit BenchmarkConfigurationView();
		~BenchmarkConfigurationView() override;

	private:
		Ui::BenchmarkConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
