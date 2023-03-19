#ifndef ELPIDA_TOPOLOGYVIEW_HPP
#define ELPIDA_TOPOLOGYVIEW_HPP

#include <QWidget>

namespace Ui
{
	class TopologyView;
}

class TopologyView : public QWidget
{
Q_OBJECT

public:
	explicit TopologyView(QWidget* parent = nullptr);

	~TopologyView() override;

private:
	Ui::TopologyView* _ui;
};

#endif // ELPIDA_TOPOLOGYVIEW_HPP
