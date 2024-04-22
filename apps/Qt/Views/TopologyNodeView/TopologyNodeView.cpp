#include "TopologyNodeView.hpp"
#include "ui_TopologyNodeView.h"

#include "Layouts/FlowLayout.hpp"
#include <QGridLayout>

#include <cmath>

#include "Elpida/Core/ValueUtilities.hpp"

using Vu = Elpida::ValueUtilities;

namespace Elpida::Application
{
	const int contentMargins = 1;

	static QString Names[] = {
			"Machine",
			"Package",
			"NumaDomain",
			"Group",
			"Die",
			"Core",
			"L1I",
			"L1D",
			"L2I",
			"L2D",
			"L3I",
			"L3D",
			"L4",
			"L5",
			"PU"
	};

	static const QString DefaultStyles[] = {
			"border-radius: 0.5em;background-color:#aebfef",   //Machine
			"border-radius: 0.5em;background-color:#99da9f",   //Package
			"border-radius: 0.5em;background-color:#d4d4a1",   //NumaDomain
			"border-radius: 0.5em;background-color:#db8ced",   //Group
			"border-radius: 0.5em;background-color:#a9a9a9",   //Die
			"border-radius: 0.5em;background-color:#9297e3",   //Core
			"border-radius: 0.5em;background-color:#f5bfbf",   //L1I
			"border-radius: 0.5em;background-color:#fa9e9e",   //L1D
			"border-radius: 0.5em;background-color:#e6abab",   //L2I
			"border-radius: 0.5em;background-color:#e48b8b",   //L2D
			"border-radius: 0.5em;background-color:#c68989",   //L3I
			"border-radius: 0.5em;background-color:#c67171",   //L3D
			"border-radius: 0.5em;background-color:#ae5151",   //L4
			"border-radius: 0.5em;background-color:#8f4242",   //L5
			"border-radius: 0.5em;background-color:#c8caea"    //PU
	};//

	static const QString HoverStyles[] = {
			"border-radius: 0.5em;background-color:#c9d3ee",   //Machine
			"border-radius: 0.5em;background-color:#bcebc1",   //Package
			"border-radius: 0.5em;background-color:#d4d4a1",   //NumaDomain
			"border-radius: 0.5em;background-color:#eebff9",   //Group
			"border-radius: 0.5em;background-color:#c0c0c0",   //Die
			"border-radius: 0.5em;background-color:#a0a4f2",   //Core
			"border-radius: 0.5em;background-color:#f9d8d8",   //L1I
			"border-radius: 0.5em;background-color:#eba8a8",   //L1D
			"border-radius: 0.5em;background-color:#f6c2c2",   //L2I
			"border-radius: 0.5em;background-color:#e19898",   //L2D
			"border-radius: 0.5em;background-color:#d09f9f",   //L3I
			"border-radius: 0.5em;background-color:#cb8282",   //L3D
			"border-radius: 0.5em;background-color:#bf7171",   //L4
			"border-radius: 0.5em;background-color:#a76363",   //L5
			"border-radius: 0.5em;background-color:#d6d8f8"    //PU
	};

	static const QString ClickStyles[] = {
			"border-radius: 0.5em;background-color:#d8dff3",   //Machine
			"border-radius: 0.5em;background-color:#d4f4d7",   //Package
			"border-radius: 0.5em;background-color:#d4d4a1",   //NumaDomain
			"border-radius: 0.5em;background-color:#f7dcfd",   //Group
			"border-radius: 0.5em;background-color:#dbdbdb",   //Die
			"border-radius: 0.5em;background-color:#aeb2ff",   //Core
			"border-radius: 0.5em;background-color:#ffc1c1",   //L1I
			"border-radius: 0.5em;background-color:#ffe4e4",   //L1D
			"border-radius: 0.5em;background-color:#fbe9e9",   //L2I
			"border-radius: 0.5em;background-color:#ffc1c1",   //L2D
			"border-radius: 0.5em;background-color:#e2bfbf",   //L3I
			"border-radius: 0.5em;background-color:#dca0a0",   //L3D
			"border-radius: 0.5em;background-color:#d79d9d",   //L4
			"border-radius: 0.5em;background-color:#bf8e8e",   //L5
			"border-radius: 0.5em;background-color:#e4e6ff"    //PU
	};

