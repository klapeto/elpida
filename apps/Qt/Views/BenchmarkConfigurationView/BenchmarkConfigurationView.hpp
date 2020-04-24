#ifndef ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
#define ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>

class QVBoxLayout;
class QListWidgetItem;

namespace Elpida
{
	class BenchmarkConfigurationModel;
	class EventSubscriptionBase;
	class ConfigurationViewsPool;
	class ConfigurationValueViewBase;

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
		std::vector<ConfigurationValueViewBase*> _rentedViews;
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
		void returnAllTaskListItems() const;
		static void taskListItemClicked(QListWidgetItem* item);
	};

} // namespace Elpida
#endif // ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
