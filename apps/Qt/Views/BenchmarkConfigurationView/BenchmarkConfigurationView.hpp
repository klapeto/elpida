#ifndef BENCHMARKCONFIGURATIONVIEW_HPP
#define BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>
#include <vector>
#include <unordered_map>

#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkConfigurationView;
	}

	class BenchmarkConfigurationModel;
	class BenchmarkConfigurationInstanceModel;
	class ConfigurationView;
	class ConfigurationViewPool;

	class BenchmarkConfigurationView : public QWidget
	{
	 Q_OBJECT
	 public:
		explicit BenchmarkConfigurationView(const BenchmarkConfigurationModel& configurationModel, ConfigurationViewPool& configurationViewPool);
		~BenchmarkConfigurationView() override;
	 private:
		Ui::BenchmarkConfigurationView* _ui;
		const BenchmarkConfigurationModel& _configurationModel;
		ConfigurationViewPool& _configurationViewPool;
		EventSubscription<const CollectionItem<BenchmarkConfigurationInstanceModel>&> _itemAddedSubscription;
		EventSubscription<const CollectionItem<BenchmarkConfigurationInstanceModel>&> _itemRemovedSubscription;
		EventSubscription<> _clearedSubscription;
		std::unordered_map<const BenchmarkConfigurationInstanceModel*, ConfigurationView*> _rentedViews;

		void ClearViews();
		void Remove(const BenchmarkConfigurationInstanceModel& model);
		void Add(const BenchmarkConfigurationInstanceModel& model);
	};
}

#endif // BENCHMARKCONFIGURATIONVIEW_HPP
