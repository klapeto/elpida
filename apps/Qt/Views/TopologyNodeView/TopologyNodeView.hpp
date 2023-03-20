#ifndef ELPIDA_TOPOLOGYNODEVIEW_HPP
#define ELPIDA_TOPOLOGYNODEVIEW_HPP

#include <QFrame>

namespace Ui
{
	class TopologyNodeView;
}
namespace Elpida::Application
{
	class TopologyNodeView : public QFrame
	{
	Q_OBJECT

	public:
		explicit TopologyNodeView(QWidget* parent = nullptr);

		~TopologyNodeView() override;

	private:
		Ui::TopologyNodeView* _ui;
	};
}

#endif // ELPIDA_TOPOLOGYNODEVIEW_HPP
