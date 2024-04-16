//
// Created by klapeto on 20/3/2023.
//

#ifndef ELPIDA_TOPOLOGYMODEL_HPP
#define ELPIDA_TOPOLOGYMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "TopologyNodeModel.hpp"
#include "EventSubscription.hpp"

namespace Elpida::Application
{

	class TopologyModel : public Model
	{
	public:
		[[nodiscard]]
		TopologyNodeModel& GetRoot();

		const std::vector<std::reference_wrapper<const TopologyNodeModel>>& GetSelectedLeafNodes() const;
		const std::vector<std::reference_wrapper<const TopologyNodeModel>>& GetLeafNodes() const;

		const std::vector<std::reference_wrapper<TopologyNodeModel>>& GetSelectedLeafNodes();
		const std::vector<std::reference_wrapper<TopologyNodeModel>>& GetLeafNodes();

		TopologyModel() = default;
		explicit TopologyModel(TopologyNodeModel root);
		TopologyModel(const TopologyModel&) = delete;
		TopologyModel& operator=(const TopologyModel&) = delete;
		TopologyModel(TopologyModel&&) noexcept = default;
		TopologyModel& operator=(TopologyModel&&) noexcept = default;
		~TopologyModel() override = default;
	private:
		TopologyNodeModel _root;
		mutable std::vector<std::reference_wrapper<const TopologyNodeModel>> _selectedLeafNodes;
		mutable std::vector<std::reference_wrapper<const TopologyNodeModel>> _leafNodes;
		EventSubscription<> _rootDataChanged;
		void SetSelectedLeafNodes();
	};

} // Application

#endif //ELPIDA_TOPOLOGYMODEL_HPP
