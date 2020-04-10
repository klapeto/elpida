/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

#include "TopologyWidget.hpp"
#include "ui_TopologyWidget.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <algorithm>
#include <Elpida/Topology/SystemTopology.hpp>
#include <Elpida/Topology/ProcessorNode.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Exceptions/ElpidaException.hpp>
#include "Ui/TopologyNodeFrame/TopologyNodeFrame.hpp"

#include <vector>
#include <cmath>

namespace Elpida
{
	struct CacheStyles
	{
		QString defaultStyle;
		QString mouseOverStyle;
		QString mouseClickStyle;
	};

	TopologyWidget::TopologyWidget(const SystemTopology& topology)
		: QWidget(), _topology(topology), _ui(new Ui::TopologyWidget), _rootFrame(nullptr)
	{
		_ui->setupUi(this);
		setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		loadTopology();
	}

	TopologyWidget::~TopologyWidget()
	{
		delete _ui;
	}

	TopologyNodeFrame* TopologyWidget::getMachineWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new TopologyNodeFrame(node);
		auto label = new QLabel();
		Elpida::TopologyNodeFrame::connect(widget, &TopologyNodeFrame::clicked, this, &TopologyWidget::onElementClick);
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QVBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(node.getName()));
		widget->setDefaultStyle(QString("background-color: #b1b1b1;border-radius: 8px;"));
		widget->setMouseOverStyle(QString("background-color: #d8d8d8;border-radius: 8px;"));
		widget->setClickedStyle(QString("background-color: #fdfdfd;border-radius: 8px;"));

		return widget;
	}

	TopologyNodeFrame* TopologyWidget::getPackageWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new TopologyNodeFrame(node);
		auto label = new QLabel();
		Elpida::TopologyNodeFrame::connect(widget, &TopologyNodeFrame::clicked, this, &TopologyWidget::onElementClick);
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QHBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(node.getName()));
		widget->setDefaultStyle(QString("background-color: #99da9f;"));
		widget->setMouseOverStyle(QString("background-color: #bcebc1;"));
		widget->setClickedStyle(QString("background-color: #d4f4d7;"));
		return widget;
	}

	TopologyNodeFrame* TopologyWidget::getGroupWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new TopologyNodeFrame(node);
		auto label = new QLabel();
		Elpida::TopologyNodeFrame::connect(widget, &TopologyNodeFrame::clicked, this, &TopologyWidget::onElementClick);
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QVBoxLayout);
		widget->layout()->addWidget(label);
		//label->setText(QString::fromStdString(node.getName()));
		widget->setDefaultStyle(QString("background-color: #db8ced;"));
		widget->setMouseOverStyle(QString("background-color: #eebff9;"));
		widget->setClickedStyle(QString("background-color: #f7dcfd;"));
		return widget;
	}

	QWidget* TopologyWidget::getNumaWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new QWidget();
		auto label = new QLabel();
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QHBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(
			node.getName() + "\n" + ValueUtilities::getValueScaleString(node.getValue()) + "B"));
		widget->setStyleSheet(QString("background-color: #acb4ec;"));
		return widget;
	}

	static CacheStyles getCacheStyleSheet(const Elpida::ProcessorNode& node)
	{
		switch (node.getType())
		{
		case ProcessorNode::Type::L1DCache:
			return
				{ QString("background-color: #fa9e9e;"), QString("background-color: #eba8a8;"),
				  QString("background-color: #ffe4e4;") };
		case ProcessorNode::Type::L1ICache:
			return
				{ QString("background-color: #f5bfbf;"), QString("background-color: #f9d8d8;"),
				  QString("background-color: #ffc1c1;") };
		case ProcessorNode::Type::L2DCache:
			return
				{ QString("background-color: #e48b8b;"), QString("background-color: #e19898;"),
				  QString("background-color: #ffc1c1;") };
		case ProcessorNode::Type::L2ICache:
			return
				{ QString("background-color: #e6abab;"), QString("background-color: #f6c2c2;"),
				  QString("background-color: #fbe9e9;") };
		case ProcessorNode::Type::L3DCache:
			return
				{ QString("background-color: #c67171;"), QString("background-color: #cb8282;"),
				  QString("background-color: #dca0a0;") };
		case ProcessorNode::Type::L3ICache:
			return
				{ QString("background-color: #c68989;"), QString("background-color: #d09f9f;"),
				  QString("background-color: #e2bfbf;") };
		case ProcessorNode::Type::L4Cache:
			return
				{ QString("background-color: #ae5151;"), QString("background-color: #bf7171;"),
				  QString("background-color: #d79d9d;") };
		case ProcessorNode::Type::L5Cache:
			return
				{ QString("background-color: #8f4242;"), QString("background-color: #a76363;"),
				  QString("background-color: #bf8e8e;") };
		default:
			return
				{ QString("background-color: #e1e3aa;"), QString("background-color: #e1e3aa;"),
				  QString("background-color: #e1e3aa;") };
		}
	}

	TopologyNodeFrame* TopologyWidget::getCacheWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new TopologyNodeFrame(node);
		auto label = new QLabel();
		widget->connect(widget, &TopologyNodeFrame::clicked, this, &TopologyWidget::onElementClick);
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		auto layout = node.getChildren().size() > 1 ? (QLayout*)new QVBoxLayout : (QLayout*)new QVBoxLayout;
		widget->setLayout(layout);
		layout->addWidget(label);
		label->setText(QString::fromStdString(
			node.getName() + ": " + ValueUtilities::getValueScaleString(node.getValue()) + "B"));
		auto styles = getCacheStyleSheet(node);
		widget->setDefaultStyle(styles.defaultStyle);
		widget->setMouseOverStyle(styles.mouseOverStyle);
		widget->setClickedStyle(styles.mouseClickStyle);
		return widget;
	}

	TopologyNodeFrame* TopologyWidget::getCoreWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new TopologyNodeFrame(node);
		auto label = new QLabel();
		Elpida::TopologyNodeFrame::connect(widget, &TopologyNodeFrame::clicked, this, &TopologyWidget::onElementClick);
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QVBoxLayout);
		widget->layout()->addWidget(label);
		label->setText(QString::fromStdString(node.getName()));
		widget->setDefaultStyle(QString("background-color: #9297e3;"));
		widget->setMouseOverStyle(QString("background-color: #a0a4f2;"));
		widget->setClickedStyle(QString("background-color: #aeb2ff;"));
		return widget;
	}

	TopologyNodeFrame* TopologyWidget::getEUWidget(const Elpida::ProcessorNode& node)
	{
		auto widget = new TopologyNodeFrame(node);
		auto checkbox = new QCheckBox();
		checkbox->setAttribute(Qt::WA_TransparentForMouseEvents);
		Elpida::TopologyNodeFrame::connect(widget, &TopologyNodeFrame::clicked, this, &TopologyWidget::onElementClick);
		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		widget->setLayout(new QVBoxLayout);
		widget->layout()->addWidget(checkbox);
		checkbox->setText(QString::fromStdString(std::to_string(node.getOsIndex())));
		widget->setDefaultStyle(QString("background-color: #c8caea;"));
		widget->setMouseOverStyle(QString("background-color: #d6d8f8;"));
		widget->setClickedStyle(QString("background-color: #e4e6ff;"));
		widget->setCheckBox(checkbox);
		return widget;
	}

	TopologyNodeFrame* TopologyWidget::getWidget(const Elpida::ProcessorNode& node)
	{
		switch (node.getType())
		{
		case ProcessorNode::Type::Machine:
			return getMachineWidget(node);
		case ProcessorNode::Type::Package:
			return getPackageWidget(node);
		case ProcessorNode::Type::Group:
			return getGroupWidget(node);
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
			throw ElpidaException("Invalid Enumeration");
		}
	}

	TopologyNodeFrame* TopologyWidget::appendChildren(const Elpida::ProcessorNode& node)
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
			auto topoChild = appendChildren(child);
			item->appendChild(topoChild);
			rootLayout->addWidget(topoChild, currRow, currColumn);
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
				rootLayout->addItem(new QSpacerItem(50, 50, QSizePolicy::Expanding, QSizePolicy::Expanding),
					currRow,
					currColumn);
				if (++currColumn >= maxColumns)
				{
					++currRow;
					currColumn = 0;
				}
			}
		}

		((QBoxLayout*)item->layout())->addLayout(rootLayout);
		return item;
	}

	void TopologyWidget::loadTopology()
	{
		auto& top = _topology;
		QLayout* layout = new QHBoxLayout();

		auto button = new QPushButton("Clear");
		button->connect(button, &QPushButton::pressed, this, &TopologyWidget::onClearPressed);

		_rootFrame = appendChildren(*top.getRoot());
		layout->addWidget(button);
		layout->addWidget(_rootFrame);
		setLayout(layout);
	}

	void TopologyWidget::clearChildrenState(TopologyNodeFrame* frame)
	{
		if (frame->getProcessorNode().getType() == ProcessorNode::Type::ExecutionUnit)
		{
			auto checkBox = frame->getCheckBox();
			if (checkBox != nullptr)
			{
				checkBox->setChecked(false);
			}
		}
		else
		{
			const auto& childs = frame->getChildren();
			for (auto child : childs)
			{
				clearChildrenState(child);
			}
		}
	}

	void TopologyWidget::appendAffinity(TopologyNodeFrame* frame)
	{
		if (frame->getProcessorNode().getType() != ProcessorNode::Type::ExecutionUnit)
		{
			const auto& childs = frame->getChildren();
			for (auto child : childs)
			{
				appendAffinity(child);
			}
		}
		else
		{
			auto checkBox = frame->getCheckBox();
			if (checkBox != nullptr)
			{
				checkBox->setChecked(true);

				if (std::none_of(_selectedNodes.begin(), _selectedNodes.end(), [&](const ProcessorNode* nd)
				{ return &frame->getProcessorNode() == nd; }))
				{
					_selectedNodes.push_front(&frame->getProcessorNode());
				}
			}
		}
	}

	void TopologyWidget::onElementClick(const TopologyNodeFrame* node)
	{
		if (node->getProcessorNode().getType() != ProcessorNode::Type::ExecutionUnit)
		{
			const auto& childs = node->getChildren();
			for (auto child : childs)
			{
				appendAffinity(child);
			}
		}
		else
		{
			auto checkBox = node->getCheckBox();
			if (checkBox != nullptr)
			{
				if (checkBox->isChecked())
				{
					checkBox->setChecked(false);
					_selectedNodes.remove(&node->getProcessorNode());
				}
				else
				{
					checkBox->setChecked(true);
					if (std::none_of(_selectedNodes.begin(), _selectedNodes.end(), [&](const ProcessorNode* nd)
					{ return &node->getProcessorNode() == nd; }))
					{
						_selectedNodes.push_front(&node->getProcessorNode());
					}
				}
			}
		}
		_affinity = TaskAffinity(_selectedNodes);
	}

	void TopologyWidget::onClearPressed()
	{
		const auto& childs = _rootFrame->getChildren();
		for (auto child : childs)
		{
			clearChildrenState(child);
		}
		_selectedNodes.clear();
	}

} // namespace Elpida
