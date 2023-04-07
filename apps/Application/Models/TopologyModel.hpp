//
// Created by klapeto on 20/3/2023.
//

#ifndef ELPIDA_TOPOLOGYMODEL_HPP
#define ELPIDA_TOPOLOGYMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Models/TopologyNodeModel.hpp"
#include "EventSubscription.hpp"

namespace Elpida::Application
{

	class TopologyModel : public Model
	{
	public:
		[[nodiscard]]
		TopologyNodeModel& GetRoot();

		std::vector<std::reference_wrapper<TopologyNodeModel>> GetSelectedLeafNodes();

		explicit TopologyModel(TopologyNodeModel root);
		TopologyModel(const TopologyModel&) = delete;
		TopologyModel& operator=(const TopologyModel&) = delete;
		TopologyModel(TopologyModel&&) noexcept = delete;
		TopologyModel& operator=(TopologyModel&&) noexcept = delete;
		~TopologyModel() override = default;
	private:
		TopologyNodeModel _root;
		EventSubscription<> _rootDataChanged;
	};

} // Application

#endif //ELPIDA_TOPOLOGYMODEL_HPP
