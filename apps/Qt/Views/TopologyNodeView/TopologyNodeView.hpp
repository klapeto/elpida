#ifndef ELPIDA_TOPOLOGYNODEVIEW_HPP
#define ELPIDA_TOPOLOGYNODEVIEW_HPP

#include <QFrame>
#include <QGraphicsScene>
#include "Models/TopologyNodeModel.hpp"
#include "EventSubscription.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class TopologyNodeView;
	}

	class TopologyNodeModel;

	class TopologyNodeView : public QFrame
	{
	Q_OBJECT

	signals:
		void clicked(const TopologyNodeView* view);
	public:
		explicit TopologyNodeView(TopologyNodeModel& topologyNodeModel, QWidget* parent = nullptr);

		~TopologyNodeView() override;

	protected:
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;
		void enterEvent(QEvent* event) override;
		void leaveEvent(QEvent* event) override;
	private:
		Ui::TopologyNodeView* _ui;
		TopologyNodeModel& _topologyNodeModel;
		EventSubscription<> _dataChangedSubscription;
		bool _mouseDown;
		bool _mouseOver;
		void OnModelChanged();
	};
}

#endif // ELPIDA_TOPOLOGYNODEVIEW_HPP
