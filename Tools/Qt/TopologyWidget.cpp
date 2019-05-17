#include "TopologyWidget.hpp"
#include "ui_TopologyWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <Elpida/Topology/SystemTopology.hpp>
#include <Elpida/Topology/ProcessorNode.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>

#include <cmath>

namespace Elpida
{

	TopologyWidget::TopologyWidget(QWidget *parent)
			: QWidget(parent), _ui(new Ui::TopologyWidget)
	{
		_ui->setupUi(this);
		setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		loadTopology();
	}

	TopologyWidget::~TopologyWidget()
	{
		delete _ui;
	}

	static QWidget* getMachineWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new QWidget();
		auto label = new QLabel();
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QVBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(node.getName()));
		widget->setStyleSheet(QString("background-color: rgb(255, 255, 255);border-radius: 8px; "));
		return widget;
	}

	static QWidget* getPackageWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new QWidget();
		auto label = new QLabel();
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QHBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(node.getName()));
		widget->setStyleSheet(QString("background-color: #99da9f;"));
		return widget;
	}

	static QWidget* getGroupWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new QWidget();
		auto label = new QLabel();
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QVBoxLayout);
		widget->layout()->addWidget(label);
		//label->setText(QString::fromStdString(node.getName()));
		widget->setStyleSheet(QString("background-color: #db8ced;"));
		return widget;
	}

	static QWidget* getNumaWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new QWidget();
		auto label = new QLabel();
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QHBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(node.getName() + "\n" + ValueUtilities::getValueScaleString(node.getValue()) + "B"));
		widget->setStyleSheet(QString("background-color: #acb4ec;"));
		return widget;
	}

	static QString getCacheStyleSheet(const Elpida::ProcessorNode& node)
	{
		switch (node.getType())
		{
			case ProcessorNode::Type::L1DCache:
				return QString("background-color: #fa9e9e;");
			case ProcessorNode::Type::L1ICache:
				return QString("background-color: #f8c2c2;");
			case ProcessorNode::Type::L2DCache:
				return QString("background-color: #e48b8b;");
			case ProcessorNode::Type::L2ICache:
				return QString("background-color: #e6abab;");
			case ProcessorNode::Type::L3DCache:
				return QString("background-color: #c67171;");
			case ProcessorNode::Type::L3ICache:
				return QString("background-color: #c68989;");
			case ProcessorNode::Type::L4Cache:
				return QString("background-color: #ae5151;");
			case ProcessorNode::Type::L5Cache:
				return QString("background-color: #8f4242;");
			default:
				return QString("background-color: #e1e3aa;");
		}
	}

	static QWidget* getCacheWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new QWidget();
		auto label = new QLabel();
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		auto layout = node.getChildren().size() > 1 ? (QLayout*) new QVBoxLayout : (QLayout*) new QVBoxLayout;
		widget->setLayout(layout);
		layout->addWidget(label);
		label->setText(QString::fromStdString(node.getName() + ": " + ValueUtilities::getValueScaleString(node.getValue()) + "B"));
		widget->setStyleSheet(getCacheStyleSheet(node));
		return widget;
	}

	static QWidget* getCoreWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new QWidget();
		auto label = new QLabel();
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QVBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(node.getName()));
		widget->setStyleSheet(QString("background-color: #9297e3;"));
		return widget;
	}

	static QWidget* getEUWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new QWidget();
		auto label = new QLabel();
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QVBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(node.getName()));
		widget->setStyleSheet(QString("background-color: #c8caea;"));
		return widget;
	}

	static QWidget* getWidget(const Elpida::ProcessorNode& node)
	{
		switch (node.getType())
		{
			case ProcessorNode::Type::Machine:
				return getMachineWidget(node);
			case ProcessorNode::Type::Package:
				return getPackageWidget(node);
			case ProcessorNode::Type::Group:
				return getGroupWidget(node);
			case ProcessorNode::Type::NumaNode:
				return getNumaWidget(node);
			case ProcessorNode::Type::L1DCache:
			case ProcessorNode::Type::L1ICache:
			case ProcessorNode::Type::L2DCache:
			case ProcessorNode::Type::L2ICache:
			case ProcessorNode::Type::L3DCache:
			case ProcessorNode::Type::L3ICache:
			case ProcessorNode::Type::L4Cache:
			case ProcessorNode::Type::L5Cache:
				return getCacheWidget(node);
			case ProcessorNode::Type::Core:
				return getCoreWidget(node);
			case ProcessorNode::Type::ExecutionUnit:
				return getEUWidget(node);
			default:
				return new QLabel(QString::fromStdString(node.getName()));
		}
		return new QLabel(QString::fromStdString(node.getName()));
	}

	static QWidget* appendChildren(const Elpida::ProcessorNode& node)
	{
		const auto& children = node.getChildren();
		unsigned maxChildren = 0;
		for (auto sibling : node.getSiblings())
		{
			if (sibling->getChildren().size() > maxChildren)
			{
				maxChildren = sibling->getChildren().size();
			}
		}
		if (children.size() > maxChildren)
		{
			maxChildren = children.size();
		}
		int maxColumns = std::ceil(sqrt(maxChildren));
		//int maxColumns = node.getType() != ProcessorNode::Type::Group ? std::ceil(sqrt(maxChildren)) : 1;
		auto currColumn = 0;
		auto currRow = 0;
		auto item = getWidget(node);

		auto rootLayout = new QGridLayout;
		if (node.getMemoryChildren().size() > 0)
		{
			for (auto& memChild : node.getMemoryChildren())
			{
				rootLayout->addWidget(getNumaWidget(memChild), currRow++, 0, 1, maxColumns);
			}
		}

		for (auto& child : children)
		{
			rootLayout->addWidget(appendChildren(child), currRow, currColumn);
			if (++currColumn >= maxColumns)
			{
				++currRow;
				currColumn = 0;
			}
		}

		if (children.size() < maxChildren)
		{
			for (size_t i = 0; i < maxChildren - children.size(); i++)
			{
				rootLayout->addItem(new QSpacerItem(50, 50, QSizePolicy::Expanding, QSizePolicy::Expanding), currRow, currColumn);
				if (++currColumn >= maxColumns)
				{
					++currRow;
					currColumn = 0;
				}
			}
		}

		((QBoxLayout*) item->layout())->addLayout(rootLayout);
		return item;
	}

	void TopologyWidget::loadTopology()
	{
		auto& top = Elpida::SystemTopology::getTopology();
		QLayout* layout = new QHBoxLayout();
		layout->addWidget(appendChildren(*top.getRoot()));
		setLayout(layout);
	}

} // namespace Elpida
