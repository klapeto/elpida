#ifndef BENCHMARKCONFIGURATIONVIEW_HPP
#define BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>

#include <vector>

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkConfigurationView;
	}

	class BenchmarkModel;
	class ConfigurationModel;
	class ConfigurationView;

	class BenchmarkConfigurationView : public QWidget
	{
	 Q_OBJECT
	 public:
		void SetModel(BenchmarkModel* benchmarkModel);

		explicit BenchmarkConfigurationView();
		~BenchmarkConfigurationView() override;
	 private:
		Ui::BenchmarkConfigurationView* _ui;
		BenchmarkModel* _benchmarkModel;
		std::vector<ConfigurationView*> _fileViews;
		std::vector<ConfigurationView*> _floatViews;
		std::vector<ConfigurationView*> _integerViews;
		std::vector<ConfigurationView*> _stringViews;
		std::vector<std::tuple<ConfigurationModel*, ConfigurationView*>> _createdWidgets;

		void ClearViews();
		ConfigurationView* RentView(ConfigurationModel& configurationModel);
		void ReturnView(ConfigurationModel* configurationModel, ConfigurationView* configurationView);

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
	};
}

#endif // BENCHMARKCONFIGURATIONVIEW_HPP
