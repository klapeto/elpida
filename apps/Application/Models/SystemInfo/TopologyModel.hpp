/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

		[[nodiscard]]
		unsigned int GetFastestProcessor() const;

		[[nodiscard]]
		const TopologyNodeModel& GetRoot() const;

		[[nodiscard]]
		std::size_t GetTotalLogicalCores() const;

		[[nodiscard]]
		std::size_t GetTotalPhysicalCores() const;

		[[nodiscard]]
		std::size_t GetTotalNumaNodes() const;

		[[nodiscard]]
		std::size_t GetTotalPackages() const;

		const std::vector<std::reference_wrapper<const TopologyNodeModel>>& GetSelectedLeafNodes() const;
		const std::vector<std::reference_wrapper<const TopologyNodeModel>>& GetLeafNodes() const;

		const std::vector<std::reference_wrapper<TopologyNodeModel>>& GetSelectedLeafNodes();
		const std::vector<std::reference_wrapper<TopologyNodeModel>>& GetLeafNodes();

		TopologyModel() = default;
		explicit TopologyModel(TopologyNodeModel root, unsigned int fastestProcessor);
		TopologyModel(const TopologyModel&) = delete;
		TopologyModel& operator=(const TopologyModel&) = delete;
		TopologyModel(TopologyModel&&) noexcept;
		TopologyModel& operator=(TopologyModel&&) noexcept;
		~TopologyModel() override = default;
	private:
		TopologyNodeModel _root;
		mutable std::vector<std::reference_wrapper<const TopologyNodeModel>> _selectedLeafNodes;
		mutable std::vector<std::reference_wrapper<const TopologyNodeModel>> _leafNodes;
		EventSubscription<> _rootDataChanged;
		unsigned int _fastestProcessor;
		std::size_t _totalPackages;
		std::size_t _totalNumaNodes;
		std::size_t _totalPhysicalCores;
		std::size_t _totalLogicalCores;
		void SetSelectedLeafNodes();

		void CountNode(const TopologyNodeModel& node);
	};

} // Application

#endif //ELPIDA_TOPOLOGYMODEL_HPP
