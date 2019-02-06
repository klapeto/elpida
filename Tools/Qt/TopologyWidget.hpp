#ifndef ELPIDA_TOPOLOGYWIDGET_HPP
#define ELPIDA_TOPOLOGYWIDGET_HPP

#include <QWidget>

namespace Elpida
{

	namespace Ui
	{
		class TopologyWidget;
	}

	class TopologyWidget: public QWidget
	{
		Q_OBJECT

		public:
			explicit TopologyWidget(QWidget *parent = nullptr);
			~TopologyWidget();

		private:
			Ui::TopologyWidget* _ui;
			void loadTopology();
	};

} // namespace Elpida
#endif // ELPIDA_TOPOLOGYWIDGET_HPP
