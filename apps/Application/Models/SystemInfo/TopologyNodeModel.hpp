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
		ProcessingUnit,
	};

	enum NativeNodeType
	{
		Machine = 0,
		Package = 1,
		NumaDomain = 1 << 1,
		Group = 1 << 2,
		Die = 1 << 3,
		Core = 1 << 4,
		L1ICache = 1 << 5,
		L1DCache = 1 << 6,
		L2ICache = 1 << 7,
		L2DCache = 1 << 8,
		L3ICache = 1 << 9,
		L3DCache = 1 << 10,
		L4Cache = 1 << 11,
		L5Cache = 1 << 12,
		ProcessingUnit = 1 << 13,
		Unknown = -1
	};

	class TopologyNodeModel final : public Model
	{
	public:

		void SetSelected(bool selected);

		[[nodiscard]]
		const std::vector<TopologyNodeModel>& GetChildren() const;

		[[nodiscard]]
		std::vector<TopologyNodeModel>& GetChildren();

		[[nodiscard]]
		const std::vector<TopologyNodeModel>& GetMemoryChildren() const;

		[[nodiscard]]
		std::vector<TopologyNodeModel>& GetMemoryChildren();

		[[nodiscard]]
		TopologyNodeType GetType() const;

		[[nodiscard]]
		std::optional<std::size_t> GetOsIndex() const;

		[[nodiscard]]
		std::optional<std::size_t> GetSize() const;

		[[nodiscard]]
		std::optional<int> GetEfficiency() const;

		[[nodiscard]]
		const std::optional<std::reference_wrapper<TopologyNodeModel>>& GetLastCache() const;

		[[nodiscard]]
		const std::optional<std::reference_wrapper<TopologyNodeModel>>& GetParent() const;

		[[nodiscard]]
		bool IsSelected() const;

		void SetRelations();

		TopologyNodeModel() = default;
		TopologyNodeModel(
				TopologyNodeType type,
				std::optional<std::size_t> osIndex,
				std::optional<std::size_t> size,
				std::optional<int> efficiency,
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
		std::optional<int> _efficiency;
		std::optional<std::reference_wrapper<TopologyNodeModel>> _lastCache;
		std::optional<std::reference_wrapper<TopologyNodeModel>> _parent;
		bool _selected;

		void SetSelectedInternal(bool selected);
	protected:
		void OnDataChanged() const override;
	};

	static TopologyNodeType TranslateNativeTopologyType(NativeNodeType nativeNodeType)
	{
		switch (nativeNodeType)
		{
		case Machine:
			return Elpida::Application::TopologyNodeType::Machine;
		case Package:
			return Elpida::Application::TopologyNodeType::Package;
		case NumaDomain:
			return Elpida::Application::TopologyNodeType::NumaDomain;
		case Group:
			return Elpida::Application::TopologyNodeType::Group;
		case Die:
			return Elpida::Application::TopologyNodeType::Die;
		case Core:
			return Elpida::Application::TopologyNodeType::Core;
		case L1ICache:
			return Elpida::Application::TopologyNodeType::L1ICache;
		case L1DCache:
			return Elpida::Application::TopologyNodeType::L1DCache;
		case L2ICache:
			return Elpida::Application::TopologyNodeType::L2ICache;
		case L2DCache:
			return Elpida::Application::TopologyNodeType::L2DCache;
		case L3ICache:
			return Elpida::Application::TopologyNodeType::L3ICache;
		case L3DCache:
			return Elpida::Application::TopologyNodeType::L3DCache;
		case L4Cache:
			return Elpida::Application::TopologyNodeType::L4Cache;
		case L5Cache:
			return Elpida::Application::TopologyNodeType::L5Cache;
		case ProcessingUnit:
			return Elpida::Application::TopologyNodeType::ProcessingUnit;
		case Unknown:
			break;
		}
		return Elpida::Application::TopologyNodeType::ProcessingUnit;
	}

} // Application

#endif //ELPIDA_TOPOLOGYNODEMODEL_HPP
