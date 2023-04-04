#ifndef ELPIDA_TOPOLOGYVIEW_HPP
#define ELPIDA_TOPOLOGYVIEW_HPP

#include <QWidget>
#include <QGraphicsScene>

namespace Elpida::Application
{
	namespace Ui
	{
		class TopologyView;
	}

	class TopologyModel;

	class TopologyView : public QWidget
	{
	Q_OBJECT
	public:
		explicit TopologyView(TopologyModel& model, QWidget* parent = nullptr);

		~TopologyView() override;

	private:
		Ui::TopologyView* _ui;
		QGraphicsScene _scene;
		TopologyModel& _model;
	};
}

#endif // ELPIDA_TOPOLOGYVIEW_HPP
