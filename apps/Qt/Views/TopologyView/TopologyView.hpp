#ifndef ELPIDA_TOPOLOGYVIEW_HPP
#define ELPIDA_TOPOLOGYVIEW_HPP

#include <QWidget>

namespace Ui
{
	class TopologyView;
}

namespace Elpida::Application
{
	class TopologyNodeModel;

	class TopologyView : public QWidget
	{
	Q_OBJECT
	public:
		explicit TopologyView(TopologyNodeModel& model, QWidget* parent);

		~TopologyView() override;

	private:
		Ui::TopologyView* _ui;
		TopologyNodeModel& _model;
	};
}

#endif // ELPIDA_TOPOLOGYVIEW_HPP