	static QWidget* AppendChildren(std::vector<TopologyNodeModel>& children)
	{
		int maxColumns = std::ceil(std::sqrt(children.size()));
		auto currColumn = 0;
		auto currRow = 0;
		auto widget = new QWidget();
		auto rootLayout = new QGridLayout();
		for (auto& child: children)
		{
			auto childView = new TopologyNodeView(child);
			rootLayout->addWidget(childView, currRow, currColumn);
			if (++currColumn >= maxColumns)
			{
				++currRow;
				currColumn = 0;
			}
		}

		widget->setLayout(rootLayout);
		return widget;
	}

	TopologyNodeView::TopologyNodeView(TopologyNodeModel& topologyNodeModel, QWidget* parent) :
			QFrame(parent),
			_ui(new Ui::TopologyNodeView), _topologyNodeModel(topologyNodeModel), _uiUpdating(false)
	{
		_ui->setupUi(this);

		setContentsMargins(contentMargins, contentMargins, contentMargins, contentMargins);

		_dataChangedSubscription = _topologyNodeModel.DataChanged().Subscribe([this]()
		{
			OnModelChanged();
		});

		if (!_topologyNodeModel.GetMemoryChildren().empty())
		{
			_ui->verticalLayout->addWidget(AppendChildren(_topologyNodeModel.GetMemoryChildren()));
		}

		if (!_topologyNodeModel.GetChildren().empty())
		{
			_ui->verticalLayout->addWidget(AppendChildren(_topologyNodeModel.GetChildren()));
		}

		_ui->lblName->setText(Names[(int)_topologyNodeModel.GetType()]);

		if (_topologyNodeModel.GetSize().has_value())
		{
			_ui->lblValue->setText(QString::fromStdString(
					Vu::Cs(ValueUtilities::GetValueScaleStringIEC(_topologyNodeModel.GetSize().value()), "B")));
		}
		else
		{
			_ui->lblValue->setVisible(false);
		}

		if (_topologyNodeModel.GetOsIndex().has_value())
		{
			_ui->lblOsIndex->setText(
					QString::fromStdString(Vu::Cs("(", _topologyNodeModel.GetOsIndex().value(), ")")));
		}
		else
		{
			_ui->lblOsIndex->setVisible(false);
		}

		this->setStyleSheet(DefaultStyles[(int)_topologyNodeModel.GetType()]);

		if (_topologyNodeModel.GetType() != TopologyNodeType::ProcessingUnit)
		{
			_ui->chkSelected->setVisible(false);
			_ui->lblEfficiencyValue->setVisible(false);
		}
		else
		{
			_ui->lblName->setVisible(false);
			auto efficiency = _topologyNodeModel.GetEfficiency();
			if (efficiency.has_value())
			{
				_ui->lblEfficiencyValue->setText(QString::fromStdString(
						Vu::Cs("[", std::to_string(efficiency.value()), "]"))
				);
			}
			else
			{
				_ui->lblEfficiencyValue->setVisible(false);
			}
		}

		_ui->verticalLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

		OnModelChanged();
	}

	TopologyNodeView::~TopologyNodeView()
	{
		delete _ui;
	}

	void TopologyNodeView::OnModelChanged()
	{
		if (_uiUpdating) return;

		auto selected = _topologyNodeModel.IsSelected();
		_uiUpdating = true;

		_ui->chkSelected->setCheckState(selected ? Qt::Checked : Qt::Unchecked);
		_uiUpdating = false;
	}

	void TopologyNodeView::mousePressEvent(QMouseEvent* event)
	{
		setStyleSheet(ClickStyles[(int)_topologyNodeModel.GetType()]);
	}

	void TopologyNodeView::mouseReleaseEvent(QMouseEvent* event)
	{
		setStyleSheet(HoverStyles[(int)_topologyNodeModel.GetType()]);

		_topologyNodeModel.SetSelected(!_topologyNodeModel.IsSelected());
	}

	void TopologyNodeView::enterEvent(QEvent* event)
	{
		setStyleSheet(HoverStyles[(int)_topologyNodeModel.GetType()]);
	}

	void TopologyNodeView::leaveEvent(QEvent* event)
	{
		setStyleSheet(DefaultStyles[(int)_topologyNodeModel.GetType()]);
	}

	void TopologyNodeView::on_chkSelected_stateChanged(int state)
	{
		_uiUpdating = true;

		_topologyNodeModel.SetSelected(state == Qt::CheckState::Checked);

		_uiUpdating = false;
	}
}
