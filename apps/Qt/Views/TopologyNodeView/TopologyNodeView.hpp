#ifndef ELPIDA_TOPOLOGYNODEVIEW_HPP
#define ELPIDA_TOPOLOGYNODEVIEW_HPP

#include <QFrame>
#include <optional>

#include "Models/SystemInfo/TopologyNodeModel.hpp"
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
	public:
		explicit TopologyNodeView(TopologyNodeModel& topologyNodeModel, std::optional<unsigned int>& fastestProcessor, QWidget* parent = nullptr);
		~TopologyNodeView() override;
	protected:
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;
		void enterEvent(QEvent* event) override;
		void leaveEvent(QEvent* event) override;
	private slots:
		void on_chkSelected_stateChanged(int state);
	private:
		Ui::TopologyNodeView* _ui;
		TopologyNodeModel& _topologyNodeModel;
		EventSubscription<> _dataChangedSubscription;
		bool _uiUpdating;
		bool _fastest;
		void OnModelChanged();
	};
}

#endif // ELPIDA_TOPOLOGYNODEVIEW_HPP
