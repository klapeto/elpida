//
// Created by klapeto on 20/3/2023.
//

#ifndef ELPIDA_TOPOLOGYNODEMODEL_HPP
#define ELPIDA_TOPOLOGYNODEMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include <cstdlib>
#include <optional>
#include <vector>

namespace Elpida::Application
{

	enum class TopologyNodeType
	{
		Machine,
		Package,
		NumaDomain,
		Group,
		Die,
		Core,
		L1ICache,
		L1DCache,
		L2ICache,
		L2DCache,
		L3ICache,
		L3DCache,
		L4Cache,
		L5Cache,
		ProcessingUnit
	};

	class TopologyNodeModel final : public Model
	{
	public:

		void SetSelected(bool selected);

		[[nodiscard]]
		bool CanBeSelected() const;

		[[nodiscard]]
		std::vector<TopologyNodeModel>& GetChildren();

		[[nodiscard]]
		std::vector<TopologyNodeModel>& GetMemoryChildren();

		[[nodiscard]]
		TopologyNodeType GetType() const;

		[[nodiscard]]
		std::optional<std::size_t> GetOsIndex() const;

		[[nodiscard]]
		std::optional<std::size_t> GetSize() const;

		[[nodiscard]]
		bool IsSelected() const;

		TopologyNodeModel() = default;
		TopologyNodeModel(
				TopologyNodeType type,
				std::optional<std::size_t> osIndex,
				std::optional<std::size_t> size,
				std::vector<TopologyNodeModel>&& children,
				std::vector<TopologyNodeModel>&& memoryChildren);
		TopologyNodeModel(const TopologyNodeModel&) = delete;
		TopologyNodeModel(TopologyNodeModel&&) noexcept = default;
		TopologyNodeModel& operator=(TopologyNodeModel&&) noexcept = default;
		TopologyNodeModel& operator=(const TopologyNodeModel&) = delete;
		~TopologyNodeModel() override = default;
	private:
		std::vector<TopologyNodeModel> _children;
		std::vector<TopologyNodeModel> _memoryChildren;
		TopologyNodeType _type;
		std::optional<std::size_t> _osIndex;
		std::optional<std::size_t> _size;
		bool _selected;
	};

} // Application

#endif //ELPIDA_TOPOLOGYNODEMODEL_HPP
