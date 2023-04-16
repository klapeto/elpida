#ifndef BENCHMARKCONFIGURATIONVIEW_HPP
#define BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>
#include <vector>
#include <unordered_map>

#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkConfigurationView;
	}

	class BenchmarkConfigurationModel;
	class BenchmarkConfigurationInstanceModel;
	class ConfigurationView;

	class BenchmarkConfigurationView : public QWidget
	{
	 Q_OBJECT
	 public:
		explicit BenchmarkConfigurationView(BenchmarkConfigurationModel& configurationModel);
		~BenchmarkConfigurationView() override;
	 private:
		Ui::BenchmarkConfigurationView* _ui;
		BenchmarkConfigurationModel& _configurationModel;
		EventSubscription<CollectionItem<BenchmarkConfigurationInstanceModel>&> _itemAddedSubscription;
		EventSubscription<CollectionItem<BenchmarkConfigurationInstanceModel>&> _itemRemovedSubscription;
		EventSubscription<> _clearedSubscription;
		std::vector<ConfigurationView*> _fileViews;
		std::vector<ConfigurationView*> _floatViews;
		std::vector<ConfigurationView*> _integerViews;
		std::vector<ConfigurationView*> _stringViews;
		std::unordered_map<BenchmarkConfigurationInstanceModel*, ConfigurationView*> _createdWidgets;

		void ClearViews();
		ConfigurationView* RentView(BenchmarkConfigurationInstanceModel& configurationModel);
		void ReturnView(BenchmarkConfigurationInstanceModel* configurationModel, ConfigurationView* configurationView);

		template<typename TFactory>
		ConfigurationView* GetOrCreate(std::vector<ConfigurationView*>& pool, TFactory factory)
		{
			if (!pool.empty())
			{
				auto view = pool.back();
				pool.pop_back();
				return view;
			}
			else
			{
				return factory();
			}
		}

		void Remove(BenchmarkConfigurationInstanceModel* model, ConfigurationView* view);
	};
}

#endif // BENCHMARKCONFIGURATIONVIEW_HPP
