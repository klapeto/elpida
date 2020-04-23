#ifndef ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
#define ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>

class QVBoxLayout;

namespace Elpida
{
	class BenchmarkConfigurationModel;
	class EventSubscriptionBase;
	class ConfigurationViewsPool;
	class ConfigurationViewBase;

	namespace Ui
	{
		class BenchmarkConfigurationView;
	}

	class BenchmarkConfigurationView : public QWidget
	{
	Q_OBJECT

	public:
		explicit BenchmarkConfigurationView(BenchmarkConfigurationModel& benchmarkConfigurationModel,
			ConfigurationViewsPool& configurationViewsPool);
		~BenchmarkConfigurationView() override;

	private:
		std::vector<ConfigurationViewBase*> _rentedViews;
		BenchmarkConfigurationModel& _benchmarkConfigurationModel;
		ConfigurationViewsPool& _configurationViewsPool;
		EventSubscriptionBase* _dataChangedSubscription;
		Ui::BenchmarkConfigurationView* _ui;
		QVBoxLayout* _containerLayout;

	signals:
		void dataChanged();

	private slots:
		void dataChangedHandler();
		void returnAllViewsToPool();
	};

} // namespace Elpida
#endif // ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
